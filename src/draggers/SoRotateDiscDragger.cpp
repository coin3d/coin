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

#include <Inventor/draggers/SoRotateDiscDragger.h>


SO_KIT_SOURCE(SoRotateDiscDragger);


void
SoRotateDiscDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateDiscDragger);
}

SoRotateDiscDragger::SoRotateDiscDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateDiscDragger);

  // FIXME: SO_KIT_ADD_CATALOG_ENTRY for all entries in this dragger's
  // catalog. 20000107 mortene.

  // FIXME: should the fields of this class be added to the field list
  // with SO_NODE_ADD_FIELD()? 20000107 mortene.

  SO_KIT_INIT_INSTANCE();
}


SoRotateDiscDragger::~SoRotateDiscDragger()
{
  COIN_STUB();
}

SbBool
SoRotateDiscDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoRotateDiscDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoRotateDiscDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoRotateDiscDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
