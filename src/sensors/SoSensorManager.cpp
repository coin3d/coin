/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoSensorManager SoSensorManager.h Inventor/sensors/SoSensorManager.h
  \brief The SoSensorManager class handles the sensor queues.
  \ingroup sensors

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
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>


// Keep these around. Even though the SoSensorManager code seems to be
// working as it should now, a lot of other stuff around in the Coin
// library depends on getting the "local" sensor handling correct, and
// enabling the debuginfo in this class can help immensely. --mortene.
#define DEBUG_DELAY_SENSORHANDLING 0
#define DEBUG_TIMER_SENSORHANDLING 0


/*!
  Constructor.
 */
SoSensorManager::SoSensorManager(void)
{
  this->queueChangedCB = NULL;
  this->queueChangedCBData = NULL;

  this->processingtimerqueue = FALSE;
  this->processingdelayqueue = FALSE;

  this->delaysensortimeout.setValue(1.0/12.0);
}

/*!
  Destructor.
 */
SoSensorManager::~SoSensorManager()
{
  // FIXME: remove entries. 19990225 mortene.
  if(this->delayqueue.getLength() != 0) {}
  if(this->timerqueue.getLength() != 0) {}
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

  if (this->processingdelayqueue) activedelayqueue = & this->delaywaitqueue;
  else activedelayqueue = & this->delayqueue;

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
                           this->delayqueue.getLength() +
                           this->delaywaitqueue.getLength() - 1,
                           newentry,
                           this->processingdelayqueue ? "" : "not ");
#endif // debug

  if (!this->processingdelayqueue) {
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

  if (this->processingtimerqueue)
    activequeue = & this->timerwaitqueue;
  else
    activequeue = & this->timerqueue;

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
                           this->timerqueue.getLength() +
                           this->timerwaitqueue.getLength() - 1,
                           newentry, newentry->getTriggerTime().getValue(),
                           this->processingtimerqueue ? "" : "not ");
#endif // debug

  if (!this->processingtimerqueue) {
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
  int idx = this->delayqueue.find(entry);
  if (idx != -1) this->delayqueue.remove(idx);

  // ..then the wait queue.
  if (idx == -1) {
    idx = this->delaywaitqueue.find(entry);
    if (idx != -1) this->delaywaitqueue.remove(idx);
  }

  if (!this->processingdelayqueue && idx != -1) this->notifyChanged();

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
  int idx = this->timerqueue.find(entry);
  if (idx != -1) this->timerqueue.remove(idx);

  // ..then the wait queue.
  if (idx == -1) {
    idx = this->timerwaitqueue.find(entry);
    if (idx != -1) this->timerwaitqueue.remove(idx);
  }

  if (!this->processingtimerqueue && idx != -1) this->notifyChanged();

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
  if (this->processingtimerqueue || this->timerqueue.getLength() == 0)
    return;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "start: %d elements", this->timerqueue.getLength());
#endif // debug

  this->processingtimerqueue = TRUE;

  SbTime currenttime = SbTime::getTimeOfDay();
  while (this->timerqueue.getLength() > 0 &&
         this->timerqueue[0]->getTriggerTime() <= currenttime) {
#if DEBUG_TIMER_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                           "process element with triggertime %s",
                           this->timerqueue[0]->getTriggerTime().format().getString());
#endif // debug
    this->timerqueue[0]->trigger();
  }

  this->processingtimerqueue = FALSE;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, before merge: %d elements",
                         this->timerqueue.getLength());
#endif // debug

  this->mergeTimerQueues();

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, after merge: %d elements",
                         this->timerqueue.getLength());
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
  if (this->processingdelayqueue || this->delayqueue.getLength() == 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "start: %d elements", this->delayqueue.getLength());
#endif // debug

  this->processingdelayqueue = TRUE;

  // Sensors with higher priorities are triggered first.
  while (this->delayqueue.getLength()) {
#if DEBUG_DELAY_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                           "treat element with pri %d",
                           this->delayqueue[i]->getPriority());
