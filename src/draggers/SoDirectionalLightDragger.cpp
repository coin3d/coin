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

#include <Inventor/draggers/SoDirectionalLightDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/directionalLightDragger.h>


SO_KIT_SOURCE(SoDirectionalLightDragger);


void
SoDirectionalLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDirectionalLightDragger);
}

SoDirectionalLightDragger::SoDirectionalLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDirectionalLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translatorSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, translatorSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorRotInv, SoRotation, TRUE, translatorSep, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotator, FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("directionalLightDragger.iv",
                                       DIRECTIONALLIGHTDRAGGER_draggergeometry,
                                       sizeof(DIRECTIONALLIGHTDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_INIT_INSTANCE();

  SoDragger *pdragger = SO_GET_ANY_PART(this, "translator", SoDragPointDragger);
  assert(pdragger);
  SoDragger *sdragger = SO_GET_ANY_PART(this, "rotator", SoDragPointDragger);
  assert(sdragger);

  this->setPartAsDefault("material", "directionalLightOverallMaterial");

  this->addValueChangedCallback(SoDirectionalLightDragger::valueChangedCB);

  this->rotFieldSensor = new SoFieldSensor(SoDirectionalLightDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoDirectionalLightDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}


SoDirectionalLightDragger::~SoDirectionalLightDragger()
{
  delete this->translFieldSensor;
  delete this->rotFieldSensor;
}

SbBool
SoDirectionalLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *rotator = (SoDragger*) this->getAnyPart("rotator", FALSE);
    rotator->setPartAsDefault("rotator", "directionalLightRotatorRotator");
    rotator->setPartAsDefault("rotatorActive",
                              "directionalLightRotatorRotatorActive");
    rotator->setPartAsDefault("feedback",
                              "directionalLightRotatorFeedback");
    rotator->setPartAsDefault("feedbackActive",
                              "directionalLightRotatorFeedbackActive");

    SoDragger *translator = (SoDragger*) this->getAnyPart("translator", FALSE);
    translator->setPartAsDefault("yzTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xzTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xyTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("yzTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xzTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xyTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("yTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("zTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("xTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("yTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("zTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");

    this->registerChildDragger(rotator);
    this->registerChildDragger(translator);

    if (this->translFieldSensor->getAttachedField() != &this->translation)
      this->translFieldSensor->attach(&this->translation);
    if (this->rotFieldSensor->getAttachedField() != &this->rotation)
      this->rotFieldSensor->attach(&this->rotation);
  }
  else {
    SoDragger *translator = (SoDragger*) this->getAnyPart("translator", FALSE);
    this->unregisterChildDragger(translator);
    SoDragger *rotator = (SoDragger*) this->getAnyPart("rotator", FALSE);
    this->unregisterChildDragger(rotator);

    if (this->rotFieldSensor->getAttachedField() != NULL)
      this->rotFieldSensor->detach();
    if (this->translFieldSensor->getAttachedField() != NULL)
      this->translFieldSensor->detach();

    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoDirectionalLightDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
  inherited::setDefaultOnNonWritingFields();
}

void
SoDirectionalLightDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoDirectionalLightDragger *thisp = (SoDirectionalLightDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoDirectionalLightDragger::valueChangedCB(void *, SoDragger *d)
{
  SoDirectionalLightDragger *thisp = (SoDirectionalLightDragger*)d;
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

  SoRotation *invRot = SO_GET_ANY_PART(thisp, "translatorRotInv", SoRotation);
  invRot->rotation = rot.inverse();
}
