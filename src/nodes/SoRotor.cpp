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
  \class SoRotor SoRotor.h Inventor/nodes/SoRotor.h
  \brief The SoRotor class is a node used to animate rotations.
  \ingroup nodes

  Insert this node in your hierarchy to apply a continuous rotation
  to some geometry. Since this node inherits the SoRotation node,
  rotation is restricted around the axis specified in the
  rotation field.

  The angle value of the SoRotation::rotation field (inherited by this
  node) gives the rotation direction. If the value is larger than 0.0,
  the rotation will be done in the counterclockwise direction, if it
  is negative the rotation will be done in the clockwise direction.

  Do not set the rotation value to exactly 0.0, as rotations are
  internally handled as quaternions, and when converting from an angle
  and a rotation value to a quaternion representation, the information
  about the angle "gets lost" if there is no actual rotation.

  Note also that the rotation will start at the given angle value.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Rotor {
        rotation 0 0 1  0
        speed 1
        on TRUE
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoRotor.h>

#include <Inventor/SbVec3f.h>
#include <Inventor/SoDB.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include "coindefs.h"
#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFFloat SoRotor::speed
  Specifies the speed of rotation, in revolutions per second. Defaults
  to 1.0.
*/
/*!
  \var SoSFBool SoRotor::on
  Enable/disable rotation. Default enabled.
*/

// *************************************************************************

//
// returns the current time. First tries the realTime field, then
// SbTime::getTimeOfDay() if field is not found.
//
static SbTime
get_current_time(void)
{
  SoField * realtime = SoDB::getGlobalField("realTime");
  if (realtime && realtime->isOfType(SoSFTime::getClassTypeId())) {
    return ((SoSFTime*)realtime)->getValue();
  }
  return SbTime::getTimeOfDay();
}

class SoRotorP {
public:
  SoRotorP(SoRotor * m) { this->master = m; }

  SbTime starttime;
  SbVec3f startaxis;
  float startangle;
  SoFieldSensor * rtfieldsensor;
  SoFieldSensor * onfieldsensor;
  SoFieldSensor * rotfieldsensor;
  SoFieldSensor * speedfieldsensor;

  static void rtFieldSensorCB(void * d, SoSensor * s);
  static void fieldSensorCB(void * d, SoSensor * s);
  void setRotation(void);

private:
  SoRotor * master;
};

#define PUBLIC(p) ((p)->master)
#define PRIVATE(p) ((p)->pimpl)

SO_NODE_SOURCE(SoRotor);

/*!
  Constructor.
*/
SoRotor::SoRotor(void)
{
  PRIVATE(this) = new SoRotorP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoRotor);

  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));

  SoField * f = SoDB::getGlobalField("realTime");
  PRIVATE(this)->rtfieldsensor = new SoFieldSensor(SoRotorP::rtFieldSensorCB, this);
  PRIVATE(this)->rtfieldsensor->attach(f);
  PRIVATE(this)->rtfieldsensor->setPriority(0);
  PRIVATE(this)->onfieldsensor = new SoFieldSensor(SoRotorP::fieldSensorCB, this);
  PRIVATE(this)->onfieldsensor->setPriority(0);
  PRIVATE(this)->onfieldsensor->attach(&this->on);
  PRIVATE(this)->speedfieldsensor = new SoFieldSensor(SoRotorP::fieldSensorCB, this);
  PRIVATE(this)->speedfieldsensor->setPriority(0);
  PRIVATE(this)->speedfieldsensor->attach(&this->speed);
  PRIVATE(this)->rotfieldsensor = new SoFieldSensor(SoRotorP::fieldSensorCB, this);
  PRIVATE(this)->rotfieldsensor->attach(&this->rotation);
  PRIVATE(this)->rotfieldsensor->setPriority(0);

  PRIVATE(this)->starttime = SbTime::zero();
  this->rotation.getValue(PRIVATE(this)->startaxis, PRIVATE(this)->startangle);
}


/*!
  Destructor.
*/
SoRotor::~SoRotor()
{
  delete PRIVATE(this)->rotfieldsensor;
  delete PRIVATE(this)->rtfieldsensor;
  delete PRIVATE(this)->onfieldsensor;
  delete PRIVATE(this)->speedfieldsensor;
  delete PRIVATE(this);
}

// Doc from parent.
void
SoRotor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoRotor, SO_FROM_INVENTOR_1);
}

// detects when some field changes value
void
SoRotorP::fieldSensorCB(void * d, SoSensor * s)
{
  SoRotor * thisp = (SoRotor *) d;

  if (s == PRIVATE(thisp)->onfieldsensor) {
    if (thisp->on.getValue() == TRUE) {
      thisp->rotation.getValue(PRIVATE(thisp)->startaxis, PRIVATE(thisp)->startangle);
      PRIVATE(thisp)->starttime = get_current_time();
    }
  }
  else if (s == PRIVATE(thisp)->speedfieldsensor) {
    thisp->rotation.getValue(PRIVATE(thisp)->startaxis, PRIVATE(thisp)->startangle);
    PRIVATE(thisp)->starttime = get_current_time();
  }
  else if (s == PRIVATE(thisp)->rotfieldsensor) {
    thisp->rotation.getValue(PRIVATE(thisp)->startaxis, PRIVATE(thisp)->startangle);
    PRIVATE(thisp)->starttime = get_current_time();
  }
}


void
SoRotorP::rtFieldSensorCB(void * d, SoSensor *)
{
  SoRotor * thisp = (SoRotor *) d;
  // got to check value of on field here in case rtfieldsensor
  // triggers before onfieldsensor.
  if (thisp->on.getValue()) {
    PRIVATE(thisp)->setRotation();
  }
}

// sets rotation based on time passed from starttime
void
SoRotorP::setRotation(void)
{
  if (this->starttime == SbTime::zero()) {
    // don't do anything first time we get here
    this->starttime = get_current_time();
    return;
  }
  SbTime difftime = get_current_time() - this->starttime;

  float diffangle = (float)
    (difftime.getValue() *
     ((double)PUBLIC(this)->speed.getValue()) * M_PI * 2.0);

  float angle = this->startangle + diffangle;

  if (angle < 0.0f) {
    angle = (float) (2.0 * M_PI - fmod((double)-angle, M_PI*2.0));
  }
  if (angle > M_PI * 2.0f) {
    angle = (float) fmod((double)angle, M_PI * 2.0);
  }
  
  this->rotfieldsensor->detach();
  PUBLIC(this)->rotation.setValue(SbRotation(this->startaxis, angle));
  this->rotfieldsensor->attach(&PUBLIC(this)->rotation);
}

#undef PRIVATE
#undef PUBLIC