#endif // debug

    if (!isidle && this->delayqueue[0]->isIdleOnly()) {
      // Remove from current queue without using unschedule().
      SoDelayQueueSensor * tmpptr = this->delayqueue[0];
      this->delayqueue.remove(0);

      // Will automatically put sensor in "processing wait-queue", and
      // merge back into the "real" queue after processing has
      // completed and queue has been emptied.
      this->insertDelaySensor(tmpptr);
    }
    else {
      this->delayqueue[0]->trigger();
    }
  }

  this->processingdelayqueue = FALSE;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "end, before merge: %d elements",
                         this->delayqueue.getLength());
#endif // debug

  this->mergeDelayQueues();

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "end, after merge: %d elements",
                         this->delayqueue.getLength());
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
  if (this->processingdelayqueue || this->delayqueue.getLength() == 0 ||
      this->delayqueue[this->delayqueue.getLength()-1]->getPriority() != 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "start: %d elements in full delay queue",
                         this->delayqueue.getLength());
#endif // debug

  this->processingdelayqueue = TRUE;

  while (this->delayqueue.getLength() &&
         this->delayqueue[this->delayqueue.getLength()-1]->getPriority() == 0)
    {
#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
      SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                             "trigger element");
#endif // debug
      this->delayqueue[this->delayqueue.getLength()-1]->trigger();
    }

  this->processingdelayqueue = FALSE;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "end, before merge: %d elements in full delay queue",
                         this->delayqueue.getLength());
#endif // debug

  this->mergeDelayQueues();

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "end, after merge: %d elements in full delay queue",
                         this->delayqueue.getLength());
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
  int i=0, merged=this->timerwaitqueue.getLength();
  while (this->timerwaitqueue.getLength() > 0) {
    SoTimerQueueSensor * ts = this->timerwaitqueue[0];
    this->timerwaitqueue.remove(0);
    SbTime triggertime = ts->getTriggerTime();

    while (i < this->timerqueue.getLength() &&
           this->timerqueue[i]->getTriggerTime() < triggertime) i++;
    this->timerqueue.insert(ts, i);
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
  int merged=this->delaywaitqueue.getLength();
  while (this->delaywaitqueue.getLength() > 0) {
    SoDelayQueueSensor * ds = this->delaywaitqueue[0];
    this->delaywaitqueue.remove(0);
    unsigned int pri = ds->getPriority();

    while (i < this->delayqueue.getLength() &&
           pri > this->delayqueue[i]->getPriority()) i++;
    this->delayqueue.insert(ds, i);
  }

  return merged;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::rescheduleTimer(SoTimerSensor * s)
{
  this->removeTimerSensor(s);
  s->reschedule(SbTime::getTimeOfDay());
  this->insertTimerSensor(s);
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::removeRescheduledTimer(SoTimerQueueSensor * s)
{
  this->removeTimerSensor(s);
}

/*!
  FIXME: write doc
*/
SbBool
SoSensorManager::isDelaySensorPending(void)
{
  return (this->delayqueue.getLength() > 0) ? TRUE : FALSE;
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
  if (this->timerqueue.getLength() > 0) {
    tm = this->timerqueue[0]->getTriggerTime();
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

  this->delaysensortimeout = t;
}

/*!
  Returns the timeout value for sensors in the delay queue.

  \sa setDelaySensorTimeout(), SoDelayQueueSensor
 */
const SbTime &
SoSensorManager::getDelaySensorTimeout(void)
{
  return this->delaysensortimeout;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::setChangedCallback(void (*func)(void *), void * data)
{
  this->queueChangedCB = func;
  this->queueChangedCBData = data;
}

void
SoSensorManager::notifyChanged(void)
{
  if(this->queueChangedCB) this->queueChangedCB(this->queueChangedCBData);
}

/*!
  This is a wrapper around the standard select(2) call, which will
  make sure the sensor queues are updated while waiting for any action
  to happen on the given file descriptors.
 */
int
SoSensorManager::doSelect(int /* nfds */, fd_set * /* readfds */, fd_set * /* writefds */,
                          fd_set * /* exceptfds */, struct timeval * /* userTimeOut */)
{
  // FIXME: implement. See SoDB::doSelect() (which should probably only
  // be a wrapper around this call). 19990425 mortene.
  COIN_STUB();
  return 0;
}
