/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
