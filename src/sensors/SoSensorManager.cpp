/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoSensorManager SoSensorManager.h Inventor/sensors/SoSensorManager.h
  \brief The SoSensorManager class handles the sensor queues.
  \ingroup sensors

  There are two major sensor types in Coin, "delay" sensors and
  "timer" sensors:

  \li Delay sensors trigger when the application is otherwise idle. In
      addition, to avoid starvation in applications that are
      continually busy, the delay-sensor queue also has a timeout
      which, when reached, will empty the queue anyhow.

  \li Timer sensors are set up to trigger at specific, absolute times.

  Each of these two types has its own queue, which is handled by the
  SoSensorManager. The queues are kept in sorted order by
  SoSensorManager, either according to trigger-time (for
  timer sensors) or by priority (for delay sensors).

  The SoSensorManager provides methods for managing these queues, by
  insertion and removal of sensors, and processing (emptying) of the
  queues.

  The sensor mechanism is crucial in Coin for a number of important
  features, most notably automatic scheduling redrawal upon changes,
  and for making it possible to set up animations in the scenegraph
  which does \e not need any additional book-keeping from the
  application code.

  SoSensorManager should usually be considered as an internal class in
  the Coin API. It is only interesting for application programmers
  when \e implementing new windowsystem-specific libraries (like
  Systems in Motion's SoQt, SoXt, SoGtk or SoWin) or wrappers. Then
  one has to set up code to empty the queues at the correct
  intervals. For detailed information on how to do that, we would
  advise you to look at the implementation of said mechanisms in the
  So*-libraries which SIM provides.

  \sa SoSensor SoTimerQueueSensor SoDelayQueueSensor
  \sa SoTimerSensor SoAlarmSensor
  \sa SoIdleSensor SoDataSensor SoOneShotSensor
  \sa SoPathSensor SoFieldSensor SoNodeSensor
*/


#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>
#include <Inventor/SbDict.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/errors/SoDebugError.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_THREADS
#include <Inventor/threads/SbMutex.h>
#endif // HAVE_THREADS

#include <assert.h>

// FIXME: find fd_set definition properly through something configure
// based.  19991214 mortene.  (Note: fd_set is in time.h under AIX?)
#if HAVE_UNISTD_H
#include <unistd.h> // fd_set (?)
#endif // HAVE_UNISTD_H


// Keep these around. Even though the SoSensorManager code seems to be
// working as it should now, a lot of other stuff around in the Coin
// library depends on getting the "local" sensor handling correct, and
// enabling the debuginfo in this class can help immensely. --mortene.
#define DEBUG_DELAY_SENSORHANDLING 0
#define DEBUG_TIMER_SENSORHANDLING 0

// This can be any "magic" bitpattern of 32 bits which seems unlikely
// to be randomly assigned to a memory word upon destruction.
//
// The 32 bits allocated for the "alive" bitpattern is used to try to
// detect when the instance has been prematurely destructed. This
// should prove useful to catch errors related to when SoSensorManager
// is destructed (on exit) while there are still live SoSensor-derived
// instances in the system, which then subsequently tries to
// unschedule themselves.
//
// <mortene@sim.no>
#define ALIVE_PATTERN 0x600dc0de /* spells "goodcode" */


class SoSensorManagerP {
public:
  SoSensorManagerP(void) : alive(ALIVE_PATTERN) { }
  ~SoSensorManagerP() { this->alive = 0xdeadbeef; /* set to whatever != ALIVE_PATTERN */ }

  SbBool processingtimerqueue, processingdelayqueue;
  SbBool processingimmediatequeue;

  // immediatequeue - stores SoDelayQueueSensors with priority 0. FIFO.
  // delayqueue   - stores SoDelayQueueSensor's in sorted order.
  // timerqueue - stores SoTimerSensors in sorted order.

  SbList <SoDelayQueueSensor *> immediatequeue;
  SbList <SoDelayQueueSensor *> delayqueue;
  SbList <SoTimerQueueSensor *> timerqueue;
  SbList <SoTimerSensor*> reschedulelist;

  // triggerdict - stores sensors that has been triggered in processDelayQueue().
  // reinsertdict - temporary storage for idle sensors during processing
  SbDict triggerdict;
  SbDict reinsertdict;

  void (*queueChangedCB)(void *);
  void * queueChangedCBData;

  SbTime delaysensortimeout;

  uint32_t alive;
  static void assertAlive(SoSensorManagerP * that);

#ifdef HAVE_THREADS
  SbMutex timermutex;
  SbMutex delaymutex;
  SbMutex immediatemutex;
  SbMutex reschedulemutex;
#endif // HAVE_THREADS
};

void
SoSensorManagerP::assertAlive(SoSensorManagerP * that)
{
  if (that->alive != ALIVE_PATTERN) {
    SoDebugError::post("SoSensorManagerP::assertAlive",
                       "Detected an attempt to access SoSensorManager "
                       "instance after it was destructed!  "
                       "This is most likely to be the result of some grave "
                       "programming error in the internal library code. "
                       "Please report this problem");
    assert(FALSE && "SoSensorManager-object no longer alive!");
  }
}

#ifdef HAVE_THREADS

#define LOCK_TIMER_QUEUE(_mgr_) \
  _mgr_->pimpl->timermutex.lock();

#define UNLOCK_TIMER_QUEUE(_mgr_) \
  _mgr_->pimpl->timermutex.unlock();

#define LOCK_DELAY_QUEUE(_mgr_) \
  _mgr_->pimpl->delaymutex.lock();

#define UNLOCK_DELAY_QUEUE(_mgr_) \
  _mgr_->pimpl->delaymutex.unlock();

#define LOCK_IMMEDIATE_QUEUE(_mgr_) \
  _mgr_->pimpl->immediatemutex.lock();

#define UNLOCK_IMMEDIATE_QUEUE(_mgr_) \
  _mgr_->pimpl->immediatemutex.unlock();

#define LOCK_RESCHEDULE_LIST(_mgr_) \
  _mgr_->pimpl->immediatemutex.lock();

#define UNLOCK_RESCHEDULE_LIST(_mgr_) \
  _mgr_->pimpl->immediatemutex.unlock();

#else // HAVE_THREADS

#define LOCK_TIMER_QUEUE(_mgr_)
#define UNLOCK_TIMER_QUEUE(_mgr_)
#define LOCK_DELAY_QUEUE(_mgr_)
#define UNLOCK_DELAY_QUEUE(_mgr_)
#define LOCK_IMMEDIATE_QUEUE(_mgr_)
#define UNLOCK_IMMEDIATE_QUEUE(_mgr_)
#define LOCK_RESCHEDULE_LIST(_mgr_)
#define UNLOCK_RESCHEDULE_LIST(_mgr_)

#endif // ! HAVE_THREADS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
 */
SoSensorManager::SoSensorManager(void)
{
  THIS = new SoSensorManagerP;

  THIS->queueChangedCB = NULL;
  THIS->queueChangedCBData = NULL;

  THIS->processingtimerqueue = FALSE;
  THIS->processingdelayqueue = FALSE;
  THIS->processingimmediatequeue = FALSE;

  THIS->delaysensortimeout.setValue(1.0/12.0);
}

/*!
  Destructor.
 */
SoSensorManager::~SoSensorManager()
{
  // FIXME: remove entries. 19990225 mortene.
  if(THIS->delayqueue.getLength() != 0) {}
  if(THIS->timerqueue.getLength() != 0) {}

  delete THIS;
}

/*!
  Add a new entry to the queue of delay sensors.

  \sa removeFromQueue()
 */
void
SoSensorManager::insertDelaySensor(SoDelayQueueSensor * newentry)
{
  SoSensorManagerP::assertAlive(THIS);
  assert(newentry);

  // immediate sensors are stored in a separate list. We don't need to
  // sort them based on SoSensor::isBefore(), but just use a FIFO
  // strategy.
  if (newentry->getPriority() == 0) {
    LOCK_IMMEDIATE_QUEUE(this);
    THIS->immediatequeue.append(newentry);
    UNLOCK_IMMEDIATE_QUEUE(this);
  }
  else {
    LOCK_DELAY_QUEUE(this);
    SbList <SoDelayQueueSensor *> & delayqueue = THIS->delayqueue;

    int pos = 0;
    while((pos < delayqueue.getLength()) &&
          ((SoSensor*)delayqueue[pos])->isBefore(newentry)) {
      pos++;
    }
    delayqueue.insert(newentry, pos);
    UNLOCK_DELAY_QUEUE(this);
    this->notifyChanged();
  }

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::insertDelaySensor",
                         "inserted delay sensor #%d -- %p -- "
                         "%sprocessing queue",
                         THIS->delayqueue.getLength() +
                         THIS->delaywaitqueue.getLength() - 1,
                         newentry,
                         THIS->processingdelayqueue ? "" : "not ");
#endif // debug
}

