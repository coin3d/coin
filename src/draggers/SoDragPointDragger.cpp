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

#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <data/draggerDefaults/dragPointDragger.h>


SO_KIT_SOURCE(SoDragPointDragger);


void
SoDragPointDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragPointDragger);
}

SoDragPointDragger::SoDragPointDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragPointDragger);

  SO_KIT_ADD_CATALOG_ENTRY(noRotSep, SoSeparator, FALSE, topSeparator, rotXSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackSep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackSwitch, SoSwitch, FALSE, planeFeedbackSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(planeFeedbackTranslation, SoTranslation, FALSE, planeFeedbackSep, planeFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotX, SoRotation, TRUE, rotXSep, xzTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotXSep, SoSeparator, FALSE, topSeparator, rotYSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotY, SoRotation, TRUE, rotYSep, zTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotYSep, SoSeparator, FALSE, topSeparator, rotZSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotZ, SoRotation, TRUE, rotZSep, yTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotZSep, SoSeparator, FALSE, topSeparator, xFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedback, SoSeparator, TRUE, xFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackSep, SoSeparator, FALSE, xFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackSwitch, SoSwitch, FALSE, topSeparator, yFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xFeedbackTranslation, SoTranslation, FALSE, xFeedbackSep, xFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xTranslator, SoTranslate1Dragger, TRUE, xTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xTranslatorSwitch, SoSwitch, FALSE, noRotSep, xyTranslatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xyFeedback, SoSeparator, TRUE, planeFeedbackSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xyTranslator, SoTranslate2Dragger, TRUE, xyTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xyTranslatorSwitch, SoSwitch, FALSE, noRotSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(xzFeedback, SoSeparator, TRUE, planeFeedbackSwitch, xyFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xzTranslator, SoTranslate2Dragger, TRUE, xzTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(xzTranslatorSwitch, SoSwitch, FALSE, rotXSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedback, SoSeparator, TRUE, yFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackSep, SoSeparator, FALSE, yFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackSwitch, SoSwitch, FALSE, topSeparator, zFeedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yFeedbackTranslation, SoTranslation, FALSE, yFeedbackSep, yFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yTranslator, SoTranslate1Dragger, TRUE, yTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yTranslatorSwitch, SoSwitch, FALSE, rotZSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(yzFeedback, SoSeparator, TRUE, planeFeedbackSwitch, xzFeedback, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yzTranslator, SoTranslate2Dragger, TRUE, yzTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(yzTranslatorSwitch, SoSwitch, FALSE, rotYSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedback, SoSeparator, TRUE, zFeedbackSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackSep, SoSeparator, FALSE, zFeedbackSwitch, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackSwitch, SoSwitch, FALSE, topSeparator, planeFeedbackSep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zFeedbackTranslation, SoTranslation, FALSE, zFeedbackSep, zFeedback, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(zTranslator, SoTranslate1Dragger, TRUE, zTranslatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(zTranslatorSwitch, SoSwitch, FALSE, rotYSep, yzTranslatorSwitch, FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("dragPointDragger.iv",
                                       DRAGPOINTDRAGGER_draggergeometry,
                                       sizeof(DRAGPOINTDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_INIT_INSTANCE();

  this->jumpLimit = 0.1f;

  // initialize default parts not contained in simple draggers
  this->setPartAsDefault("xFeedback", "dragPointXFeedback");
  this->setPartAsDefault("yFeedback", "dragPointYFeedback");
  this->setPartAsDefault("zFeedback", "dragPointZFeedback");
  this->setPartAsDefault("xyFeedback", "dragPointXYFeedback");
  this->setPartAsDefault("xzFeedback", "dragPointXZFeedback");
  this->setPartAsDefault("yzFeedback", "dragPointYZFeedback");

  // create simple draggers that compromise this dragger
  (void)SO_GET_ANY_PART(this, "xTranslator", SoTranslate1Dragger);
  (void)SO_GET_ANY_PART(this, "yTranslator", SoTranslate1Dragger);
  (void)SO_GET_ANY_PART(this, "zTranslator", SoTranslate1Dragger);
  (void)SO_GET_ANY_PART(this, "xyTranslator", SoTranslate2Dragger);
  (void)SO_GET_ANY_PART(this, "xzTranslator", SoTranslate2Dragger);
  (void)SO_GET_ANY_PART(this, "yzTranslator", SoTranslate2Dragger);

  // set rotations to align draggers to their respective axis/planes
  SoRotation *xrot = new SoRotation;
  xrot->rotation.setValue(SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI*0.5f));
  this->setAnyPartAsDefault("rotX", xrot);
  SoRotation *yrot = new SoRotation;
  yrot->rotation.setValue(SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), M_PI*0.5f));
  this->setAnyPartAsDefault("rotY", yrot);
  SoRotation *zrot = new SoRotation;
  zrot->rotation.setValue(SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), M_PI*0.5f));
  this->setAnyPartAsDefault("rotZ", zrot);

  // initialize switch nodes
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "planeFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "xFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "yFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "zFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);

  this->currAxis = 1;
  this->updateSwitchNodes();

  this->addStartCallback(SoDragPointDragger::startCB, this);
  this->addMotionCallback(SoDragPointDragger::motionCB, this);
  this->addFinishCallback(SoDragPointDragger::finishCB, this);
  this->addOtherEventCallback(SoDragPointDragger::metaKeyChangeCB, this);

  this->addValueChangedCallback(SoDragPointDragger::valueChangedCB);
  this->fieldSensor = new SoFieldSensor(SoDragPointDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoDragPointDragger::~SoDragPointDragger()
{
  delete this->fieldSensor;
}

SbBool
SoDragPointDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);

    SoDragger *xdragger = (SoDragger*) this->getAnyPart("xTranslator", FALSE);
    xdragger->setPartAsDefault("translator", "dragPointXTranslatorTranslator");
    xdragger->setPartAsDefault("translatorActive", "dragPointXTranslatorTranslatorActive");
    this->registerDragger(xdragger);

    SoDragger *ydragger = (SoDragger*) this->getAnyPart("yTranslator", FALSE);
    ydragger->setPartAsDefault("translator", "dragPointYTranslatorTranslator");
    ydragger->setPartAsDefault("translatorActive", "dragPointYTranslatorTranslatorActive");
    this->registerDragger(ydragger);

    SoDragger *zdragger = (SoDragger*) this->getAnyPart("zTranslator", FALSE);
    zdragger->setPartAsDefault("translator", "dragPointZTranslatorTranslator");
    zdragger->setPartAsDefault("translatorActive", "dragPointZTranslatorTranslatorActive");
    this->registerDragger(zdragger);

    SoDragger *xydragger = (SoDragger*) this->getAnyPart("xyTranslator", FALSE);
    xydragger->setPartAsDefault("translator", "dragPointXYTranslatorTranslator");
    xydragger->setPartAsDefault("translatorActive", "dragPointXYTranslatorTranslatorActive");
    this->registerDragger(xydragger);

    SoDragger *xzdragger = (SoDragger*) this->getAnyPart("xzTranslator", FALSE);
    xzdragger->setPartAsDefault("translator", "dragPointXZTranslatorTranslator");
    xzdragger->setPartAsDefault("translatorActive", "dragPointXZTranslatorTranslatorActive");
    this->registerDragger(xzdragger);

    SoDragger *yzdragger = (SoDragger*) this->getAnyPart("yzTranslator", FALSE);
    yzdragger->setPartAsDefault("translator", "dragPointYZTranslatorTranslator");
    yzdragger->setPartAsDefault("translatorActive", "dragPointYZTranslatorTranslatorActive");
    this->registerDragger(yzdragger);

    SoDragPointDragger::fieldSensorCB(this, NULL);
    if (this->fieldSensor->getAttachedField() != &this->translation) {
      this->fieldSensor->attach(&this->translation);
    }
  }
  else {
    this->unregisterDragger("xTranslator");
    this->unregisterDragger("yTranslator");
    this->unregisterDragger("zTranslator");
    this->unregisterDragger("xyTranslator");
    this->unregisterDragger("xzTranslator");
    this->unregisterDragger("yzTranslator");
    if (this->fieldSensor->getAttachedField() != NULL) {
      this->fieldSensor->detach();
    }

    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoDragPointDragger::setDefaultOnNonWritingFields(void)
{
  this->xTranslator.setDefault(TRUE);
  this->yTranslator.setDefault(TRUE);
  this->zTranslator.setDefault(TRUE);

  this->xyTranslator.setDefault(TRUE);
  this->xzTranslator.setDefault(TRUE);
  this->yzTranslator.setDefault(TRUE);

  this->planeFeedbackTranslation.setDefault(TRUE);
  this->xFeedbackTranslation.setDefault(TRUE);
  this->yFeedbackTranslation.setDefault(TRUE);
  this->zFeedbackTranslation.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

void
SoDragPointDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoDragPointDragger::valueChangedCB(void *, SoDragger * d)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;

  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f t;
  t[0] = matrix[3][0];
  t[1] = matrix[3][1];
  t[2] = matrix[3][2];

  thisp->fieldSensor->detach();
  if (thisp->translation.getValue() != t) {
    thisp->translation = t;
  }
  thisp->fieldSensor->attach(&thisp->translation);
}

void
SoDragPointDragger::setJumpLimit(const float limit)
{
  this->jumpLimit = limit;
}

float
SoDragPointDragger::getJumpLimit(void) const
{
  return this->jumpLimit;
}

void
SoDragPointDragger::showNextDraggerSet(void)
{
  this->currAxis = (this->currAxis + 1) % 3;
  this->updateSwitchNodes();
}

void
SoDragPointDragger::dragStart(void)
{
  SoSwitch *sw;
  if (this->getActiveChildDragger()->isOfType(SoTranslate2Dragger::getClassTypeId())) {
    sw = SO_GET_ANY_PART(this, "planeFeedbackSwitch", SoSwitch);
    SoInteractionKit::setSwitchValue(sw, this->currAxis);
  }
  else {
    switch (this->currAxis) {
    case 0:
      sw = SO_GET_ANY_PART(this, "xFeedbackSwitch", SoSwitch);
      break;
    case 1:
      sw = SO_GET_ANY_PART(this, "yFeedbackSwitch", SoSwitch);
      break;
    case 2:
      sw = SO_GET_ANY_PART(this, "zFeedbackSwitch", SoSwitch);
      break;
    default:
      assert(0); sw = NULL; // Dummy assignment to avoid compiler warning.
      break;
    }
    SoInteractionKit::setSwitchValue(sw, 0);
  }
}

void
SoDragPointDragger::drag(void)
{
  // FIXME: update feedback information, pederb 20000202
}

void
SoDragPointDragger::dragFinish(void)
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "planeFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "xFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "yFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "zFeedbackSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, SO_SWITCH_NONE);
}

void
SoDragPointDragger::startCB(void *d, SoDragger *)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;
  thisp->dragStart();
}

void
SoDragPointDragger::motionCB(void *d, SoDragger *)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;
  thisp->drag();
}

