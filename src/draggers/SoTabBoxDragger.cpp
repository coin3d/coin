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

#include <Inventor/draggers/SoTabBoxDragger.h>
#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoTransform.h>


SO_KIT_SOURCE(SoTabBoxDragger);


void
SoTabBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabBoxDragger);
}

SoTabBoxDragger::SoTabBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabBoxDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, tabPlane1Sep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1Sep, SoSeparator, FALSE, topSeparator, tabPlane2Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1Xf, SoTransform, TRUE, tabPlane1Sep, tabPlane1, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane1, SoTabPlaneDragger, TRUE, tabPlane1Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2Sep, SoSeparator, FALSE, topSeparator, tabPlane3Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2Xf, SoTransform, TRUE, tabPlane2Sep, tabPlane2, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane2, SoTabPlaneDragger, TRUE, tabPlane2Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3Sep, SoSeparator, FALSE, topSeparator, tabPlane4Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3Xf, SoTransform, TRUE, tabPlane3Sep, tabPlane3, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane3, SoTabPlaneDragger, TRUE, tabPlane3Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4Sep, SoSeparator, FALSE, topSeparator, tabPlane5Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4Xf, SoTransform, TRUE, tabPlane4Sep, tabPlane4, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane4, SoTabPlaneDragger, TRUE, tabPlane4Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5Sep, SoSeparator, FALSE, topSeparator, tabPlane6Sep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5Xf, SoTransform, TRUE, tabPlane5Sep, tabPlane5, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane5, SoTabPlaneDragger, TRUE, tabPlane5Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6Sep, SoSeparator, FALSE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6Xf, SoTransform, TRUE, tabPlane6Sep, tabPlane6, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(tabPlane6, SoTabPlaneDragger, TRUE, tabPlane6Sep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(boxGeom, SoSeparator, TRUE, geomSeparator, "", TRUE);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoTabBoxDragger::~SoTabBoxDragger()
{
  COIN_STUB();
}

SbBool
SoTabBoxDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoTabBoxDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoTabBoxDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoTabBoxDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabBoxDragger::invalidateSurroundScaleCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTabBoxDragger::adjustScaleTabSizeCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