/*!
  Add a new entry to the timer queue of sensors. The queue will be sorted in
  order of supposed trigger time.

  \sa removeFromQueue()
 */
void
SoSensorManager::insertTimerSensor(SoTimerQueueSensor * newentry)
{
  SoSensorManagerP::assertAlive(THIS);
  assert(newentry);

  SbList <SoTimerQueueSensor *> & timerqueue = THIS->timerqueue;

  LOCK_TIMER_QUEUE(this);
  int i = 0;
  while (i < timerqueue.getLength() &&
         ((SoSensor*)timerqueue[i])->isBefore(newentry)) {
    i++;
  }
  timerqueue.insert(newentry, i);

  UNLOCK_TIMER_QUEUE(this);

#if DEBUG_TIMER_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::insertTimerSensor",
                         "inserted timer sensor #%d -- %p "
                         "(triggertime %f) -- "
                         "%sprocessing queue",
                         THIS->timerqueue.getLength() +
                         THIS->timerwaitqueue.getLength() - 1,
                         newentry, newentry->getTriggerTime().getValue(),
                         THIS->processingtimerqueue ? "" : "not ");
#endif // debug

  if (!THIS->processingtimerqueue) {
    this->notifyChanged();
  }
}

/*!
  Remove an entry from the queue of prioritized sensors.

  \sa addToQueue()
 */
