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
  \class SoRotor SoRotor.h Inventor/nodes/SoRotor.h
  \brief The SoRotor class is a node used to animate rotations.
  \ingroup nodes

  Insert this node in your hierarchy to apply a continuous rotation
  to some geometry. Since this node inherits the SoRotation node,
  rotation is restricted around the axis specified in the
  rotation field.
*/

#include <Inventor/nodes/SoRotor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/SbVec3f.h>
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
  this->onfieldsensor->attach(&this->on);
  this->speedfieldsensor = new SoFieldSensor(SoRotor::fieldSensorCB, this);
  this->speedfieldsensor->attach(&this->speed);
  this->rotfieldsensor = new SoFieldSensor(SoRotor::fieldSensorCB, this);
  this->rotfieldsensor->attach(&this->rotation);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoRotor);
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
      thisp->starttime = SbTime::getTimeOfDay();
      if (!thisp->oneshotsensor->isScheduled())
        thisp->oneshotsensor->schedule();
    }
  }
  else if (s == thisp->speedfieldsensor) {
    thisp->rotation.getValue(thisp->startaxis, thisp->startangle);
    thisp->starttime = SbTime::getTimeOfDay();
  }
  else if (s == thisp->rotfieldsensor) {
    thisp->rotation.getValue(thisp->startaxis, thisp->startangle);
    thisp->starttime = SbTime::getTimeOfDay();
  }
}

// oneshot sensor is used to animate
void
SoRotor::oneshotSensorCB(void * d, SoSensor *)
{
  SoRotor * thisp = (SoRotor *) d;
  assert(thisp->on.getValue() == TRUE);
  thisp->setRotation();
  thisp->oneshotsensor->schedule();
}

// sets rotation based on time passed from starttime
void
SoRotor::setRotation(void)
{
  if (this->starttime == SbTime::zero()) {
    // don't do anything first time we get here
    this->starttime = SbTime::getTimeOfDay();
    return;
  }
  SbTime difftime = SbTime::getTimeOfDay() - this->starttime;

  float diffangle = (float)
    (difftime.getValue() *
     ((double)this->speed.getValue()) * M_PI * 2.0);
  
  float angle = this->startangle + diffangle;

  if (angle > M_PI * 2.0f) {
    angle = (float) fmod(angle, M_PI * 2.0);
  }

  this->rotfieldsensor->detach();
  this->rotation.setValue(SbRotation(this->startaxis, angle));
  this->rotfieldsensor->attach(&this->rotation);
}
