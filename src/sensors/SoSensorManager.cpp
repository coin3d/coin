/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSensorManager SoSensorManager.h Inventor/sensors/SoSensorManager.h
  \brief The SoSensorManager class handles the sensor queues.
  \ingroup sensors

  There are two major sensor types in Coin: delay-sensors and
  timer-sensors. Each of these two types has its own queue, which is
  handled by the SoSensorManager. The queues are kept in sorted order by
  SoSensorManager, either according to trigger-time (for timer-sensors)
  or by priority (for delay-sensors).

  Delay-sensors trigger when the application is otherwise idle. In
  addition, to avoid starvation in applications that are continually
  busy, the delay-sensor queue also has a timeout which, when reached,
  will empty the queue anyhow.

  Timer-sensors are set up to trigger at specific, abolute times.


  FIXME: doc

  ..provides methods for inserting, removing, processing (emptying)
  queues of sensors..

  ..the sensor mechanism is crucial in Coin for (automatic redrawal upon
  changes,)..

  ..should usually be considered as an internal class in the Coin
  system, only interesting for "normal users" when implementing new
  windowsystem-specific libraries (like SoQt,) ... which usually goes
  like this: (register change callback -- drop in delay queue sensor
  of type XXX for redraw queueing -- call process*Queue when... --
  ...) ... confer the SoQt sourcecode to see a complete example of use
  (list interesting files)...

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
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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

#ifndef DOXYGEN_SKIP_THIS

class SoSensorManagerP {
public:

  SbBool processingtimerqueue, processingdelayqueue;

  SbList <SoDelayQueueSensor *> delayqueue, delaywaitqueue;
  SbList <SoTimerQueueSensor *> timerqueue, timerwaitqueue;
  SbList <SoTimerSensor*> reschedulelist;

  void (*queueChangedCB)(void *);
  void * queueChangedCBData;

  SbTime delaysensortimeout;
};

#endif // DOXYGEN_SKIP_THIS 

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
  assert(newentry);

  SbList<SoDelayQueueSensor *> * activedelayqueue;

  if (THIS->processingdelayqueue) activedelayqueue = & THIS->delaywaitqueue;
  else activedelayqueue = & THIS->delayqueue;

  int pos = 0;
  while((pos < activedelayqueue->getLength()) &&
        // Internally sorted in backward order for more effective list
        // processing during triggering.
        ((SoSensor *)newentry)->isBefore((SoSensor *)((*activedelayqueue)[pos])))
    pos++;
  activedelayqueue->insert(newentry, pos);

#if DEBUG_DELAY_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::insertDelaySensor",
                           "inserted delay sensor #%d -- %p -- "
                           "%sprocessing queue",
                           THIS->delayqueue.getLength() +
                           THIS->delaywaitqueue.getLength() - 1,
                           newentry,
                           THIS->processingdelayqueue ? "" : "not ");
#endif // debug

  if (!THIS->processingdelayqueue) {
    this->notifyChanged();
  }
}

/*!
  Add a new entry to the timer queue of sensors. The queue will be sorted in
  order of supposed trigger time.

  \sa removeFromQueue()
 */
void
SoSensorManager::insertTimerSensor(SoTimerQueueSensor * newentry)
{
  assert(newentry);

  SbList<SoTimerQueueSensor *> * activequeue;

  if (THIS->processingtimerqueue)
    activequeue = & THIS->timerwaitqueue;
  else
    activequeue = & THIS->timerqueue;

  int i=0;
  while (i < activequeue->getLength() &&
         ((SoSensor *)(*activequeue)[i])->isBefore((SoSensor *)newentry))
    i++;
  activequeue->insert(newentry, i);

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
  Remove an entry from the sensor queue.

  \sa addToQueue()
 */
void
SoSensorManager::removeDelaySensor(SoDelayQueueSensor * entry)
{
  // Check "real" queue first..
  int idx = THIS->delayqueue.find(entry);
  if (idx != -1) THIS->delayqueue.remove(idx);

  // ..then the wait queue.
  if (idx == -1) {
    idx = THIS->delaywaitqueue.find(entry);
    if (idx != -1) THIS->delaywaitqueue.remove(idx);
  }

  if (!THIS->processingdelayqueue && idx != -1) this->notifyChanged();

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoSensorManager::removeDelaySensor",
                              "trying to remove element not in list");
  }
#endif // COIN_DEBUG
}

/*!
  \overload
 */
void
SoSensorManager::removeTimerSensor(SoTimerQueueSensor * entry)
{
  // Check "real" queue first..
  int idx = THIS->timerqueue.find(entry);
  if (idx != -1) THIS->timerqueue.remove(idx);

  // ..then the wait queue.
  if (idx == -1) {
    idx = THIS->timerwaitqueue.find(entry);
    if (idx != -1) THIS->timerwaitqueue.remove(idx);
  }

  if (!THIS->processingtimerqueue && idx != -1) this->notifyChanged();

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoSensorManager::removeTimerSensor",
                              "trying to remove element not in list");
  }
#endif // COIN_DEBUG
}

/*!
  Trigger all the timers which has expired.
 */
void
SoSensorManager::processTimerQueue(void)
{
  if (THIS->processingtimerqueue || THIS->timerqueue.getLength() == 0)
    return;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "start: %d elements", THIS->timerqueue.getLength());
#endif // debug

  assert(THIS->reschedulelist.getLength() == 0);
  THIS->processingtimerqueue = TRUE;

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
    sensor->trigger();
  }

  THIS->processingtimerqueue = FALSE;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, before merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug

  this->mergeTimerQueues();

  int n = THIS->reschedulelist.getLength();
  if (n) {
    SbTime time = SbTime::getTimeOfDay();
    for (int i = 0; i < n; i++) {
      THIS->reschedulelist[i]->reschedule(time);
    }
    THIS->reschedulelist.truncate(0);
  }

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, after merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug

  // Should I do this?. 19990207 mortene.