void
SoSensorManager::removeDelaySensor(SoDelayQueueSensor * entry)
{
  SoSensorManagerP::assertAlive(THIS);

  LOCK_DELAY_QUEUE(this);
  // Check "real" queue first..
  int idx = THIS->delayqueue.find(entry);
  if (idx != -1) THIS->delayqueue.remove(idx);
  UNLOCK_DELAY_QUEUE(this);

  // ..then the immediate queue.
  if (idx == -1) {
    LOCK_IMMEDIATE_QUEUE(this);
    idx = THIS->immediatequeue.find(entry);
    if (idx != -1) THIS->immediatequeue.remove(idx);
    UNLOCK_IMMEDIATE_QUEUE(this);
  }
  // ..then the reinsert list
  if (idx == -1) {
    if (THIS->reinsertdict.remove((unsigned long) entry)) {
      idx = 0; // make sure notifyChanged() is called.
    }
  }

  if (idx != -1) this->notifyChanged();

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoSensorManager::removeDelaySensor",
                              "trying to remove element not in list");
  }
#endif // COIN_DEBUG
}

/*!
  Remove an entry from the queue of timer sensors.
 */
void
SoSensorManager::removeTimerSensor(SoTimerQueueSensor * entry)
{
  SoSensorManagerP::assertAlive(THIS);

  LOCK_TIMER_QUEUE(this);
  int idx = THIS->timerqueue.find(entry);
  if (idx != -1) {
    THIS->timerqueue.remove(idx);
    UNLOCK_TIMER_QUEUE(this);
    this->notifyChanged();
  }
  else {
    UNLOCK_TIMER_QUEUE(this);
#if COIN_DEBUG
    SoDebugError::postWarning("SoSensorManager::removeTimerSensor",
                              "trying to remove element not in list");
#endif // COIN_DEBUG
  }
}

