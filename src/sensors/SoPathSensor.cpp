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
  \class SoPathSensor SoPathSensor.h Inventor/sensors/SoPathSensor.h
  \brief The SoPathSensor class detects changes to paths.
  \ingroup sensors

  If you need to know when a path changes (i.e. nodes in the path has
  been removed, or new nodes is added), use this sensor to get a
  notification.
*/

#include <Inventor/sensors/SoPathSensor.h>
#include <Inventor/SoPath.h>


/*!
  Default constructor. Use setFunction() to set up a callback function
  later.
*/
SoPathSensor::SoPathSensor(void)
{
  this->convict = NULL;
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoPathSensor::SoPathSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->convict = NULL;
}

/*!
  Destructor.
*/
SoPathSensor::~SoPathSensor(void)
{
  if (this->convict) this->detach();
}

/*!
  Attach sensor to a path. Whenever the path changes, the sensor will
  be triggered and call the callback function.

  \sa detach()
 */
void
SoPathSensor::attach(SoPath * path)
{
  this->convict = path;
  path->addAuditor(this, SoNotRec::SENSOR);
}

/*!
  Detach sensor from path. As long as an SoPathSensor is detached, it
  will never invoke its callback function.

  \sa attach()
 */
void
SoPathSensor::detach(void)
{
  if (this->convict) this->convict->removeAuditor(this, SoNotRec::SENSOR);
  this->convict = NULL;
}

/*!
  Returns a pointer to the path connected to the sensor.

  \sa attach(), detach()
 */
SoPath *
SoPathSensor::getAttachedPath(void) const
{
  return this->convict;
}

// Doc from superclass.
void
SoPathSensor::notify(SoNotList * l)
{
  inherited::notify(l);

  // FIXME: I don't know what the heck we're supposed to do here, but
  // I included the overriding in case we find out after 1.0 release,
  // and need to fix the behavior without changing the class signature
  // (which would break binary compatibility). 20000402 mortene.
}

// Doc from superclass.
void
SoPathSensor::dyingReference(void)
{
  this->invokeDeleteCallback();
  this->detach();
}
