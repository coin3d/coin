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

#include <Inventor/manips/SoPointLightManip.h>

SO_NODE_SOURCE(SoPointLightManip);


void
SoPointLightManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPointLightManip);
}

SoPointLightManip::SoPointLightManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPointLightManip);

  // FIXME: stuff missing? 20000108 mortene.

  COIN_STUB();
}


SoPointLightManip::~SoPointLightManip()
{
  COIN_STUB();
}

void
SoPointLightManip::setDragger(SoDragger * newdragger)
{
  COIN_STUB();
}

SoDragger *
SoPointLightManip::getDragger(void)
{
  COIN_STUB();
  return NULL;
}

SbBool
SoPointLightManip::replaceNode(SoPath * p)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoPointLightManip::replaceManip(SoPath * p, SoPointLight * newone) const
{
  COIN_STUB();
  return FALSE;
}

void
SoPointLightManip::doAction(SoAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::callback(SoCallbackAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::handleEvent(SoHandleEventAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::pick(SoPickAction * action)
{
  COIN_STUB();
}

void
SoPointLightManip::search(SoSearchAction * action)
{
  COIN_STUB();
}

SoChildList *
SoPointLightManip::getChildren(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoPointLightManip::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoPointLightManip::fieldSensorCB(void * f, SoSensor * d)
{
  COIN_STUB();
}

void
SoPointLightManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  COIN_STUB();
}

void
SoPointLightManip::transferFieldValues(const SoPointLight * from, SoPointLight * to)
{
  COIN_STUB();
}
