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

#include <Inventor/manips/SoClipPlaneManip.h>

SO_NODE_SOURCE(SoClipPlaneManip);


void
SoClipPlaneManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlaneManip);
}

SoClipPlaneManip::SoClipPlaneManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoClipPlaneManip);

  // FIXME: initialize field to correct value. 20000107 mortene.
  SO_NODE_ADD_FIELD(draggerPosition, (0.0f, 0.0f, 0.0f));


  // FIXME: stuff missing? 20000107 mortene.

  COIN_STUB();
}


SoClipPlaneManip::~SoClipPlaneManip()
{
  COIN_STUB();
}

void
SoClipPlaneManip::setDragger(SoDragger * newdragger)
{
  COIN_STUB();
}

SoDragger *
SoClipPlaneManip::getDragger(void)
{
  COIN_STUB();
  return NULL;
}

SoNode *
SoClipPlaneManip::copy(SbBool copyconnections) const
{
  COIN_STUB();
  return NULL;
}

void
SoClipPlaneManip::setValue(const SbBox3f & box, const SbVec3f & planenormal, float draggerscalefactor)
{
  COIN_STUB();
}

SbBool
SoClipPlaneManip::replaceNode(SoPath * p)
{
  COIN_STUB();
  return FALSE;
}

SbBool
SoClipPlaneManip::replaceManip(SoPath * p, SoClipPlane * newone) const
{
  COIN_STUB();
  return FALSE;
}

void
SoClipPlaneManip::doAction(SoAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::callback(SoCallbackAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::GLRender(SoGLRenderAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::getBoundingBox(SoGetBoundingBoxAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::getMatrix(SoGetMatrixAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::handleEvent(SoHandleEventAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::pick(SoPickAction * action)
{
  COIN_STUB();
}

void
SoClipPlaneManip::search(SoSearchAction * action)
{
  COIN_STUB();
}

SoChildList *
SoClipPlaneManip::getChildren(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoClipPlaneManip::valueChangedCB(void * f, SoDragger * d)
{
  COIN_STUB();
}

void
SoClipPlaneManip::fieldSensorCB(void * f, SoSensor * d)
{
  COIN_STUB();
}
