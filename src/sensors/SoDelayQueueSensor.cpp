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
  \class SoDelayQueueSensor SoDelayQueueSensor.h Inventor/sensors/SoDelayQueueSensor.h
  \brief The SoDelayQueueSensor class is the abstract base class for priority scheduled sensors.
  \ingroup sensors

  Delay queue sensors are invoked upon various events \e not related
  to time occurs. See documentation of subclasses to see which types
  of events can be surveilled by the builtin sensor types.

  The priority values can be used to queue events by their importance,
  so the sensors are triggered in the sequence you want.
*/

#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <Inventor/SoDB.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SbBool SoDelayQueueSensor::scheduled
  \c TRUE if the sensor is currently scheduled.
*/

/*!
  Default constructor.
 */
SoDelayQueueSensor::SoDelayQueueSensor(void)
{
  this->scheduled = FALSE;
  this->priority = SoDelayQueueSensor::getDefaultPriority();
}

/*!
  Constructor taking as arguments the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoDelayQueueSensor::SoDelayQueueSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->scheduled = FALSE;
  this->priority = SoDelayQueueSensor::getDefaultPriority();
}

/*!
  Destructor.
*/
SoDelayQueueSensor::~SoDelayQueueSensor(void)
{
  // NB: it won't work to move this to the SoSensor destructor (where
  // it really belongs), because you can't use pure virtual methods
  // from a destructor.
  if (this->isScheduled()) this->unschedule();
}

/*!
  Set this sensor's priority in the scheduling queue.

  \sa getPriority(), getDefaultPriority()
 */
void
SoDelayQueueSensor::setPriority(uint32_t pri)
{
  this->priority = pri;

  if (this->isScheduled()) {
    // Immediately trigger if scheduled and priority is set to zero.
    if (pri == 0) this->trigger();
    // FIXME: reschedule if new priority is != old priority?
    // 20000401 mortene.
  }
}

/*!
  Returns sensor scheduling priority.

  \sa setPriority(), getDefaultPriority()
 */
uint32_t
SoDelayQueueSensor::getPriority(void) const
{
  return this->priority;
}

/*!
  Returns the default scheduling priority value.

  \sa getPriority()
 */
uint32_t
SoDelayQueueSensor::getDefaultPriority(void)
{
  return 100;
}

/*!
  Put the sensor in the global delay queue. This means it will be
  triggered either when the CPU is idle, or when the specified delay
  queue time-out is reached.

  \sa SoDB::setDelaySensorTimeout(), unschedule(), isScheduled()
 */
void
SoDelayQueueSensor::schedule(void)
{
  // FIXME: is this correct, or should we perhaps re-schedule?
  // 19990226 mortene.
  if (this->isScheduled()) return;

  SoDB::getSensorManager()->insertDelaySensor(this);
  this->scheduled = TRUE;
}

/*!
  Remove sensor from the delay queue, without triggering it first.

  \sa schedule(), isScheduled()
 */
void
SoDelayQueueSensor::unschedule(void)
{
#if COIN_DEBUG
  if (!this->isScheduled()) {
    SoDebugError::postWarning("SoDelayQueueSensor::unschedule",
                              "tried to unschedule a sensor which is "
                              "not scheduled");
    return;
  }
#endif // COIN_DEBUG

  SoDB::getSensorManager()->removeDelaySensor(this);
  this->scheduled = FALSE;
}

/*!
  Check if this sensor is scheduled for triggering.

  \sa schedule(), unschedule()
 */
SbBool
SoDelayQueueSensor::isScheduled(void) const
{
  return this->scheduled;
}

/*!
  Returns a flag indicating whether or not the sensor should only be
  triggered if the application is truly idle, and \e not when the
  delay queue is processed because of the delay queue timeout.

  \sa SoDB::setDelaySensorTimeout()
 */
SbBool
SoDelayQueueSensor::isIdleOnly(void) const
{
  return FALSE;
}

SbBool
SoDelayQueueSensor::isBefore(const SoSensor * s) const
{
  return (this->priority < ((SoDelayQueueSensor *)s)->priority);
}
