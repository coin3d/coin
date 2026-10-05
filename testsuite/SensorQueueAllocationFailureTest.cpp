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

#include <Inventor/SoDB.h>
#include <Inventor/SbTime.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <cstdlib>
#include <cstring>
#include <new>

static bool failnextarray = false;

void * operator new[](std::size_t size)
{
  if (failnextarray) {
    failnextarray = false;
    throw std::bad_alloc();
  }
  void * p = std::malloc(size ? size : 1);
  if (!p) throw std::bad_alloc();
  return p;
}

void operator delete[](void * p) noexcept { std::free(p); }

static void count(void * data, SoSensor *)
{
  ++*static_cast<int *>(data);
}

class TestTimerSensor : public SoTimerSensor {
public:
  TestTimerSensor(SoSensorCB * cb, void * data) : SoTimerSensor(cb, data) { }
  using SoTimerQueueSensor::setTriggerTime;
};

static int testRecurring(SoSensorManager * manager)
{
  int calls = 0;
  TestTimerSensor * timers[5];
  const SbTime due = SbTime::getTimeOfDay() - SbTime(1.0);
  for (int i = 0; i < 5; ++i) {
    timers[i] = new TestTimerSensor(count, &calls);
    timers[i]->setInterval(SbTime(60.0));
    timers[i]->schedule();
    timers[i]->setTriggerTime(due);
  }

  failnextarray = true;
  bool failed = false;
  try { manager->processTimerQueue(); }
  catch (const std::bad_alloc &) { failed = true; }
  const bool consumed = !failnextarray;
  failnextarray = false;
  if (!failed || !consumed || calls != 4 || timers[4]->isScheduled()) return 1;
  for (int i = 0; i < 4; ++i) {
    if (!timers[i]->isScheduled()) return 2;
  }
  timers[4]->schedule();
  if (!timers[4]->isScheduled()) return 3;
  timers[4]->setTriggerTime(due);
  manager->processTimerQueue();
  if (calls != 5) return 4;
  for (int i = 0; i < 5; ++i) delete timers[i];
  return 0;
}

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  const bool timer = std::strcmp(argv[1], "timer") == 0;
  const bool recurring = std::strcmp(argv[1], "recurring") == 0;
  const bool immediate = std::strcmp(argv[1], "immediate") == 0;
  if (!timer && !immediate && !recurring &&
      std::strcmp(argv[1], "delay") != 0) return 2;

  SoDB::init();
  SoDB::enableRealTimeSensor(FALSE);
  SoDB::setDelaySensorTimeout(SbTime::zero());
  SoSensorManager * manager = SoDB::getSensorManager();
  if (recurring) {
    const int result = testRecurring(manager);
    if (result == 0) SoDB::finish();
    return result;
  }
  int calls = 0;
  SoOneShotSensor * delays[5];
  SoAlarmSensor * alarms[5];
  for (int i = 0; i < 5; ++i) {
    delays[i] = new SoOneShotSensor(count, &calls);
    alarms[i] = new SoAlarmSensor(count, &calls);
    if (immediate) delays[i]->setPriority(0);
    alarms[i]->setTimeFromNow(SbTime(60.0));
  }

  for (int i = 0; i < 4; ++i) {
    if (timer) alarms[i]->schedule();
    else delays[i]->schedule();
  }

  failnextarray = true;
  bool failed = false;
  try {
    if (timer) alarms[4]->schedule();
    else delays[4]->schedule();
  }
  catch (const std::bad_alloc &) { failed = true; }
  const bool consumed = !failnextarray;
  failnextarray = false;
  if (!failed || !consumed ||
      (timer ? alarms[4]->isScheduled() : delays[4]->isScheduled())) return 1;

  // A failed insertion must leave the queue unlocked and allow a retry.
  if (timer) alarms[4]->schedule();
  else delays[4]->schedule();
  if (!(timer ? alarms[4]->isScheduled() : delays[4]->isScheduled())) return 3;

  if (timer) {
    for (int i = 0; i < 5; ++i) {
      alarms[i]->unschedule();
      alarms[i]->setTime(SbTime::getTimeOfDay() - SbTime(1.0));
      alarms[i]->schedule();
    }
    manager->processTimerQueue();
  }
  else if (immediate) manager->processImmediateQueue();
  else manager->processDelayQueue(TRUE);
  if (calls != 5) return 4;

  for (int i = 0; i < 5; ++i) {
    delete alarms[i];
    delete delays[i];
  }
  SoDB::finish();
  return 0;
}
