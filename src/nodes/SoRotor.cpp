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

  Note also that the rotation will start at the given angle value.
*/

#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFTime.h>
#include <coindefs.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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


SO_NODE_SOURCE(SoRotor);

/*!
  Constructor.
*/
SoRotor::SoRotor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoRotor);

  SO_NODE_ADD_FIELD(speed, (1.0f));
  SO_NODE_ADD_FIELD(on, (TRUE));

  this->oneshotsensor = new SoOneShotSensor(SoRotor::oneshotSensorCB, this);
  this->oneshotsensor->setPriority(1);
  this->onfieldsensor = new SoFieldSensor(SoRotor::fieldSensorCB, this);
  this->onfieldsensor->setPriority(0);
  this->onfieldsensor->attach(&this->on);
  this->speedfieldsensor = new SoFieldSensor(SoRotor::fieldSensorCB, this);
  this->speedfieldsensor->setPriority(0);
  this->speedfieldsensor->attach(&this->speed);
  this->rotfieldsensor = new SoFieldSensor(SoRotor::fieldSensorCB, this);
  this->rotfieldsensor->attach(&this->rotation);
  this->rotfieldsensor->setPriority(0);

  this->starttime = SbTime::zero();
  this->rotation.getValue(this->startaxis, this->startangle);
  this->oneshotsensor->schedule();
}


/*!
  Destructor.
*/
SoRotor::~SoRotor()
{
  delete this->rotfieldsensor;
  delete this->oneshotsensor;
  delete this->onfieldsensor;
  delete this->speedfieldsensor;
}

// Doc from parent.
void
SoRotor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoRotor, SO_FROM_INVENTOR_2_1);
}

// detects when some field changes value
void
SoRotor::fieldSensorCB(void * d, SoSensor * s)
{
  SoRotor * thisp = (SoRotor *) d;

  if (s == thisp->onfieldsensor) {
    if (thisp->on.getValue() == FALSE) {
      if (thisp->oneshotsensor->isScheduled())
        thisp->oneshotsensor->unschedule();
    }
    else {
      thisp->rotation.getValue(thisp->startaxis, thisp->startangle);
      thisp->starttime = get_current_time();
      if (!thisp->oneshotsensor->isScheduled())
        thisp->oneshotsensor->schedule();
    }
  }
  else if (s == thisp->speedfieldsensor) {
    thisp->rotation.getValue(thisp->startaxis, thisp->startangle);
    thisp->starttime = get_current_time();
  }
  else if (s == thisp->rotfieldsensor) {
    thisp->rotation.getValue(thisp->startaxis, thisp->startangle);
    thisp->starttime = get_current_time();
  }
}

// oneshot sensor is used to animate
void
SoRotor::oneshotSensorCB(void * d, SoSensor *)
{
  SoRotor * thisp = (SoRotor *) d;
  // got to check value of on field here in case oneshotsensor
  // triggers before onfieldsensor.
  if (thisp->on.getValue()) {
    thisp->setRotation();
    thisp->oneshotsensor->schedule();
  }
}

// sets rotation based on time passed from starttime
void
SoRotor::setRotation(void)
{
  if (this->starttime == SbTime::zero()) {
    // don't do anything first time we get here
    this->starttime = get_current_time();
    return;
  }
  SbTime difftime = get_current_time() - this->starttime;

  float diffangle = (float)
    (difftime.getValue() *
     ((double)this->speed.getValue()) * M_PI * 2.0);

  float angle = this->startangle + diffangle;

  if (angle > M_PI * 2.0f) {
    angle = (float) fmod((double)angle, M_PI * 2.0);
  }

  this->rotfieldsensor->detach();
  this->rotation.setValue(SbRotation(this->startaxis, angle));
  this->rotfieldsensor->attach(&this->rotation);
}
