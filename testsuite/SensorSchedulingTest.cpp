/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// Public sensor scheduling regressions run in a fresh process so the global
// realTime timer and sensor queues do not interfere with CoinTests.
#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodes/SoSeparator.h>

#include <iostream>
#include <stdexcept>
#include <string>

namespace {
void check(bool condition, const char * message)
{
  if (!condition) throw std::runtime_error(message);
}
void count(void * data, SoSensor *)
{
  ++*static_cast<int *>(data);
}
bool timerPending()
{
  SbTime deadline;
  return SoDB::getSensorManager()->isTimerSensorPending(deadline) != FALSE;
}
// Expose the protected trigger-time setter to make timer dispatch deterministic.
class TestTimerSensor : public SoTimerSensor {
public:
  TestTimerSensor(SoSensorCB * callback, void * data) : SoTimerSensor(callback, data) {}
  using SoTimerQueueSensor::setTriggerTime;
};
void fail(void * data, SoSensor *)
{
  ++*static_cast<int *>(data);
  throw std::runtime_error("injected callback exception");
}
void rescheduleSelf(void * data, SoSensor * sensor)
{
  ++*static_cast<int *>(data);
  sensor->schedule();
}
void cancelAndFail(void * data, SoSensor * sensor)
{
  sensor->unschedule();
  fail(data, sensor);
}
void failChanged(void *)
{
  throw std::runtime_error("injected changed callback exception");
}
void changed(void * data)
{
  ++*static_cast<int *>(data);
}
void scheduleAnotherTimer(void * data, SoSensor *)
{
  static_cast<SoAlarmSensor *>(data)->schedule();
}
void drainDelay(void *)
{
  SoDB::getSensorManager()->processDelayQueue(TRUE);
}
void drainTimer(void *)
{
  SoDB::getSensorManager()->processTimerQueue();
}
struct ChangeState {
  SoSensor * sensor;
  bool observed;
};
void rescheduleChanged(void * data)
{
  ChangeState * state = static_cast<ChangeState *>(data);
  SoDB::getSensorManager()->setChangedCallback(NULL, NULL);
  state->observed = state->sensor->isScheduled() == FALSE;
  state->sensor->schedule();
}
void run(const std::string & name)
{
  SoSensorManager * manager = SoDB::getSensorManager();
  int calls = 0;
  if (name == "alarm_cancel") {
    SoAlarmSensor alarm(count, &calls);
    alarm.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    alarm.schedule();
    alarm.schedule();
    alarm.unschedule();
    const bool pending = timerPending();
    // Drain while the alarm is alive, even when testing an unfixed library.
    manager->processTimerQueue();
    check(!pending, "cancel left a duplicate alarm in the queue");
    check(calls == 0, "canceled alarm still triggered");
  }
  else if (name == "recurring_cancel") {
    SoTimerSensor timer(count, &calls);
    timer.setInterval(SbTime(60.0));
    timer.schedule();
    timer.schedule();
    timer.unschedule();
    const bool pending = timerPending();
    // A second unschedule is not a recovery mechanism: the flag is false.
    check(!pending, "cancel left a duplicate recurring timer in the queue");
  }
  else if (name == "alarm_reschedule") {
    SoAlarmSensor alarm(count, &calls);
    alarm.setTimeFromNow(SbTime(60.0));
    alarm.schedule();
    alarm.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    alarm.schedule();
    manager->processTimerQueue();
    check(calls == 1, "rescheduled alarm must trigger exactly once");
    check(!timerPending(), "rescheduled alarm left another entry");
  }
  else if (name == "timeout_change_cancel") {
    manager->setDelaySensorTimeout(SbTime(60.0));
    SoOneShotSensor sensor(count, &calls);
    sensor.schedule();
    manager->setDelaySensorTimeout(SbTime(30.0));
    manager->setDelaySensorTimeout(SbTime::zero());
    const bool pending = timerPending();
    sensor.unschedule();
    check(!pending, "disabled timeout left an alarm in the queue");
    check(calls == 0, "changing timeout invoked the delay callback");
  }
  else if (name == "zero_repeat" || name == "zero_nonidle") {
    SoIdleSensor sensor(count, &calls);
    sensor.schedule();
    check(!timerPending(), "zero timeout armed an alarm on insertion");
    if (name == "zero_repeat") manager->setDelaySensorTimeout(SbTime::zero());
    else manager->processDelayQueue(FALSE);
    const bool pending = timerPending();
    const bool scheduled = sensor.isScheduled() != FALSE;
    manager->processDelayQueue(TRUE);
    check(!pending, "zero timeout was rearmed");
    check(scheduled, "idle sensor was lost during timeout handling");
    check(calls == 1, "idle sensor must run once during the idle pass");
  }
  else if (name == "nonzero_timeout") {
    manager->setDelaySensorTimeout(SbTime(60.0));
    SoOneShotSensor sensor(count, &calls);
    sensor.schedule();
    check(timerPending(), "nonzero timeout did not arm an alarm");
    const SbTime before = SbTime::getTimeOfDay();
    manager->setDelaySensorTimeout(SbTime(30.0));
    const SbTime after = SbTime::getTimeOfDay();
    SbTime deadline;
    check(manager->isTimerSensorPending(deadline) != FALSE,
          "changing nonzero timeout removed its alarm");
    check(deadline >= before + SbTime(30.0) &&
          deadline <= after + SbTime(30.0), "timeout deadline was not updated");
    manager->processDelayQueue(TRUE);
    check(calls == 1, "normal delay processing stopped working");
    manager->setDelaySensorTimeout(SbTime::zero());
    check(!timerPending(), "timeout cancellation failed after delay processing");
  }
  else if (name == "node_detach") {
    SoSeparator * node = new SoSeparator;
    node->ref();
    {
      SoNodeSensor sensor(count, &calls);
      sensor.attach(node);
      node->touch();
      check(sensor.isScheduled(), "node touch did not schedule sensor");
      sensor.detach();
      check(!sensor.isScheduled(), "detach left sensor scheduled");
      manager->processDelayQueue(TRUE);
      check(calls == 0, "detached node sensor invoked callback");
      sensor.attach(node);
      node->touch();
      manager->processDelayQueue(TRUE);
      check(calls == 1, "reattached sensor stopped working");
    }
    node->unref();
  }
  else if (name == "delay_reentry" || name == "delay_timeout_reentry") {
    if (name == "delay_timeout_reentry") manager->setDelaySensorTimeout(SbTime(60.0));
    SoOneShotSensor sensor(count, &calls);
    manager->setChangedCallback(drainDelay, NULL);
    sensor.schedule();
    manager->setChangedCallback(NULL, NULL);
    check(calls == 1, "reentrant callback did not drain delay queue");
    check(!sensor.isScheduled(), "reentrant processing left phantom scheduled flag");
    check(!manager->isDelaySensorPending(), "reentrant processing left delay entry");
    manager->setDelaySensorTimeout(SbTime::zero());
  }
  else if (name == "timer_reentry") {
    SoAlarmSensor sensor(count, &calls);
    sensor.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    manager->setChangedCallback(drainTimer, NULL);
    sensor.schedule();
    manager->setChangedCallback(NULL, NULL);
    check(calls == 1, "reentrant callback did not drain timer queue");
    check(!sensor.isScheduled() && !timerPending(), "reentrant timer state inconsistent");
  }
  else if (name == "delay_changed_exception" || name == "timer_changed_exception") {
    SoOneShotSensor delay(count, &calls);
    SoAlarmSensor timer(count, &calls);
    timer.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    SoSensor * sensor = name == "delay_changed_exception" ?
      static_cast<SoSensor *>(&delay) : static_cast<SoSensor *>(&timer);
    manager->setChangedCallback(failChanged, NULL);
    bool caught = false;
    try { sensor->schedule(); }
    catch (const std::runtime_error &) { caught = true; }
    manager->setChangedCallback(NULL, NULL);
    check(caught, "changed callback exception was swallowed");
    check(sensor->isScheduled(), "queued sensor lost its scheduled flag");
    sensor->unschedule();
    manager->processDelayQueue(TRUE);
    manager->processTimerQueue();
    check(calls == 0, "canceled sensor triggered after changed exception");
  }
  else if (name == "delay_remove_reentry" || name == "timer_remove_reentry") {
    SoOneShotSensor delay(count, &calls);
    SoAlarmSensor timer(count, &calls);
    timer.setTimeFromNow(SbTime(60.0));
    SoSensor * sensor = name == "delay_remove_reentry" ?
      static_cast<SoSensor *>(&delay) : static_cast<SoSensor *>(&timer);
    sensor->schedule();
    ChangeState state = {sensor, false};
    manager->setChangedCallback(rescheduleChanged, &state);
    sensor->unschedule();
    manager->setChangedCallback(NULL, NULL);
    check(state.observed, "removal callback observed stale scheduled flag");
    check(sensor->isScheduled(), "removal callback could not reschedule sensor");
    sensor->unschedule();
    check(!manager->isDelaySensorPending() && !timerPending(), "rescheduled sensor could not be canceled");
  }
  else if (name == "timer_exception") {
    TestTimerSensor timer(fail, &calls);
    timer.setInterval(SbTime(60.0));
    timer.schedule();
    timer.setTriggerTime(SbTime::getTimeOfDay() - SbTime(1.0));
    bool caught = false;
    try { manager->processTimerQueue(); }
    catch (const std::runtime_error &) { caught = true; }
    check(caught && calls == 1, "timer callback exception was not propagated");
    check(timer.isScheduled() && timerPending(), "exception stranded recurring timer");
    timer.setFunction(count);
    timer.setTriggerTime(SbTime::getTimeOfDay() - SbTime(1.0));
    manager->processTimerQueue();
    check(calls == 2 && timerPending(), "recurring timer did not recover after exception");
    timer.unschedule();
    check(!timerPending(), "recovered recurring timer could not be canceled");
  }
  else if (name == "timer_cancel_exception") {
    TestTimerSensor timer(cancelAndFail, &calls);
    timer.setInterval(SbTime(60.0));
    timer.schedule();
    timer.setTriggerTime(SbTime::getTimeOfDay() - SbTime(1.0));
    bool caught = false;
    try { manager->processTimerQueue(); }
    catch (const std::runtime_error &) { caught = true; }
    check(caught && calls == 1, "canceling timer exception was not propagated");
    check(!timer.isScheduled() && !timerPending(), "exception resurrected canceled timer");
  }
  else if (name == "delay_deferred_exception") {
    int selfcalls = 0;
    SoOneShotSensor self(rescheduleSelf, &selfcalls);
    self.setPriority(50);
    SoOneShotSensor throwing(fail, &calls);
    throwing.setPriority(100);
    self.schedule();
    throwing.schedule();
    bool caught = false;
    try { manager->processDelayQueue(TRUE); }
    catch (const std::runtime_error &) { caught = true; }
    check(caught && calls == 1 && selfcalls == 1, "deferred sensor pass did not stop at exception");
    check(self.isScheduled() && manager->isDelaySensorPending(), "exception stranded deferred sensor");
    self.setFunction(count);
    manager->processDelayQueue(TRUE);
    check(selfcalls == 2 && !self.isScheduled(), "deferred sensor did not recover after exception");
  }
  else if (name == "delay_exception") {
    int idlecalls = 0;
    SoIdleSensor idle(count, &idlecalls);
    idle.setPriority(50);
    SoOneShotSensor throwing(fail, &calls);
    throwing.setPriority(100);
    idle.schedule();
    throwing.schedule();
    bool caught = false;
    try { manager->processDelayQueue(FALSE); }
    catch (const std::runtime_error &) { caught = true; }
    check(caught && calls == 1, "delay callback exception was not propagated");
    check(idle.isScheduled() && manager->isDelaySensorPending(), "exception stranded idle sensor");
    manager->processDelayQueue(TRUE);
    check(idlecalls == 1 && !idle.isScheduled(), "idle sensor did not recover after exception");
  }
  else if (name == "absolute_deadline") {
    SbTime deadline(123.0);
    check(!manager->isTimerSensorPending(deadline) && deadline == SbTime(123.0),
          "empty timer query changed output time");
    SoAlarmSensor alarm(count, &calls);
    const SbTime expected = SbTime::getTimeOfDay() + SbTime(60.0);
    alarm.setTime(expected);
    alarm.schedule();
    check(manager->isTimerSensorPending(deadline) && deadline == expected,
          "timer query did not return absolute trigger time");
    SbTime wait = deadline - SbTime::getTimeOfDay();
    check(wait > SbTime::zero() && wait <= SbTime(60.0), "invalid relative wait");
    alarm.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    alarm.schedule();
    check(manager->isTimerSensorPending(deadline), "overdue alarm missing");
    wait = deadline - SbTime::getTimeOfDay();
    if (wait < SbTime::zero()) wait = SbTime::zero();
    check(wait == SbTime::zero(), "overdue wait was not clamped to zero");
    manager->processTimerQueue();
    check(calls == 1 && !timerPending(), "overdue alarm did not dispatch");
  }
  else if (name == "changed_during_dispatch") {
    int notices = 0;
    SoAlarmSensor next(count, &calls);
    next.setTimeFromNow(SbTime(60.0));
    SoAlarmSensor first(scheduleAnotherTimer, &next);
    first.setTime(SbTime::getTimeOfDay() - SbTime(1.0));
    manager->setChangedCallback(changed, &notices);
    first.schedule();
    check(notices == 1, "ordinary insertion did not notify integration");
    notices = 0;
    manager->processTimerQueue();
    manager->setChangedCallback(NULL, NULL);
    check(notices == 0, "queue-processing notification behavior changed");
    SbTime deadline;
    check(manager->isTimerSensorPending(deadline) && deadline == next.getTriggerTime(),
          "post-dispatch query did not find newly scheduled timer");
    next.unschedule();
  }
  else if (name == "immediate_notification") {
    int notices = 0;
    SoOneShotSensor sensor(count, &calls);
    sensor.setPriority(0);
    manager->setChangedCallback(changed, &notices);
    sensor.schedule();
    check(notices == 0 && manager->isDelaySensorPending(),
          "immediate insertion notification contract changed");
    manager->processImmediateQueue();
    manager->setChangedCallback(NULL, NULL);
    check(calls == 1 && !sensor.isScheduled() && !manager->isDelaySensorPending(),
          "immediate processing did not drain pending work");
  }
  else throw std::runtime_error("unknown test case");
}
}

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  SoDB::init();
  SoDB::enableRealTimeSensor(FALSE);
  SoDB::setDelaySensorTimeout(SbTime::zero());
  try {
    run(argv[1]);
    SoDB::finish();
    std::cout << argv[1] << ": passed\n";
    return 0;
  }
  catch (const std::exception & error) {
    // Exit without dispatching remaining queues from a failed regression.
    std::cerr << argv[1] << ": " << error.what() << '\n';
    return 1;
  }
}
