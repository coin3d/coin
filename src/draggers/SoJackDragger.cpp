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

/*!
  \class SoJackDragger SoJackDragger.h Inventor/draggers/SoJackDragger.h
  \brief The SoJackDragger class is a dragger you can translate, rotate and scale.
  \ingroup draggers

  FIXME: write class doc
*/

#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/jackDragger.h>

SO_KIT_SOURCE(SoJackDragger);


void
SoJackDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoJackDragger);
}

/*!
  Default constructor.
*/
SoJackDragger::SoJackDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoJackDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, scaler, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoScaleUniformDragger, TRUE, topSeparator, rotator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, topSeparator, geomSeparator, TRUE);


  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("jackDragger.iv",
                                       JACKDRAGGER_draggergeometry,
                                       sizeof(JACKDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();

  SO_GET_ANY_PART(this, "translator", SoDragPointDragger);
  SO_GET_ANY_PART(this, "rotator", SoRotateSphericalDragger);
  SO_GET_ANY_PART(this, "scaler", SoScaleUniformDragger);

  SoAntiSquish *squish = SO_GET_ANY_PART(this, "antiSquish", SoAntiSquish);
  squish->sizing = SoAntiSquish::BIGGEST_DIMENSION;

  this->addValueChangedCallback(SoJackDragger::valueChangedCB);
  this->rotFieldSensor = new SoFieldSensor(SoJackDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoJackDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->scaleFieldSensor = new SoFieldSensor(SoJackDragger::fieldSensorCB, this);
  this->scaleFieldSensor->setPriority(0);
  this->setUpConnections(TRUE, TRUE);
}

/*!
  Destructor.
*/
SoJackDragger::~SoJackDragger()
{
  delete this->rotFieldSensor;
  delete this->scaleFieldSensor;
  delete this->translFieldSensor;
}

SbBool
SoJackDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child;
    child = (SoDragger*) this->getAnyPart("rotator", FALSE);
    child->setPartAsDefault("rotator",
                            "jackRotatorRotator");
    child->setPartAsDefault("rotatorActive",
                            "jackRotatorRotatorActive");
    child->setPartAsDefault("feedback",
                            "jackRotatorFeedback");
    this->addChildDragger(child);

    child = (SoDragger*) this->getAnyPart("scaler", FALSE);
    child->setPartAsDefault("scaler", "jackScalerScaler");
    child->setPartAsDefault("scalerActive", "jackScalerScalerActive");
    child->setPartAsDefault("feedback", "jackScalerFeedback");
    child->setPartAsDefault("feedbackActive", "jackScalerFeedbackActive");
    this->addChildDragger(child);

    child = (SoDragger*) this->getAnyPart("translator", FALSE);
    child->setPartAsDefault("xTranslator.translator", "jackTranslatorLineTranslator");
    child->setPartAsDefault("yTranslator.translator", "jackTranslatorLineTranslator");
    child->setPartAsDefault("zTranslator.translator", "jackTranslatorLineTranslator");
    child->setPartAsDefault("xTranslator.translatorActive", "jackTranslatorLineTranslatorActive");
    child->setPartAsDefault("yTranslator.translatorActive", "jackTranslatorLineTranslatorActive");
    child->setPartAsDefault("zTranslator.translatorActive", "jackTranslatorLineTranslatorActive");
    child->setPartAsDefault("xzTranslator.translator", "jackTranslatorPlaneTranslator");
    child->setPartAsDefault("xyTranslator.translator", "jackTranslatorPlaneTranslator");
    child->setPartAsDefault("yzTranslator.translator", "jackTranslatorPlaneTranslator");
    child->setPartAsDefault("xzTranslator.translatorActive", "jackTranslatorPlaneTranslatorActive");
    child->setPartAsDefault("xyTranslator.translatorActive", "jackTranslatorPlaneTranslatorActive");
    child->setPartAsDefault("yzTranslator.translatorActive", "jackTranslatorPlaneTranslatorActive");
    child->setPartAsDefault("xFeedback", "jackTranslatorXFeedback");
    child->setPartAsDefault("yFeedback", "jackTranslatorYFeedback");
    child->setPartAsDefault("zFeedback", "jackTranslatorZFeedback");
    child->setPartAsDefault("xzFeedback", "jackTranslatorXZFeedback");
    child->setPartAsDefault("xyFeedback", "jackTranslatorXYFeedback");
    child->setPartAsDefault("yzFeedback", "jackTranslatorYZFeedback");
    this->addChildDragger(child);

    if (this->rotFieldSensor->getAttachedField() != &this->rotation) {
      this->rotFieldSensor->attach(&this->rotation);
    }
    if (this->scaleFieldSensor->getAttachedField() != &this->scaleFactor) {
      this->scaleFieldSensor->attach(&this->scaleFactor);
    }
    if (this->translFieldSensor->getAttachedField() != &this->translation) {
      this->translFieldSensor->attach(&this->translation);
    }
  }
  else {
    this->removeChildDragger("rotator");
    this->removeChildDragger("scaler");
    this->removeChildDragger("translator");
    if (this->rotFieldSensor->getAttachedField() != NULL) {
      this->rotFieldSensor->detach();
    }
    if (this->translFieldSensor->getAttachedField() != NULL) {
      this->translFieldSensor->detach();
    }
    if (this->scaleFieldSensor->getAttachedField() != NULL) {
      this->scaleFieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoJackDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoJackDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoJackDragger *thisp = (SoJackDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoJackDragger::valueChangedCB(void *, SoDragger * d)
{
  SoJackDragger *thisp = (SoJackDragger*)d;
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

/*!
  Used to invalidate the \e surroundScale and \e antiSquish parts when a child
  dragger starts or finishes dragging.
*/
void
SoJackDragger::invalidateSurroundScaleCB(void * f, SoDragger * d)
{
  SoJackDragger *thisp = (SoJackDragger*) f;
  SoSurroundScale *surround = SO_CHECK_PART(thisp, "surroundScale", SoSurroundScale);
  if (surround) {
    surround->invalidate();
    SoAntiSquish *squish = SO_CHECK_PART(thisp, "antiSquish", SoAntiSquish);
    if (squish) squish->recalc();
  }
}

//
// convenience method that takes care of callbacks before registering child
//
void
SoJackDragger::addChildDragger(SoDragger *child)
{
  child->addStartCallback(SoJackDragger::invalidateSurroundScaleCB, this);
  child->addFinishCallback(SoJackDragger::invalidateSurroundScaleCB, this);
  this->registerChildDragger(child);
}

//
// convenience method that removes callbacks before unregistering child
//
void
SoJackDragger::removeChildDragger(const char *childname)
{
  SoDragger *child = (SoDragger*) this->getAnyPart(childname, FALSE);
  child->removeStartCallback(SoJackDragger::invalidateSurroundScaleCB, this);
  child->removeFinishCallback(SoJackDragger::invalidateSurroundScaleCB, this);
  this->unregisterChildDragger(child);
}
