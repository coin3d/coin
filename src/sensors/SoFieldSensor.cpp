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
  \class SoFieldSensor SoFieldSensor.h Inventor/sensors/SoFieldSensor.h
  \brief The SoFieldSensor class detects changes to a field.
  \ingroup sensors

  Attach a field to a sensor of this type to put it under
  surveillance, so you can act upon changes to the field.
*/

#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoField.h>


/*!
  Constructor.
 */
SoFieldSensor::SoFieldSensor(void)
{
  this->convict = NULL;
}

/*!
  Constructor taking as parameters the sensor callback function and
  the userdata which will be passed the callback.

  \sa setFunction(), setData()
 */
SoFieldSensor::SoFieldSensor(SoSensorCB * func, void * data)
  : inherited(func, data)
{
  this->convict = NULL;
}

/*!
  Destructor.
*/
SoFieldSensor::~SoFieldSensor(void)
{
  if (this->convict) this->detach();
}

/*!
  Attach sensor to a field. Whenever the field's value changes, the
  sensor will be triggered and call the callback function.

  \sa detach()
 */
void
SoFieldSensor::attach(SoField * field)
{
  this->convict = field;
  field->addAuditor(this, SoNotRec::SENSOR);
}

/*!
  Detach sensor from field. As long as an SoFieldSensor is detached,
  it will never call its callback function.

  \sa attach()
 */
void
SoFieldSensor::detach(void)
{
  if (this->convict) this->convict->removeAuditor(this, SoNotRec::SENSOR);
  this->convict = NULL;
}

/*!
  Returns a pointer to the field connected to the sensor.

  \sa attach(), detach()
 */
SoField *
SoFieldSensor::getAttachedField(void) const
{
  return this->convict;
}

// Doc from superclass.
void
SoFieldSensor::trigger(void)
{
  this->convict->evaluate();
  inherited::trigger();
}

// Doc from superclass.
void
SoFieldSensor::notify(SoNotList * l)
{
  inherited::notify(l);

  // FIXME: I don't know what the heck we're supposed to do here, but
  // I included the overloading in case we find out after 1.0 release,
  // and need to fix the behavior without changing the class signature
  // (which would break binary compatibility). 20000402 mortene.
}

// Doc from superclass.
void
SoFieldSensor::dyingReference(void)
{
  this->detach();
}
