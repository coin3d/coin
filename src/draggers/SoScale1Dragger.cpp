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

#include <Inventor/draggers/SoScale1Dragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SbRotation.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/scale1Dragger.h>


SO_KIT_SOURCE(SoScale1Dragger);


void
SoScale1Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale1Dragger);
}

SoScale1Dragger::SoScale1Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale1Dragger);

  SO_KIT_ADD_CATALOG_ENTRY(scalerSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoSeparator, TRUE, scalerSwitch, scalerActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scalerActive, SoSeparator, TRUE, scalerSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("scale1Dragger.iv",
                                       SCALE1DRAGGER_draggergeometry,
                                       sizeof(SCALE1DRAGGER_draggergeometry));
  }

  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("scaler", "scale1Scaler");
  this->setPartAsDefault("scalerActive", "scale1ScalerActive");
  this->setPartAsDefault("feedback", "scale1Feedback");
  this->setPartAsDefault("feedbackActive", "scale1FeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  // setup projector
  this->lineProj = new SbLineProjector();
  this->addStartCallback(SoScale1Dragger::startCB);
  this->addMotionCallback(SoScale1Dragger::motionCB);
  this->addFinishCallback(SoScale1Dragger::finishCB);

  this->addValueChangedCallback(SoScale1Dragger::valueChangedCB);

  this->fieldSensor = new SoFieldSensor(SoScale1Dragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoScale1Dragger::~SoScale1Dragger()
{
  delete this->lineProj;
  delete this->fieldSensor;
}

SbBool
SoScale1Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoScale1Dragger::fieldSensorCB(this, NULL);

    if (this->fieldSensor->getAttachedField() != &this->scaleFactor) {
      this->fieldSensor->attach(&this->scaleFactor);
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
SoScale1Dragger::fieldSensorCB(void * d, SoSensor *)
{
  assert(d);
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f t, s;
  SbRotation r, so;

  matrix.getTransform(t, r, s, so);
  s = thisp->scaleFactor.getValue();
  matrix.setTransform(t, r, s, so);
  thisp->setMotionMatrix(matrix);
}

void
SoScale1Dragger::valueChangedCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->fieldSensor->attach(&thisp->scaleFactor);
}

void
SoScale1Dragger::startCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->dragStart();
}

void
SoScale1Dragger::motionCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->drag();
}

void
SoScale1Dragger::finishCB(void * f, SoDragger * d)
{
  SoScale1Dragger *thisp = (SoScale1Dragger*)d;
  thisp->dragFinish();
}

void
SoScale1Dragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->lineProj->setLine(SbLine(hitPt, hitPt + SbVec3f(1.0f, 0.0f, 0.0f)));
}

void
SoScale1Dragger::drag(void)
{
  this->lineProj->setViewVolume(this->getViewVolume());
  this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = lineProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();

  float motion = projPt[0];
  if (motion > 0.0f && startPt[0] != 0.0f)
    motion /= startPt[0];
  else
    motion = 0.0f;

  this->setMotionMatrix(this->appendScale(this->getStartMotionMatrix(),
                                          SbVec3f(fabs(motion), 1.0f, 1.0f),
                                          SbVec3f(0.0f, 0.0f, 0.0f)));
}

void
SoScale1Dragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "scalerSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
