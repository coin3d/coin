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

#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoSurroundScale.h>


SO_KIT_SOURCE(SoJackDragger);


void
SoJackDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoJackDragger);
}

SoJackDragger::SoJackDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoJackDragger);

  SO_KIT_ADD_CATALOG_ENTRY(surroundScale, SoSurroundScale, TRUE, topSeparator, antiSquish, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(antiSquish, SoAntiSquish, FALSE, topSeparator, scaler, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(scaler, SoScaleUniformDragger, TRUE, topSeparator, rotator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, topSeparator, geomSeparator, TRUE);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoJackDragger::~SoJackDragger()
{
  COIN_STUB();
}

SbBool
SoJackDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoJackDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoJackDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoJackDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoJackDragger::invalidateSurroundScaleCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
