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

#include <Inventor/draggers/SoSpotLightDragger.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>


SO_KIT_SOURCE(SoSpotLightDragger);


void
SoSpotLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoSpotLightDragger);
}

SoSpotLightDragger::SoSpotLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoSpotLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translatorSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorRotInv, SoRotation, TRUE, translatorSep, translator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, translatorSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, beamSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamSep, SoSeparator, TRUE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(beamPlacement, SoTranslation, TRUE, beamSep, beamScale, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamScale, SoScale, TRUE, beamSep, beamSwitch, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamSwitch, SoSwitch, TRUE, beamSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(beam, SoSeparator, TRUE, beamSwitch, beamActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamActive, SoSeparator, TRUE, beamSwitch, "", TRUE);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(angle, (1.0f));

  SO_KIT_INIT_INSTANCE();
}


SoSpotLightDragger::~SoSpotLightDragger()
{
  COIN_STUB();
}

SbBool
SoSpotLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoSpotLightDragger::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}

void
SoSpotLightDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoSpotLightDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoSpotLightDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoSpotLightDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoSpotLightDragger::doneCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoSpotLightDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoSpotLightDragger::drag(void)
{
  COIN_STUB();
}

void
SoSpotLightDragger::dragFinish(void)
{
  COIN_STUB();
}

void
SoSpotLightDragger::setBeamScaleFromAngle(float beamangle)
{
  COIN_STUB();
}
