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

#include <Inventor/manips/SoSpotLightManip.h>

SO_NODE_SOURCE(SoSpotLightManip);


void
SoSpotLightManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSpotLightManip);
}

SoSpotLightManip::SoSpotLightManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSpotLightManip);

  // FIXME: stuff missing? 20000108 mortene.

  COIN_STUB();
}


SoSpotLightManip::~SoSpotLightManip()
{
  COIN_STUB();
}

void
SoSpotLightManip::setDragger(SoDragger * newdragger)
{
  COIN_STUB();
}

SoDragger *
SoSpotLightManip::getDragger(void)
{
  COIN_STUB();
  return NULL;
}

SbBool
SoSpotLightManip::replaceNode(SoPath * p)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoSpotLightManip::replaceManip(SoPath * p, SoSpotLight * newone) const
{
  COIN_STUB();
  return FALSE;
}

void
SoSpotLightManip::doAction(SoAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::callback(SoCallbackAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::handleEvent(SoHandleEventAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::pick(SoPickAction * action)
{
  COIN_STUB();
}

void
SoSpotLightManip::search(SoSearchAction * action)
{
  COIN_STUB();
}

SoChildList *
SoSpotLightManip::getChildren(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoSpotLightManip::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoSpotLightManip::fieldSensorCB(void * f, SoSensor * d)
{
  COIN_STUB();
}

void
SoSpotLightManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  COIN_STUB();
}

void
SoSpotLightManip::transferFieldValues(const SoSpotLight * from, SoSpotLight * to)
{
  COIN_STUB();
}
