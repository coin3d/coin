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


SO_KIT_SOURCE(SoTrackballDragger);


void
SoTrackballDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoTrackballDragger);
}

SoTrackballDragger::SoTrackballDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoTrackballDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // XRotator
  // XRotatorActive
  // XRotatorSwitch
  // YRotator
  // YRotatorActive
  // YRotatorSwitch
  // ZRotator
  // ZRotatorActive
  // ZRotatorSwitch
  // antiSquish
  // rotator
  // rotatorActive
  // rotatorSwitch
  // surroundScale
  // userAxis
  // userAxisActive
  // userAxisRotation
  // userAxisSwitch
  // userRotator
  // userRotatorActive
  // userRotatorSwitch

  // FIXME: should the fields of this class be added to the field list
  // with SO_NODE_ADD_FIELD()? 20000107 mortene.

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
