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


SO_KIT_SOURCE(SoTabBoxDragger);


void
SoTabBoxDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTabBoxDragger);
}

SoTabBoxDragger::SoTabBoxDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTabBoxDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // boxGeom
  // surroundScale
  // tabPlane1
  // tabPlane1Sep
  // tabPlane1Xf
  // tabPlane2
  // tabPlane2Sep
  // tabPlane2Xf
  // tabPlane3
  // tabPlane3Sep
  // tabPlane3Xf
  // tabPlane4
  // tabPlane4Sep
  // tabPlane4Xf
  // tabPlane5
  // tabPlane5Sep
  // tabPlane5Xf
  // tabPlane6
  // tabPlane6Sep
  // tabPlane6Xf

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