/*!
  Trigger all the timers which has expired.
 */
void
SoSensorManager::processTimerQueue(void)
{
  SoSensorManagerP::assertAlive(THIS);

  if (THIS->processingtimerqueue || THIS->timerqueue.getLength() == 0)
    return;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "start: %d elements", THIS->timerqueue.getLength());
#endif // debug

  assert(THIS->reschedulelist.getLength() == 0);
  THIS->processingtimerqueue = TRUE;

  LOCK_TIMER_QUEUE(this);

  SbTime currenttime = SbTime::getTimeOfDay();
  while (THIS->timerqueue.getLength() > 0 &&
         THIS->timerqueue[0]->getTriggerTime() <= currenttime) {
#if DEBUG_TIMER_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                           "process element with triggertime %s",
                           THIS->timerqueue[0]->getTriggerTime().format().getString());
#endif // debug
    SoSensor * sensor = THIS->timerqueue[0];
    THIS->timerqueue.remove(0);
    UNLOCK_TIMER_QUEUE(this);
    sensor->trigger();
    LOCK_TIMER_QUEUE(this);
  }

  UNLOCK_TIMER_QUEUE(this);

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, before merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug

  LOCK_RESCHEDULE_LIST(this);
  int n = THIS->reschedulelist.getLength();
  if (n) {
    SbTime time = SbTime::getTimeOfDay();
    for (int i = 0; i < n; i++) {
      THIS->reschedulelist[i]->reschedule(time);
    }
    THIS->reschedulelist.truncate(0);
  }
  UNLOCK_RESCHEDULE_LIST(this);

  THIS->processingtimerqueue = FALSE;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, after merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug
}

//
// callback from reinsertdict which will reinsert the sensor
//
static void
reinsert_dict_cb(unsigned long, void * sensor, void * sensormanager)
{
  SoSensorManager * thisp = (SoSensorManager*) sensormanager;
  thisp->insertDelaySensor((SoDelayQueueSensor*) sensor);
}

/*!
  Trigger all delay queue entries in priority order.

  The \a isidle flag indicates whether or not the processing happens
  because the application is idle or because the delay queue timeout
  was reached.

  A delay queue sensor with priority > 0 can only be triggered once
  during a call to this function. If a delay sensor is rescheduled
  during processDelayQueue(), it is not processed until the next time
  this function is called. This is done to avoid an infinite loop
  while processing the sensors.

  A delay queue sensor with priority 0 is called an immediate sensor.

  \sa SoDB::setDelaySensorTimeout()
  \sa SoSensorManager::processImmediateQueue()
*/
void
SoSensorManager::processDelayQueue(SbBool isidle)
{
  SoSensorManagerP::assertAlive(THIS);

  this->processImmediateQueue();

  if (THIS->processingdelayqueue || THIS->delayqueue.getLength() == 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "start: %d elements", THIS->delayqueue.getLength());
#endif // debug

  THIS->processingdelayqueue = TRUE;

  // triggerdict is used to store sensors that has already been
  // triggered. A sensor should only be triggered once during a call
  // to processDelayQueue(), otherwise we might risk never returning
  // from this function. E.g. SoSceneManager::scheduleRedraw()
  // triggers a delay sensor, which again triggers a redraw. During
  // the redraw, SoSceneManager::scheduleRedraw() might be called
  // again, etc...
  THIS->triggerdict.clear();

  LOCK_DELAY_QUEUE(this);

  // Sensors with higher priorities are triggered first.
  while (THIS->delayqueue.getLength()) {
#if DEBUG_DELAY_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                           "treat element with pri %d",
                           THIS->delayqueue[0]->getPriority());
#endif // debug

    SoDelayQueueSensor * sensor = THIS->delayqueue[0];
    THIS->delayqueue.remove(0);
    UNLOCK_DELAY_QUEUE(this);

    if (!isidle && sensor->isIdleOnly()) {
      // move sensor to another temporary list. It will be reinserted
      // at the end of this function. We do this to be able to always
      // remove the first list element. We avoid searching for the
      // first non-idle sensor.
      (void) THIS->reinsertdict.enter((unsigned long) sensor, (void*) sensor);
    }
    else {
      // only trigger sensor once per processing loop
      if (THIS->triggerdict.enter((unsigned long) sensor, (void*) sensor)) {
        sensor->trigger();
      }
      else {
        // Reuse the "reinsert" list to store the sensor. It will be
        // reinserted at the end of this function.
        (void) THIS->reinsertdict.enter((unsigned long) sensor, (void*) sensor);
      }
    }
    LOCK_DELAY_QUEUE(this);
  }
  UNLOCK_DELAY_QUEUE(this);

  // reinsert sensors that couldn't be triggered, either because it
  // was an idle sensor, or because the sensor had already been
  // triggered
  THIS->reinsertdict.applyToAll(reinsert_dict_cb, (void*) this);
  THIS->reinsertdict.clear();
  THIS->processingdelayqueue = FALSE;
}

