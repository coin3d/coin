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
