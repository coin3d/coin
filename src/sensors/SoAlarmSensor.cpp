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
  \class SoAlarmSensor SoAlarmSensor.h Inventor/sensors/SoAlarmSensor.h
  \brief The SoAlarmSensor class is a sensor which will trigger once at
  a specified time.
  \ingroup sensors

  TODO: doc
 */

#include <Inventor/sensors/SoAlarmSensor.h>
#include <assert.h>

/*!
  Constructor.
 */
SoAlarmSensor::SoAlarmSensor(void)
{
}

/*!
  Constructor taking as parameters the sensor callback function and the
  userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoAlarmSensor::SoAlarmSensor(SoSensorCB *func, void *data)
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
  Set the time at which the sensor will trigger. You must also
  schedule() the sensor manually after calling this method.

  \sa setTimeFromNow(), getTime()
 */
void
SoAlarmSensor::setTime(const SbTime & absTime)
{
  this->alarm = absTime;
}

/*!
  Set the alarm to be at a specified offset from the current time. You
  must also schedule() the sensor manually after calling this method.

  \sa setTime(), getTime()
 */
void
SoAlarmSensor::setTimeFromNow(const SbTime & relTime)
{
  this->alarm.setToTimeOfDay();
  this->alarm += relTime;
}

/*!
  Returns the alarm time.

  \sa setTime(), setTimeFromNow()
 */
const SbTime &
SoAlarmSensor::getTime(void) const
{
  return this->alarm;
}
