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

#include <Inventor/draggers/SoScale2UniformDragger.h>


SO_KIT_SOURCE(SoScale2UniformDragger);


void
SoScale2UniformDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoScale2UniformDragger);
}

SoScale2UniformDragger::SoScale2UniformDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoScale2UniformDragger);

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


SoScale2UniformDragger::~SoScale2UniformDragger()
{
  COIN_STUB();
}

SbBool
SoScale2UniformDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoScale2UniformDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::finishCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::drag(void)
{
  COIN_STUB();
}

void
SoScale2UniformDragger::dragFinish(void)
{
  COIN_STUB();
}
