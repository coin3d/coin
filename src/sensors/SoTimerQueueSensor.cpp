/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoTimerQueueSensor SoTimerQueueSensor.h Inventor/sensors/SoTimerQueueSensor.h
  \brief The SoTimerQueueSensor class is the abstract base class for
  sensors trigging upon the occurence of certain timer events.
  \ingroup sensors

  TODO: doc
 */

#include <Inventor/sensors/SoTimerQueueSensor.h>
#include <Inventor/SoDB.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SbBool SoTimerQueueSensor::scheduled
  \a TRUE if the sensor is currently scheduled.
*/


/*!
  Constructor.
 */
SoTimerQueueSensor::SoTimerQueueSensor(void)
  : scheduled(FALSE)
{
}

/*!
  Constructor taking as parameters the sensor callback function and the
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
  // NB: it won't work to move this to the SoSensor destructor (where
  // it really belongs), because you can't use pure virtual methods
  // from a destructor.
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
