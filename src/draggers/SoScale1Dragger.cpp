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

#include <Inventor/draggers/SoScale1Dragger.h>


SO_KIT_SOURCE(SoScale1Dragger);


void
SoScale1Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale1Dragger);
}

SoScale1Dragger::SoScale1Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale1Dragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.
  // feedback
  // feedbackActive
  // feedbackSwitch
  // scaler
  // scalerActive
  // scalerSwitch

  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoScale1Dragger::~SoScale1Dragger()
{
  COIN_STUB();
}

SbBool
SoScale1Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoScale1Dragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoScale1Dragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale1Dragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale1Dragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale1Dragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale1Dragger::dragStart(void)
{
  COIN_STUB();
}

void
SoScale1Dragger::drag(void)
{
  COIN_STUB();
}

void
SoScale1Dragger::dragFinish(void)
{
  COIN_STUB();
}
