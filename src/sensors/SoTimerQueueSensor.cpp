/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoTimerQueueSensor SoTimerQueueSensor.h Inventor/sensors/SoTimerQueueSensor.h
  \brief The SoTimerQueueSensor class is the abstract base class for sensors triggering on certain timer events.
  \ingroup sensors

  Timer sensors triggers upon specific points in time. If a timer
  sensor can not trigger at the exact moment it has been scheduled
  (due to other activity in Coin, a task suspended or heavy load from
  other applications on the system), it will be triggered at the first
  opportunity after the scheduled time has passed.

  See the documentation of the subclasses for information on what ways
  there are to specify times, intervals, etc.
*/

#include <Inventor/sensors/SoTimerQueueSensor.h>
#include <Inventor/SoDB.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SbBool SoTimerQueueSensor::scheduled
  \c TRUE if the sensor is currently scheduled.
*/


/*!
  Default constructor.
 */
SoTimerQueueSensor::SoTimerQueueSensor(void)
  : scheduled(FALSE)
{
}

/*!
  Constructor taking as arguments the sensor callback function and the
  userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoTimerQueueSensor::SoTimerQueueSensor(SoSensorCB * func, void * data)
  : inherited(func, data), scheduled(FALSE)
{
}

/*!
  Destructor.
*/
SoTimerQueueSensor::~SoTimerQueueSensor(void)
{
  // Note: it won't work to move this to the SoSensor destructor
  // (where it really belongs), because you can't use pure virtual
  // methods from a destructor.
  if (this->isScheduled()) this->unschedule();
}

/*!
  Returns the time at which the sensor will trigger.

  \sa setTriggerTime()
 */
const SbTime &
SoTimerQueueSensor::getTriggerTime(void) const
{
  return this->triggertime;
}

/*!
  Set absolute time at which to trigger sensor.

  \sa getTriggerTime()
 */
void
SoTimerQueueSensor::setTriggerTime(const SbTime & time)
{
  this->triggertime = time;
  // FIXME: reschedule? 19990425 mortene.
}

/*!
  Overloaded to clear scheduled flag before triggering.
*/
void 
SoTimerQueueSensor::trigger(void)
{
  this->scheduled = FALSE;
  inherited::trigger();
}

/*!
  Put the sensor in the global timer queue.

  \sa unschedule(), isScheduled()
 */
void
SoTimerQueueSensor::schedule(void)
{
#if COIN_DEBUG
  assert(this->scheduled == TRUE || this->scheduled == FALSE);
  if (this->isScheduled()) {
    SoDebugError::postWarning("SoTimerQueueSensor::schedule",
                              "already scheduled!");
    return;
  }
#endif // COIN_DEBUG

  SoDB::getSensorManager()->insertTimerSensor(this);
  this->scheduled = TRUE;
}

/*!
  Remove sensor from the timer queue, without triggering it first.

  \sa schedule(), isScheduled()
 */
void
SoTimerQueueSensor::unschedule(void)
{
#if COIN_DEBUG
  assert(this->scheduled == TRUE || this->scheduled == FALSE);
  if (!this->isScheduled()) {
    SoDebugError::postWarning("SoTimerQueueSensor::unschedule",
                              "not scheduled!");
    return;
  }
#endif // COIN_DEBUG
  SoDB::getSensorManager()->removeTimerSensor(this);
  this->scheduled = FALSE;
}

/*!
  Check if this sensor is scheduled for triggering.

  \sa schedule(), unschedule()
 */
SbBool
SoTimerQueueSensor::isScheduled(void) const
{
  return this->scheduled;
}

SbBool
SoTimerQueueSensor::isBefore(const SoSensor * s) const
{
  return (this->triggertime < ((SoTimerQueueSensor *)s)->triggertime);
}
