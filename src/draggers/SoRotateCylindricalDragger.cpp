/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/rotateCylindricalDragger.h>


SO_KIT_SOURCE(SoRotateCylindricalDragger);


void
SoRotateCylindricalDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateCylindricalDragger);
}

SoRotateCylindricalDragger::SoRotateCylindricalDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateCylindricalDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateCylindricalDragger.iv",
                                       ROTATECYLINDRICALDRAGGER_draggergeometry,
                                       sizeof(ROTATECYLINDRICALDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateCylindricalRotator");
  this->setPartAsDefault("rotatorActive", "rotateCylindricalRotatorActive");
  this->setPartAsDefault("feedback", "rotateCylindricalFeedback");
  this->setPartAsDefault("feedbackActive", "rotateCylindricalFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->cylinderProj = new SbCylinderPlaneProjector();
  this->userProj = FALSE;
  this->addStartCallback(SoRotateCylindricalDragger::startCB);
  this->addMotionCallback(SoRotateCylindricalDragger::motionCB);
  this->addFinishCallback(SoRotateCylindricalDragger::doneCB);

  this->addValueChangedCallback(SoRotateCylindricalDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateCylindricalDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoRotateCylindricalDragger::~SoRotateCylindricalDragger()
{
  delete this->fieldSensor;
  if (!this->userProj) delete this->cylinderProj;
}

SbBool
SoRotateCylindricalDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateCylindricalDragger::fieldSensorCB(this, NULL);

    if (this->fieldSensor->getAttachedField() != &this->rotation) {
      this->fieldSensor->attach(&this->rotation);
    }
  }
  else {
    if (this->fieldSensor->getAttachedField() != NULL) {
      this->fieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  this->connectionsSetUp = onoff;
  return oldval;
}

void
SoRotateCylindricalDragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  r = thisp->rotation.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

void
SoRotateCylindricalDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->fieldSensor->attach(&thisp->rotation);
}

void
SoRotateCylindricalDragger::setProjector(SbCylinderProjector * p)
{
  if (!this->userProj) delete this->cylinderProj;
  this->cylinderProj = p;
}

const SbCylinderProjector *
SoRotateCylindricalDragger::getProjector(void) const
{
  return this->cylinderProj;
}

void
SoRotateCylindricalDragger::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  assert(fromfc->isOfType(SoRotateCylindricalDragger::getClassTypeId()));
  SoRotateCylindricalDragger *from = (SoRotateCylindricalDragger *)fromfc;
  this->cylinderProj = (SbCylinderProjector*)from->cylinderProj->copy();
  this->userProj = FALSE;
  if (copyconnections) {
    COIN_STUB();
  }
}

void
SoRotateCylindricalDragger::startCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->dragStart();
}

void
SoRotateCylindricalDragger::motionCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->drag();
}

void
SoRotateCylindricalDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateCylindricalDragger *thisp = (SoRotateCylindricalDragger*)d;
  thisp->dragFinish();
}

void
SoRotateCylindricalDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  SbLine line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f));
  SbVec3f ptOnLine = line.getClosestPoint(hitPt);
  this->cylinderProj->setCylinder(SbCylinder(line, (ptOnLine-hitPt).length()));
}

void
SoRotateCylindricalDragger::drag(void)
{
  this->cylinderProj->setViewVolume(this->getViewVolume());
  this->cylinderProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = cylinderProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  SbRotation rot = this->cylinderProj->getRotation(startPt, projPt);

  this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                             rot, SbVec3f(0.0f, 0.0f, 0.0f)));
}

void
SoRotateCylindricalDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