//    this->notifyChanged();
}

/*!
  Trigger all delay queue entries in priority order.

  The \a isidle flag indicates whether or not the processing happens
  because the application is idle or because the delay queue timeout
  was reached.

  \sa SoDB::setDelaySensorTimeout()
 */
void
SoSensorManager::processDelayQueue(SbBool isidle)
{
  if (THIS->processingdelayqueue || THIS->delayqueue.getLength() == 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "start: %d elements", THIS->delayqueue.getLength());
#endif // debug

  THIS->processingdelayqueue = TRUE;

  // Sensors with higher priorities are triggered first.
  int len = THIS->delayqueue.getLength();
  while (len) {
#if DEBUG_DELAY_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                           "treat element with pri %d",
                           THIS->delayqueue[len-1]->getPriority());
#endif // debug

    SoDelayQueueSensor * sensor = THIS->delayqueue[len-1];
    THIS->delayqueue.removeFast(len-1);

    if (!isidle && sensor->isIdleOnly()) {
      // Will automatically put sensor in "processing wait-queue", and
      // merge back into the "real" queue after processing has
      // completed and queue has been emptied.
      this->insertDelaySensor(sensor);
    }
    else {
      ((SoSensor*)sensor)->trigger();
    }
    len = THIS->delayqueue.getLength();
  }

  THIS->processingdelayqueue = FALSE;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "end, before merge: %d elements",
                         THIS->delayqueue.getLength());
#endif // debug

  this->mergeDelayQueues();

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "end, after merge: %d elements",
                         THIS->delayqueue.getLength());
#endif // debug

  // Should I do this?. 19990207 mortene.
//    this->notifyChanged();
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::processImmediateQueue(void)
{
  if (THIS->processingdelayqueue || THIS->delayqueue.getLength() == 0 ||
      THIS->delayqueue[THIS->delayqueue.getLength()-1]->getPriority() != 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "start: %d elements in full delay queue",
                         THIS->delayqueue.getLength());
#endif // debug

  THIS->processingdelayqueue = TRUE;

  while (THIS->delayqueue.getLength() &&
         THIS->delayqueue[THIS->delayqueue.getLength()-1]->getPriority() == 0)
    {
#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
      SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                             "trigger element");
#endif // debug
      SoSensor * sensor = THIS->delayqueue.pop();
      sensor->trigger();
    }

  THIS->processingdelayqueue = FALSE;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "end, before merge: %d elements in full delay queue",
                         THIS->delayqueue.getLength());
#endif // debug

  this->mergeDelayQueues();

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "end, after merge: %d elements in full delay queue",
                         THIS->delayqueue.getLength());
#endif // debug

  // Should I do this? 19990207 mortene.
//    this->notifyChanged();
}

/*!
  \internal

  Merges the wait queue (which is the "incoming" queue during queue
  processing) with the real queue after the processing has been done.

  Returns number of items moved from wait queue.
*/
int
SoSensorManager::mergeTimerQueues(void)
{
  int i=0, merged=THIS->timerwaitqueue.getLength();
  while (THIS->timerwaitqueue.getLength() > 0) {
    SoTimerQueueSensor * ts = THIS->timerwaitqueue[0];
    THIS->timerwaitqueue.remove(0);
    SbTime triggertime = ts->getTriggerTime();

    while (i < THIS->timerqueue.getLength() &&
           THIS->timerqueue[i]->getTriggerTime() < triggertime) i++;
    THIS->timerqueue.insert(ts, i);
  }
  return merged;
}

/*!
  \internal

  Merges the wait queue (which is the "incoming" queue during queue
  processing) with the real queue after the processing has been done.

  Returns number of items moved from wait queue.
*/
int
SoSensorManager::mergeDelayQueues(void)
{
  int i=0;
  int merged=THIS->delaywaitqueue.getLength();
  while (THIS->delaywaitqueue.getLength() > 0) {
    SoDelayQueueSensor * ds = THIS->delaywaitqueue[0];
    THIS->delaywaitqueue.remove(0);
    unsigned int pri = ds->getPriority();

    while (i < THIS->delayqueue.getLength() &&
           pri > THIS->delayqueue[i]->getPriority()) i++;
    THIS->delayqueue.insert(ds, i);
  }

  return merged;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::rescheduleTimer(SoTimerSensor * s)
{
  THIS->reschedulelist.append(s);
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::removeRescheduledTimer(SoTimerQueueSensor * s)
{
  int idx = THIS->reschedulelist.find((SoTimerSensor*)s);
  if (idx >= 0) {
    THIS->reschedulelist.remove(idx);
  }
  else {
    this->removeTimerSensor(s);
  }
}

/*!
  FIXME: write doc
*/
SbBool
SoSensorManager::isDelaySensorPending(void)
{
  return (THIS->delayqueue.getLength() > 0) ? TRUE : FALSE;
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
  if (THIS->timerqueue.getLength() > 0) {
    tm = THIS->timerqueue[0]->getTriggerTime();
    return TRUE;
  }

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
  return THIS->delaysensortimeout;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::setChangedCallback(void (*func)(void *), void * data)
{
  THIS->queueChangedCB = func;
  THIS->queueChangedCBData = data;
}

void
SoSensorManager::notifyChanged(void)
{
  if(THIS->queueChangedCB) THIS->queueChangedCB(THIS->queueChangedCBData);
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

#undef THIS
