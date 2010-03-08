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
  \class SoOneShotSensor SoOneShotSensor.h Inventor/sensors/SoOneShotSensor.h
  \brief The SoOneShotSensor class is a sensor which will trigger once.
  \ingroup sensors

  Since SoOneShotSensor is a subclass of SoDelayQueueSensor, it will
  trigger as soon as either the run-time system is idle, or if it is
  continually busy it will trigger within a fixed amount of time (this
  is by default 1/12th of a second, see
  SoSensorManager::setDelaySensorTimeout()).
*/

#include <Inventor/sensors/SoOneShotSensor.h>
#include <assert.h>

/*!
  Constructor.
 */
SoOneShotSensor::SoOneShotSensor(void)
{
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoOneShotSensor::SoOneShotSensor(SoSensorCB *func, void *data)
  : inherited(func, data)
{
}

/*!
  Destructor.
*/
SoOneShotSensor::~SoOneShotSensor(void)
{
}
