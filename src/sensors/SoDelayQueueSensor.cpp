/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
  if (this->priority != pri) {
    this->priority = pri;
    if (this->isScheduled()) {
      this->unschedule();
      this->schedule();
    }
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

// Documented in superclass.
void
SoDelayQueueSensor::trigger(void)
{
  // Overridden to clear scheduled flag before triggering.

  this->scheduled = FALSE;
  inherited::trigger();
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
  if (!this->scheduled) {
    SoDB::getSensorManager()->insertDelaySensor(this);
    this->scheduled = TRUE;
  }
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
