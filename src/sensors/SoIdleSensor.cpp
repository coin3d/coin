/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoIdleSensor SoIdleSensor.h Inventor/sensors/SoIdleSensor.h
  \brief The SoIdleSensor class is a sensor which will trigger as soon as the application is idle.
  \ingroup sensors

  An SoIdleSensor differs from an SoOneShotSensor in that it will not
  trigger if the delay queue processing is occurring due to the delay
  queue timeout, but \e only when the application is idle.

  \sa SoDB::setDelaySensorTimeout()
*/

#include <Inventor/sensors/SoIdleSensor.h>
#include <assert.h>

/*!
  Default constructor.
 */
SoIdleSensor::SoIdleSensor(void)
{
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoIdleSensor::SoIdleSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
}

/*!
  Destructor.
*/
SoIdleSensor::~SoIdleSensor(void)
{
}

// Only processed when application is idle -- not at delay queue
// timeouts.
SbBool
SoIdleSensor::isIdleOnly(void) const
{
  return TRUE;
}
