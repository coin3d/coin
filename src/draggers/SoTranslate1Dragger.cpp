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

#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/sensors/SoFieldSensor.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_KIT_SOURCE(SoTranslate1Dragger);


void
SoTranslate1Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTranslate1Dragger);
}

SoTranslate1Dragger::SoTranslate1Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTranslate1Dragger);

  SO_KIT_ADD_CATALOG_ENTRY(translatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoSeparator, TRUE, translatorSwitch, translatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorActive, SoSeparator, TRUE, translatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("translate1Dragger.iv", NULL, 0);
  }

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();

  // initialize default parts
  this->setPartAsDefault("translator", "translate1Translator");
  this->setPartAsDefault("translatorActive", "translate1TranslatorActive");
  this->setPartAsDefault("feedback", "translate1Feedback");
  this->setPartAsDefault("feedbackActive", "translate1FeedbackActive");

  // initialize swich values
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  
  // setup projector
  this->lineProj = new SbLineProjector();
  this->addStartCallback(SoTranslate1Dragger::startCB);
  this->addMotionCallback(SoTranslate1Dragger::motionCB);
  this->addFinishCallback(SoTranslate1Dragger::finishCB);

  this->addValueChangedCallback(SoTranslate1Dragger::valueChangedCB);
  
  this->fieldSensor = new SoFieldSensor(SoTranslate1Dragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);
 
  this->setUpConnections(TRUE, TRUE);
}


SoTranslate1Dragger::~SoTranslate1Dragger()
{
  delete this->lineProj;
  delete this->fieldSensor;
}

SbBool
SoTranslate1Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  SbBool oldval = this->connectionsSetUp;
  
  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    
    SoTranslate1Dragger::fieldSensorCB(this, NULL);
    
    if (this->fieldSensor->getAttachedField() != &this->translation) {
      this->fieldSensor->attach(&this->translation);
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
SoTranslate1Dragger::fieldSensorCB(void *d, SoSensor *)
{
  assert(d);
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f t = thisp->translation.getValue();
  matrix[3][0] = t[0];
  matrix[3][1] = t[1];
  matrix[3][2] = t[2];
  thisp->setMotionMatrix(matrix);
}

void
SoTranslate1Dragger::valueChangedCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

#if 0
  SbVec3f t;
  t[0] = matrix[3][0];
  t[1] = matrix[3][1];
  t[2] = matrix[3][2];
  
  thisp->fieldSensor->detach();
  if (thisp->translation.getValue() != t) {
    thisp->translation = t;
  }
  thisp->fieldSensor->attach(&thisp->translation);
#else
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);
  thisp->fieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->fieldSensor->attach(&thisp->translation);
#endif
}

void
SoTranslate1Dragger::startCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->dragStart();
}

void
SoTranslate1Dragger::motionCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->drag();
}

void
SoTranslate1Dragger::finishCB(void *, SoDragger * d)
{
  SoTranslate1Dragger *thisp = (SoTranslate1Dragger*)d;
  thisp->dragFinish();
}

void
SoTranslate1Dragger::dragStart(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  this->lineProj->setLine(SbLine(hitPt, hitPt + SbVec3f(1.0f, 0.0f, 0.0f)));
}

void
SoTranslate1Dragger::drag(void)
{
  this->lineProj->setViewVolume(this->getViewVolume());
  this->lineProj->setWorkingSpace(this->getLocalToWorldMatrix());

  SbVec3f projPt = lineProj->project(this->getNormalizedLocaterPosition());
  SbVec3f startPt = this->getLocalStartingPoint();
  SbVec3f motion = projPt - startPt;  
  this->setMotionMatrix(this->appendTranslation(this->getStartMotionMatrix(), motion));
}

void
SoTranslate1Dragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "translatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
  sw = SO_GET_ANY_PART(this, "feedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}
