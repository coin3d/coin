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

#include <Inventor/draggers/SoTrackballDragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSwitch.h>


SO_KIT_SOURCE(SoTrackballDragger);


void
SoTrackballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTrackballDragger);
}

SoTrackballDragger::SoTrackballDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTrackballDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, XRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotatorSwitch, SoSwitch, TRUE, geomSeparator, YRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotator, SoSeparator, TRUE, XRotatorSwitch, XRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(XRotatorActive, SoSeparator, TRUE, XRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotatorSwitch, SoSwitch, TRUE, geomSeparator, ZRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotator, SoSeparator, TRUE, YRotatorSwitch, YRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(YRotatorActive, SoSeparator, TRUE, YRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotatorSwitch, SoSwitch, TRUE, geomSeparator, userAxisRotation, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotator, SoSeparator, TRUE, ZRotatorSwitch, ZRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(ZRotatorActive, SoSeparator, TRUE, ZRotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisRotation, SoRotation, TRUE, geomSeparator, userAxisSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisSwitch, SoSwitch, TRUE, geomSeparator, userRotatorSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxis, SoSeparator, TRUE, userAxisSwitch, userAxisActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userAxisActive, SoSeparator, TRUE, userAxisSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotatorSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotator, SoSeparator, TRUE, userRotatorSwitch, userRotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(userRotatorActive, SoSeparator, TRUE, userRotatorSwitch, "", TRUE);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoTrackballDragger::~SoTrackballDragger()
{
  COIN_STUB();
}

SbBool
SoTrackballDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoTrackballDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoTrackballDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoTrackballDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

SbBool
SoTrackballDragger::isAnimationEnabled(void)
{
  COIN_STUB();
  return FALSE;
}

void
SoTrackballDragger::setAnimationEnabled(SbBool newval)
{
  COIN_STUB();
}

void
SoTrackballDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoTrackballDragger::drag(void)
{
  COIN_STUB();
}

void
SoTrackballDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoTrackballDragger::setAllPartsActive(SbBool onoroff)
{
  COIN_STUB();
}

void
SoTrackballDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTrackballDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTrackballDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTrackballDragger::metaKeyChangeCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
