/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <math.h>

#include <data/draggerDefaults/rotateDiscDragger.h>


SO_KIT_SOURCE(SoRotateDiscDragger);


void
SoRotateDiscDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateDiscDragger);
}

SoRotateDiscDragger::SoRotateDiscDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateDiscDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateDiscDragger.iv",
                                       ROTATEDISCDRAGGER_draggergeometry,
                                       sizeof(ROTATEDISCDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateDiscRotator");
  this->setPartAsDefault("rotatorActive", "rotateDiscRotatorActive");
  this->setPartAsDefault("feedback", "rotateDiscFeedback");
  this->setPartAsDefault("feedbackActive", "rotateDiscFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->planeProj = new SbPlaneProjector();
  this->addStartCallback(SoRotateDiscDragger::startCB);
  this->addMotionCallback(SoRotateDiscDragger::motionCB);
  this->addFinishCallback(SoRotateDiscDragger::doneCB);

  this->addValueChangedCallback(SoRotateDiscDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateDiscDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoRotateDiscDragger::~SoRotateDiscDragger()
{
  delete this->fieldSensor;
  delete this->planeProj;
}

SbBool
SoRotateDiscDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateDiscDragger::fieldSensorCB(this, NULL);

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
SoRotateDiscDragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  r = thisp->rotation.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

void
SoRotateDiscDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
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
SoRotateDiscDragger::startCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->dragStart();
}

void
SoRotateDiscDragger::motionCB(void *, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->drag();
}

void
SoRotateDiscDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateDiscDragger *thisp = (SoRotateDiscDragger*)d;
  thisp->dragFinish();
}

void
SoRotateDiscDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->planeProj->setPlane(SbPlane(SbVec3f(0.0f, 0.0f, 1.0f),
                                    hitPt));
}

void
SoRotateDiscDragger::drag(void)
{
  this->planeProj->setViewVolume(this->getViewVolume());
  this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = planeProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  SbPlane plane(SbVec3f(0.0f, 0.0f, 1.0f), startPt);
  SbLine line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 1.0f));
  SbVec3f center;
  plane.intersect(line, center);
  startPt -= center;
  projPt -= center;
  startPt.normalize();
  projPt.normalize();
  SbVec3f dir = startPt.cross(projPt);
  float angle = acos(SbClamp(startPt.dot(projPt), -1.0f, 1.0f));
  if (dir[2] < 0.0f) angle = -angle;

  this->setMotionMatrix(this->appendRotation(this->getStartMotionMatrix(),
                                             SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), angle),
                                             SbVec3f(0.0f, 0.0f, 0.0f)));
}

void
SoRotateDiscDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
