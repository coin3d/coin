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

#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>


SO_KIT_SOURCE(SoCenterballDragger);


void
SoCenterballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoCenterballDragger);
}

SoCenterballDragger::SoCenterballDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoCenterballDragger);

  SO_KIT_ADD_CATALOG_ENTRY(XAxis, SoSeparator, TRUE, XAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XAxisSwitch, SoSwitch, TRUE, geomSeparator, YAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(XCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rot2X90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotator, SoRotateCylindricalDragger, TRUE, topSeparator, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YAxis, SoSeparator, TRUE, YAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YAxisSwitch, SoSwitch, TRUE, geomSeparator, ZAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(YCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rotY90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotator, SoRotateCylindricalDragger, TRUE, topSeparator, ZCenterChanger, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZAxis, SoSeparator, TRUE, ZAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZAxisSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(ZCenterChanger, SoTranslate2Dragger, TRUE, topSeparator, rotX90, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotator, SoRotateCylindricalDragger, TRUE, topSeparator, YCenterChanger, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, lightModel, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(lightModel, SoLightModel, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rot2X90, SoRotation, TRUE, topSeparator, XRotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotX90, SoRotation, TRUE, topSeparator, ZRotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotY90, SoRotation, TRUE, topSeparator, XCenterChanger, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, YRotator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translateToCenter, SoMatrixTransform, TRUE, topSeparator, surroundScale, TRUE);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();
}


SoCenterballDragger::~SoCenterballDragger()
{
  COIN_STUB();
}

void
SoCenterballDragger::saveStartParameters(void)
{
  COIN_STUB();
}

void
SoCenterballDragger::transferCenterDraggerMotion(SoDragger * childdragger)
{
  COIN_STUB();
}

void
SoCenterballDragger::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoCenterballDragger::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoCenterballDragger::setSwitches(SoDragger * activechild)
{
  COIN_STUB();
}

SbBool
SoCenterballDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoCenterballDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoCenterballDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoCenterballDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoCenterballDragger::kidStartCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoCenterballDragger::kidFinishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
