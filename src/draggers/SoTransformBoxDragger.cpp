/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>


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

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoTransformBoxDragger::~SoTransformBoxDragger()
{
  COIN_STUB();
}

SbBool
SoTransformBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoTransformBoxDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoTransformBoxDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoTransformBoxDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTransformBoxDragger::invalidateSurroundScaleCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
