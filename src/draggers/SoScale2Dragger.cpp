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

#include <Inventor/draggers/SoScale2Dragger.h>


SO_KIT_SOURCE(SoScale2Dragger);


void
SoScale2Dragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale2Dragger);
}

SoScale2Dragger::SoScale2Dragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale2Dragger);

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


SoScale2Dragger::~SoScale2Dragger()
{
  COIN_STUB();
}

SbBool
SoScale2Dragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoScale2Dragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoScale2Dragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2Dragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2Dragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2Dragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2Dragger::dragStart(void)
{
  COIN_STUB();
}

void
SoScale2Dragger::drag(void)
{
  COIN_STUB();
}

void
SoScale2Dragger::dragFinish(void)
{
  COIN_STUB();
}
