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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/rotateSphericalDragger.h>


//
// FIXME: investigate what to do with prevMotionMatrix and
// prevWorldHitPoint, pederb 2000-01-27

SO_KIT_SOURCE(SoRotateSphericalDragger);


void
SoRotateSphericalDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateSphericalDragger);
}

SoRotateSphericalDragger::SoRotateSphericalDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateSphericalDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("rotateSphericalDragger.iv",
                                       ROTATESPHERICALDRAGGER_draggergeometry,
                                       sizeof(ROTATESPHERICALDRAGGER_draggergeometry));
  }

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("rotator", "rotateSphericalRotator");
  this->setPartAsDefault("rotatorActive", "rotateSphericalRotatorActive");
  this->setPartAsDefault("feedback", "rotateSphericalFeedback");
  this->setPartAsDefault("feedbackActive", "rotateSphericalFeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->sphereProj = new SbSpherePlaneProjector();
  this->userProj = FALSE;
  this->addStartCallback(SoRotateSphericalDragger::startCB);
  this->addMotionCallback(SoRotateSphericalDragger::motionCB);
  this->addFinishCallback(SoRotateSphericalDragger::doneCB);

  this->addValueChangedCallback(SoRotateSphericalDragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoRotateSphericalDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoRotateSphericalDragger::~SoRotateSphericalDragger()
{
  delete this->fieldSensor;
  if (!this->userProj) delete this->sphereProj;
}

SbBool
SoRotateSphericalDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoRotateSphericalDragger::fieldSensorCB(this, NULL);

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
SoRotateSphericalDragger::fieldSensorCB(void *d, SoSensor *)
{
  assert(d);
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  matrix.setTransform(trans, thisp->rotation.getValue(), scale, scaleOrient);
  thisp->setMotionMatrix(matrix);
}

void
SoRotateSphericalDragger::valueChangedCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
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
SoRotateSphericalDragger::setProjector(SbSphereProjector * p)
{
  if (!this->userProj) delete this->sphereProj;
  this->userProj = TRUE;
  this->sphereProj = p;
}

const SbSphereProjector *
SoRotateSphericalDragger::getProjector(void) const
{
  return this->sphereProj;
}

void
SoRotateSphericalDragger::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  assert(fromfc->isOfType(SoRotateSphericalDragger::getClassTypeId()));
  SoRotateSphericalDragger *from = (SoRotateSphericalDragger*) fromfc;

  this->sphereProj = (SbSphereProjector*)from->sphereProj->copy();
  this->userProj = FALSE;

  if (copyconnections) {
    COIN_STUB();
  }
}

void
SoRotateSphericalDragger::startCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->dragStart();
}

void
SoRotateSphericalDragger::motionCB(void *, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->drag();
}

void
SoRotateSphericalDragger::doneCB(void * f, SoDragger * d)
{
  SoRotateSphericalDragger *thisp = (SoRotateSphericalDragger*)d;
  thisp->dragFinish();
}

void
SoRotateSphericalDragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  float radius = hitPt.length();
  this->sphereProj->setSphere(SbSphere(SbVec3f(0.0f, 0.0f, 0.0f), radius));

  this->sphereProj->setViewVolume(this->getViewVolume());
  this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = this->sphereProj->project(this->getNormalizedLocaterPosition());
  this->getLocalToWorldMatrix().multVecMatrix(projPt, this->prevWorldHitPt);
  this->prevMotionMatrix = this->getMotionMatrix();
}

void
SoRotateSphericalDragger::drag(void)
{
  this->sphereProj->setViewVolume(this->getViewVolume());
  this->sphereProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f startPt;
  this->getWorldToLocalMatrix().multVecMatrix(this->prevWorldHitPt, startPt);
  SbVec3f projPt = this->sphereProj->project(this->getNormalizedLocaterPosition());
  this->getLocalToWorldMatrix().multVecMatrix(projPt, this->prevWorldHitPt);

  SbRotation rot = this->sphereProj->getRotation(startPt, projPt);

  this->prevMotionMatrix = this->appendRotation(this->prevMotionMatrix, rot,
                                                SbVec3f(0.0f, 0.0f, 0.0f));
  this->setMotionMatrix(this->prevMotionMatrix);
}

void
SoRotateSphericalDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "rotatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