void
SoDragPointDragger::finishCB(void *d, SoDragger *)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;
  thisp->dragFinish();
}

void
SoDragPointDragger::metaKeyChangeCB(void * d, SoDragger *)
{
  SoDragPointDragger *thisp = (SoDragPointDragger*)d;
  // we're only interested if dragger is _not_ active
  if (thisp->getActiveChildDragger()) return;
  const SoEvent *event = thisp->getEvent();
  if (SO_KEY_PRESS_EVENT(event, LEFT_CONTROL) ||
      SO_KEY_PRESS_EVENT(event, RIGHT_CONTROL)) {
    thisp->showNextDraggerSet();
  }
}

void
SoDragPointDragger::registerDragger(SoDragger *dragger)
{
  this->registerChildDragger(dragger);
}

void
SoDragPointDragger::unregisterDragger(const char *name)
{
  SoDragger *dragger = (SoDragger*) this->getAnyPart(name, FALSE);
  this->unregisterChildDragger(dragger);
}

void
SoDragPointDragger::updateSwitchNodes()
{
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "xTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 0 ? 0 : SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "yTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 1 ? 0 : SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "zTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 2 ? 0 : SO_SWITCH_NONE);

  sw = SO_GET_ANY_PART(this, "xyTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 2 ? 0 : SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "xzTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 1 ? 0 : SO_SWITCH_NONE);
  sw = SO_GET_ANY_PART(this, "yzTranslatorSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, this->currAxis == 0 ? 0 : SO_SWITCH_NONE);
}
