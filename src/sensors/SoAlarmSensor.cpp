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
  \class SoAlarmSensor SoAlarmSensor.h Inventor/sensors/SoAlarmSensor.h
  \brief The SoAlarmSensor class is a sensor which will trigger once at a specified time.
  \ingroup sensors

  SoAlarmSensor provides a convenient way of setting up triggers for
  jobs which should be executed only once when they are scheduled.
 */

#include <Inventor/sensors/SoAlarmSensor.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Default constructor.
 */
SoAlarmSensor::SoAlarmSensor(void)
{
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoAlarmSensor::SoAlarmSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
}

/*!
  Destructor.
*/
SoAlarmSensor::~SoAlarmSensor(void)
{
}

/*!
  Set the time at which the sensor will trigger.

  Note that you must manually schedule() the sensor after calling this
  method.

  \sa setTimeFromNow(), getTime()
 */
void
SoAlarmSensor::setTime(const SbTime & abstime)
{
  this->alarm = abstime;
}

/*!
  Set the alarm to be at a specified offset from the current time.

  Note that you must manually schedule() the sensor after calling this
  method.

  \sa setTime(), getTime()
 */
void
SoAlarmSensor::setTimeFromNow(const SbTime & reltime)
{
  this->alarm.setToTimeOfDay();
  this->alarm += reltime;
}

/*!
  Returns the trigger time for the alarm as an absolute value from
  1970-01-01 00:00:00.

  \sa setTime(), setTimeFromNow()
 */
const SbTime &
SoAlarmSensor::getTime(void) const
{
  return this->alarm;
}

// Doc from superclass.
void
SoAlarmSensor::schedule(void)
{
  this->setTriggerTime(this->alarm);
  inherited::schedule();
}
