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

#include <Inventor/manips/SoTransformManip.h>
#include <coindefs.h> // COIN_STUB()

SO_NODE_SOURCE(SoTransformManip);


void
SoTransformManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformManip);
}

SoTransformManip::SoTransformManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformManip);

  // FIXME: stuff missing? 20000108 mortene.

  COIN_STUB();
}


SoTransformManip::~SoTransformManip()
{
  COIN_STUB();
}

void
SoTransformManip::setDragger(SoDragger * newdragger)
{
  COIN_STUB();
}

SoDragger *
SoTransformManip::getDragger(void)
{
  COIN_STUB();
  return NULL;
}

SbBool
SoTransformManip::replaceNode(SoPath * p)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoTransformManip::replaceManip(SoPath * p, SoTransform * newone) const
{
  COIN_STUB();
  return FALSE;
}

void
SoTransformManip::doAction(SoAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::callback(SoCallbackAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::handleEvent(SoHandleEventAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::pick(SoPickAction * action)
{
  COIN_STUB();
}

void
SoTransformManip::search(SoSearchAction * action)
{
  COIN_STUB();
}

SoChildList *
SoTransformManip::getChildren(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoTransformManip::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoTransformManip::fieldSensorCB(void * f, SoSensor * d)
{
  COIN_STUB();
}

void
SoTransformManip::copyContents(const SoFieldContainer * fromfc, SbBool copyconnections)
{
  COIN_STUB();
}

void
SoTransformManip::transferFieldValues(const SoTransform * from, SoTransform * to)
{
  COIN_STUB();
}
