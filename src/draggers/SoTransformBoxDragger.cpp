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

#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/transformBoxDragger.h>

SO_KIT_SOURCE(SoTransformBoxDragger);


void
SoTransformBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTransformBoxDragger);
}

SoTransformBoxDragger::SoTransformBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTransformBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, scaler, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoScaleUniformDragger, TRUE, topSeparator, rotator1Sep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Sep, SoSeparator, FALSE, topSeparator, rotator2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1Rot, SoRotation, TRUE, rotator1Sep, rotator1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator1, SoRotateCylindricalDragger, TRUE, rotator1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Sep, SoSeparator, FALSE, topSeparator, rotator3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2Rot, SoRotation, TRUE, rotator2Sep, rotator2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator2, SoRotateCylindricalDragger, TRUE, rotator2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Sep, SoSeparator, FALSE, topSeparator, translator1Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3Rot, SoRotation, TRUE, rotator3Sep, rotator3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator3, SoRotateCylindricalDragger, TRUE, rotator3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Sep, SoSeparator, FALSE, topSeparator, translator2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1Rot, SoRotation, TRUE, translator1Sep, translator1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator1, SoTranslate2Dragger, TRUE, translator1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Sep, SoSeparator, FALSE, topSeparator, translator3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2Rot, SoRotation, TRUE, translator2Sep, translator2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator2, SoTranslate2Dragger, TRUE, translator2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Sep, SoSeparator, FALSE, topSeparator, translator4Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3Rot, SoRotation, TRUE, translator3Sep, translator3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator3, SoTranslate2Dragger, TRUE, translator3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Sep, SoSeparator, FALSE, topSeparator, translator5Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4Rot, SoRotation, TRUE, translator4Sep, translator4, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator4, SoTranslate2Dragger, TRUE, translator4Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Sep, SoSeparator, FALSE, topSeparator, translator6Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5Rot, SoRotation, TRUE, translator5Sep, translator5, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator5, SoTranslate2Dragger, TRUE, translator5Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Sep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6Rot, SoRotation, TRUE, translator6Sep, translator6, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator6, SoTranslate2Dragger, TRUE, translator6Sep, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("transformBoxDragger.iv",
                                       TRANSFORMBOXDRAGGER_draggergeometry,
                                       sizeof(TRANSFORMBOXDRAGGER_draggergeometry));
  }

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  SO_GET_ANY_PART(this, "scaler", SoScaleUniformDragger);
  SO_GET_ANY_PART(this, "rotator1", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "rotator2", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "rotator3", SoRotateCylindricalDragger);
  SO_GET_ANY_PART(this, "translator1", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator2", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator3", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator4", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator5", SoTranslate2Dragger);
  SO_GET_ANY_PART(this, "translator6", SoTranslate2Dragger);

  SoRotation *rot;
  rot = SO_GET_ANY_PART(this, "rotator1Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "rotator2Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "rotator3Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);

  rot = SO_GET_ANY_PART(this, "translator1Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "translator2Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), -M_PI/2.0f);

  rot = SO_GET_ANY_PART(this, "translator3Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI/2.0f);
  rot = SO_GET_ANY_PART(this, "translator4Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), -M_PI/2.0f);

  rot = SO_GET_ANY_PART(this, "translator5Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), M_PI);
  rot = SO_GET_ANY_PART(this, "translator6Rot", SoRotation);
  rot->rotation = SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f);

  SoAntiSquish *squish = SO_GET_ANY_PART(this, "antiSquish", SoAntiSquish);
  squish->sizing = SoAntiSquish::BIGGEST_DIMENSION;

  this->addValueChangedCallback(SoTransformBoxDragger::valueChangedCB);
  this->rotFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoTransformBoxDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);
  this->setUpConnections(TRUE, TRUE);
}


SoTransformBoxDragger::~SoTransformBoxDragger()
{
  delete this->rotFieldSensor;
  delete this->translFieldSensor;
  delete this->scaleFieldSensor;
}

SbBool
SoTransformBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    int i;
    SbString str;
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child = (SoDragger*) this->getAnyPart("scaler", FALSE);
    child->setPartAsDefault("scaler", "transformBoxScalerScaler");
    child->setPartAsDefault("scalerActive", "transformBoxScalerScalerActive");
    child->setPartAsDefault("feedback", "transformBoxScalerFeedback");
    child->setPartAsDefault("feedbackActive", "transformBoxScalerFeedbackActive");
    this->registerChildDragger(child);

    for (i = 1; i <= 3; i++) {
      str.sprintf("rotator%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("rotator", "transformBoxRotatorRotator");
      child->setPartAsDefault("rotatorActive", "transformBoxRotatorRotatorActive");
      child->setPartAsDefault("feedback", "transformBoxRotatorFeedback");
      child->setPartAsDefault("feedbackActive", "transformBoxRotatorFeedbackActive");
      this->registerChildDragger(child);
    }

    for (i = 1; i <= 6; i++) {
      str.sprintf("translator%d", i);
      child = (SoDragger*)this->getAnyPart(str.getString(), FALSE);
      child->setPartAsDefault("translator", "transformBoxTranslatorTranslator");
      child->setPartAsDefault("translatorActive", "transformBoxTranslatorTranslatorActive");
      child->setPartAsDefault("xAxisFeedback", "transformBoxTranslatorXAxisFeedback");
      child->setPartAsDefault("yAxisFeedback", "transformBoxTranslatorYAxisFeedback");
      this->registerChildDragger(child);
    }
    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
    if (this->rotFieldSensor->getAttachedField() != &this->rotation) {
      this->rotFieldSensor->attach(&this->rotation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
  }
  else {
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("scaler", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("rotator1", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("rotator2", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("rotator3", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator1", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator2", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator3", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator4", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator5", FALSE));
    this->unregisterChildDragger((SoDragger*)this->getAnyPart("translator6", FALSE));

    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
    if (this->rotFieldSensor->getAttachedField() != NULL) {
      this->rotFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoTransformBoxDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoTransformBoxDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoTransformBoxDragger *thisp = (SoTransformBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoTransformBoxDragger::valueChangedCB(void *, SoDragger * d)
{
  SoTransformBoxDragger *thisp = (SoTransformBoxDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();

  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->rotFieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->rotFieldSensor->attach(&thisp->rotation);

  thisp->scaleFieldSensor->detach();
  if (thisp->scaleFactor.getValue() != scale)
    thisp->scaleFactor = scale;
  thisp->scaleFieldSensor->attach(&thisp->scaleFactor);
}

void
SoTransformBoxDragger::invalidateSurroundScaleCB(void *, SoDragger * d)
{
  SoTransformBoxDragger *thisp = (SoTransformBoxDragger*) d;
  SoSurroundScale *surround = SO_GET_ANY_PART(thisp, "surroundScale", SoSurroundScale);
  surround->invalidate();
}
