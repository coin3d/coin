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

#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>


SO_KIT_SOURCE(SoRotateCylindricalDragger);


void
SoRotateCylindricalDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoRotateCylindricalDragger);
}

SoRotateCylindricalDragger::SoRotateCylindricalDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoRotateCylindricalDragger);

  SO_KIT_ADD_CATALOG_ENTRY(rotatorSwitch, SoSwitch, TRUE, geomSeparator, feedbackSwitch, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoSeparator, TRUE, rotatorSwitch, rotatorActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotatorActive, SoSeparator, TRUE, rotatorSwitch, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackSwitch, SoSwitch, TRUE, geomSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(feedback, SoSeparator, TRUE, feedbackSwitch, feedbackActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(feedbackActive, SoSeparator, TRUE, feedbackSwitch, "", TRUE);

  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));

  SO_KIT_INIT_INSTANCE();
}


SoRotateCylindricalDragger::~SoRotateCylindricalDragger()
{
  COIN_STUB();
}

SbBool
SoRotateCylindricalDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  COIN_STUB();
  return FALSE;
}

void
SoRotateCylindricalDragger::fieldSensorCB(void * f, SoSensor * s)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::setProjector(SbCylinderProjector * p)
{
  COIN_STUB();
}

const SbCylinderProjector *
SoRotateCylindricalDragger::getProjector(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoRotateCylindricalDragger::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::startCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::motionCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::doneCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::dragStart(void)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::drag(void)
{
  COIN_STUB();
}

void
SoRotateCylindricalDragger::dragFinish(void)
{
  COIN_STUB();
}
