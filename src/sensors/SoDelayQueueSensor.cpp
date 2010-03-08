/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoDelayQueueSensor SoDelayQueueSensor.h Inventor/sensors/SoDelayQueueSensor.h
  \brief The SoDelayQueueSensor class is the abstract base class for priority scheduled sensors.
  \ingroup sensors

  Delay queue sensors are invoked upon various events \e not related
  to time occurrences. See documentation of subclasses to see which
  types of events can be surveilled by the builtin sensor types.

  The priority values can be used to queue events by their importance,
  so the sensors are triggered in the sequence you want.

  The queue of delay sensors (i.e. instances of subclasses of
  SoDelayQueueSensor) will be processed as soon as either the run-time
  system is idle, or if it is continually busy they will be processed
  within a fixed amount of time.

  This time interval is by default 1/12th of a second, but can be
  controlled with the SoSensorManager::setDelaySensorTimeout()
  interface.
*/

#include <assert.h>

#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <Inventor/SoDB.h>
#include <Inventor/errors/SoDebugError.h>


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
  Set this sensor's priority in the scheduling queue. When sensors
  are processed, high priority sensors will trigger before low
  priority sensors. 

  Please note that a low number means a high priority. A sensor with
  priority 5 will trigger before a sensor with priority 6.

  Sensors with priority 0 have a special meaning in Coin/Inventor.
  They are called immediate sensors, and will trigger immediately
  after the current notification chain has finished. Priority 0
  sensors should be used with care, since they might lead to bad
  performance if you do some processing in the sensor callback.

  This setting does not affect SoDataSensor delete-callback
  invocations, which always are immediate.

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
  Returns the default scheduling priority value. The default
  sensor priority is 100.

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
