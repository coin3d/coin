/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#include <Inventor/draggers/SoPointLightDragger.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/nodes/SoMaterial.h>


SO_KIT_SOURCE(SoPointLightDragger);


void
SoPointLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoPointLightDragger);
}

SoPointLightDragger::SoPointLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoPointLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, topSeparator, geomSeparator, TRUE);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));

  SO_KIT_INIT_INSTANCE();
}


SoPointLightDragger::~SoPointLightDragger()
{
  COIN_STUB();
}

SbBool
SoPointLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoPointLightDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoPointLightDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoPointLightDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}