/*!
  Process all immediate sensors (delay sensors with priority 0).

  Be aware that you might risk an infinite loop using immediate
  sensors. Unlike delay queue sensors, immediate sensors can be
  rescheduled and triggered multiple times during immediate queue
  processing.

  \sa SoDelayQueueSensor::setPriority() */
void
SoSensorManager::processImmediateQueue(void)
{
  SoSensorManagerP::assertAlive(THIS);

  if (THIS->processingimmediatequeue) return;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "start: %d elements in full immediate queue",
                         THIS->immediatequeue.getLength());
#endif // debug

  THIS->processingimmediatequeue = TRUE;

  // FIXME: implement some better logic to break out of the
  // processing loop. Right now we break out if more than 10000
  // immediate sensors are processed. pederb, 2002-01-30
  int triggercnt = 0;

  LOCK_IMMEDIATE_QUEUE(this);

  while (THIS->immediatequeue.getLength()) {
#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
    SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                           "trigger element");
#endif // debug
    SoSensor * sensor = THIS->immediatequeue[0];
    THIS->immediatequeue.remove(0);
    UNLOCK_IMMEDIATE_QUEUE(this);

    sensor->trigger();

    LOCK_IMMEDIATE_QUEUE(this);
    triggercnt++;
    if (triggercnt > 10000) break;
  }
  if (THIS->immediatequeue.getLength()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoSensorManager::processImmediateQueue",
                              "Infinite loop detected. Breaking out.");
#endif // COIN_DEBUG
  }
  UNLOCK_IMMEDIATE_QUEUE(this);

  THIS->processingimmediatequeue = FALSE;
}

/*!
  \COININTERNAL
*/
void
SoSensorManager::rescheduleTimer(SoTimerSensor * s)
{
  SoSensorManagerP::assertAlive(THIS);

  LOCK_RESCHEDULE_LIST(this);
  THIS->reschedulelist.append(s);
  UNLOCK_RESCHEDULE_LIST(this);
}

/*!
  \COININTERNAL
*/
void
SoSensorManager::removeRescheduledTimer(SoTimerQueueSensor * s)
{
  SoSensorManagerP::assertAlive(THIS);

  LOCK_RESCHEDULE_LIST(this);
  int idx = THIS->reschedulelist.find((SoTimerSensor*)s);
  if (idx >= 0) {
    THIS->reschedulelist.remove(idx);
    UNLOCK_RESCHEDULE_LIST(this);
  }
  else {
    UNLOCK_RESCHEDULE_LIST(this);
    this->removeTimerSensor(s);
  }
}

