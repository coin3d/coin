/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLVertexPoint SoVRMLVertexPoint.h Inventor/VRMLnodes/SoVRMLVertexPoint.h
  \brief The SoVRMLVertexPoint class is a superclass for point based VRML shapes.
*/

/*!
  \var SoSFNode SoVRMLVertexPoint::coord
  Should contain an SoVRMLCoordinate node.
*/

/*!
  \var SoSFNode SoVRMLVertexPoint::color
  Can contain an SoVRMLColor node when color per point is needed.
*/

#include <Inventor/VRMLnodes/SoVRMLVertexPoint.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <stddef.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLVertexPoint);

// Doc in parent
void
SoVRMLVertexPoint::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLVertexPoint, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLVertexPoint::SoVRMLVertexPoint(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLVertexPoint);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(coord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(color, (NULL));
}

/*!
  Destructor.
*/
SoVRMLVertexPoint::~SoVRMLVertexPoint()
{
}

// Doc in parent
void
SoVRMLVertexPoint::doAction(SoAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->doAction(action);

  node = this->color.getValue();
  if (node) node->doAction(action);
}

// Doc in parent
void
SoVRMLVertexPoint::GLRender(SoGLRenderAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->GLRender(action);

  node = this->color.getValue();
  if (node) node->GLRender(action);
}

// Doc in parent
void
SoVRMLVertexPoint::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
}

// Doc in parent
void
SoVRMLVertexPoint::callback(SoCallbackAction * action)
{
  SoVRMLVertexPoint::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLVertexPoint::pick(SoPickAction * action)
{
  SoVRMLVertexPoint::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLVertexPoint::notify(SoNotList * list)
{
  inherited::notify(list);
}

// Doc in parent
void
SoVRMLVertexPoint::computeBBox(SoAction * action, SbBox3f & box,
                               SbVec3f & center)
{
  SoVRMLCoordinate * node = (SoVRMLCoordinate*) this->coord.getValue();
  if (node == NULL) return;

  int num = node->point.getNum();
  const SbVec3f * coords = node->point.getValues(0);

  box.makeEmpty();
  while (num--) {
    box.extendBy(*coords++);
  }
  if (!box.isEmpty()) center = box.getCenter();
}

// Doc in parent
void
SoVRMLVertexPoint::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  SoVRMLCoordinate * c = (SoVRMLCoordinate*) this->coord.getValue();
  if (c) {
    action->addNumPoints(c->point.getNum());
  }
}

// Doc in parent
SbBool
SoVRMLVertexPoint::shouldGLRender(SoGLRenderAction * action)
{
  if (this->coord.getValue() == NULL) return FALSE;
  return inherited::shouldGLRender(action);
}
