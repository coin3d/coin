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

#include <Inventor/manips/SoDirectionalLightManip.h>

SO_NODE_SOURCE(SoDirectionalLightManip);


void
SoDirectionalLightManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoDirectionalLightManip);
}

SoDirectionalLightManip::SoDirectionalLightManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoDirectionalLightManip);

  // FIXME: stuff missing? 20000108 mortene.

  COIN_STUB();
}


SoDirectionalLightManip::~SoDirectionalLightManip()
{
  COIN_STUB();
}

void
SoDirectionalLightManip::setDragger(SoDragger * newdragger)
{
  COIN_STUB();
}

SoDragger *
SoDirectionalLightManip::getDragger(void)
{
  COIN_STUB();
  return NULL;
}

SbBool
SoDirectionalLightManip::replaceNode(SoPath * p)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoDirectionalLightManip::replaceManip(SoPath * p, SoDirectionalLight * newone) const
{
  COIN_STUB();
  return FALSE;
}

void
SoDirectionalLightManip::doAction(SoAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::callback(SoCallbackAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::handleEvent(SoHandleEventAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::pick(SoPickAction * action)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::search(SoSearchAction * action)
{
  COIN_STUB();
}

SoChildList *
SoDirectionalLightManip::getChildren(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoDirectionalLightManip::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::fieldSensorCB(void * f, SoSensor * d)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  COIN_STUB();
}

void
SoDirectionalLightManip::transferFieldValues(const SoDirectionalLight * from, SoDirectionalLight * to)
{
  COIN_STUB();
}