/*!
  Returns \c TRUE if at least one delay sensor or immediate sensor is
  present in the respective queue, otherwise \c FALSE.
*/
SbBool
SoSensorManager::isDelaySensorPending(void)
{
  SoSensorManagerP::assertAlive(THIS);

  return (THIS->delayqueue.getLength() ||
          THIS->immediatequeue.getLength()) ? TRUE : FALSE;
}

/*!
  Returns \c TRUE if at least one timer sensor is present in the
  queue, otherwise \c FALSE.

  If sensors are pending, the time interval until the next one should
  be triggered will be put in the \a tm variable.
*/
SbBool
SoSensorManager::isTimerSensorPending(SbTime & tm)
{
  SoSensorManagerP::assertAlive(THIS);

  LOCK_TIMER_QUEUE(this);
  if (THIS->timerqueue.getLength() > 0) {
    tm = THIS->timerqueue[0]->getTriggerTime();
    UNLOCK_TIMER_QUEUE(this);
    return TRUE;
  }

  UNLOCK_TIMER_QUEUE(this);
  return FALSE;
}

/*!
  Delay sensors are usually triggered only when the system is
  idle. But when there are continuous updates to the scene graph,
  there's a possibility that the delay queue will starve and sensors
  are never triggered. To make sure this won't happen, this is a
  timeout value for the delay queue. When this timeout has been reached,
  the sensors in the delay queue gets processed before other sensors and
  events. This method will let the user set this timeout value.

  The default value is 1/12 of a second.

  \sa getDelaySensorTimeout(), SoDelayQueueSensor
*/
void
SoSensorManager::setDelaySensorTimeout(const SbTime & t)
{
  SoSensorManagerP::assertAlive(THIS);

#if COIN_DEBUG
  if(t < SbTime(0.0)) {
    SoDebugError::postWarning("SoDB::setDelaySensorTimeout",
                              "Tried to set negative interval.");
    return;
  }
#endif // COIN_DEBUG

  THIS->delaysensortimeout = t;
}

/*!
  Returns the timeout value for sensors in the delay queue.

  \sa setDelaySensorTimeout(), SoDelayQueueSensor
 */
const SbTime &
SoSensorManager::getDelaySensorTimeout(void)
{
  SoSensorManagerP::assertAlive(THIS);

  return THIS->delaysensortimeout;
}

/*!
  For setting up a callback function to be invoked whenever any of the
  sensor queues are changed.

  This callback should typically be responsible for updating the
  client-side mechanism which is used for processing the queues.
*/
void
SoSensorManager::setChangedCallback(void (*func)(void *), void * data)
{
  SoSensorManagerP::assertAlive(THIS);

  THIS->queueChangedCB = func;
  THIS->queueChangedCBData = data;
}

void
SoSensorManager::notifyChanged(void)
{
  SoSensorManagerP::assertAlive(THIS);

  if (THIS->queueChangedCB &&
      !THIS->processingtimerqueue &&
      !THIS->processingdelayqueue &&
      !THIS->processingimmediatequeue) {
    THIS->queueChangedCB(THIS->queueChangedCBData);
  }
}

/*!
  This is a wrapper around the standard select(2) call, which will
  make sure the sensor queues are updated while waiting for any action
  to happen on the given file descriptors.

  The void* arguments must be valid pointers to fd_set
  structures. We've changed this from the original SGI Inventor API to
  avoid messing up the header file with system-specific includes.
*/
int
SoSensorManager::doSelect(int nfds, void * readfds, void * writefds,
                          void * exceptfds, struct timeval * usertimeout)
{
  // FIXME: implement. See SoDB::doSelect() (which should probably only
  // be a wrapper around this call). 19990425 mortene.
  COIN_STUB();
  return 0;
}

int
SoSensorManager::mergeTimerQueues(void)
{
  assert(0 && "obsoleted");
  return 0;
}

int
SoSensorManager::mergeDelayQueues(void)
{
  assert(0 && "obsoleted");
  return 0;
}


#undef THIS
