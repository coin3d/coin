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

#include <Inventor/draggers/SoPointLightDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h> // COIN_STUB()

#include <data/draggerDefaults/pointLightDragger.h>


SO_KIT_SOURCE(SoPointLightDragger);


void
SoPointLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoPointLightDragger);
}

SoPointLightDragger::SoPointLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoPointLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, topSeparator, geomSeparator, TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("pointLightDragger.iv",
                                       POINTLIGHTDRAGGER_draggergeometry,
                                       sizeof(POINTLIGHTDRAGGER_draggergeometry));
  }

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_INIT_INSTANCE();

  SoDragger *pdragger = SO_GET_ANY_PART(this, "translator", SoDragPointDragger);
  assert(pdragger);

  this->setPartAsDefault("material", "pointLightOverallMaterial");

  this->addValueChangedCallback(SoPointLightDragger::valueChangedCB);
  this->fieldSensor = new SoFieldSensor(SoPointLightDragger::fieldSensorCB, this);
  this->fieldSensor->setPriority(0);
  this->setUpConnections(TRUE, TRUE);
}


SoPointLightDragger::~SoPointLightDragger()
{
  delete this->fieldSensor;
}

SbBool
SoPointLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *child = (SoDragger*) this->getAnyPart("translator", FALSE);
    child->setPartAsDefault("yzTranslator.translator",
                            "pointLightTranslatorPlaneTranslator");
    child->setPartAsDefault("xzTranslator.translator",
                            "pointLightTranslatorPlaneTranslator");
    child->setPartAsDefault("xyTranslator.translator",
                            "pointLightTranslatorPlaneTranslator");

    child->setPartAsDefault("yzTranslator.translatorActive",
                            "pointLightTranslatorPlaneTranslatorActive");
    child->setPartAsDefault("xzTranslator.translatorActive",
                            "pointLightTranslatorPlaneTranslatorActive");
    child->setPartAsDefault("xyTranslator.translatorActive",
                            "pointLightTranslatorPlaneTranslatorActive");

    child->setPartAsDefault("xTranslator.translator",
                            "pointLightTranslatorLineTranslator");
    child->setPartAsDefault("yTranslator.translator",
                            "pointLightTranslatorLineTranslator");
    child->setPartAsDefault("zTranslator.translator",
                            "pointLightTranslatorLineTranslator");

    child->setPartAsDefault("xTranslator.translatorActive",
                            "pointLightTranslatorLineTranslatorActive");
    child->setPartAsDefault("yTranslator.translatorActive",
                            "pointLightTranslatorLineTranslatorActive");
    child->setPartAsDefault("zTranslator.translatorActive",
                            "pointLightTranslatorLineTranslatorActive");
    this->registerChildDragger(child);
    if (this->fieldSensor->getAttachedField() != &this->translation) {
      this->fieldSensor->attach(&this->translation);
    }
  }
  else {
    SoDragger *child = (SoDragger*) this->getAnyPart("translator", FALSE);
    this->unregisterChildDragger(child);
    if (this->fieldSensor->getAttachedField() != NULL) {
      this->fieldSensor->detach();
    }
    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

void
SoPointLightDragger::setDefaultOnNonWritingFields(void)
{
  // FIXME: not sure if I should call setDefault() for any fields here...
  // pederb, 20000203
  COIN_STUB();
}

void
SoPointLightDragger::fieldSensorCB(void *d, SoSensor *)
{
  SoPointLightDragger *thisp = (SoPointLightDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

void
SoPointLightDragger::valueChangedCB(void *, SoDragger * d)
{
  SoPointLightDragger *thisp = (SoPointLightDragger*)d;

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
