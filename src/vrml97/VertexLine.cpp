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
  \class SoVRMLVertexLine SovRMLVertexLine.h Inventor/VRMLnodes/SovRMLVertexLine.h
  \brief The SoVRMLVertexLine class is a superclass for line based VRML geometry.
*/

/*!
  \var SoSFNode SoVRMLVertexLine::coord
  Should contain an SoVRMLCoordinate node.
*/

/*!
  \var SoSFNode SoVRMLVertexLine::color
  Can contain an SoVRMLColor node if multiple colors are wanted.
*/

/*!
  \var SoSFBool SoVRMLVertexLine::colorPerVertex

  When TRUE, colors will be bound per vertex, otherwise per
  line. Default value is TRUE.
  
*/

#include <Inventor/VRMLnodes/SoVRMLVertexLine.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <stddef.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLVertexLine);

// Doc in parent
void
SoVRMLVertexLine::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLVertexLine, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLVertexLine::SoVRMLVertexLine(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLVertexLine);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(coord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(color, (NULL));
  SO_VRMLNODE_ADD_FIELD(colorPerVertex, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLVertexLine::~SoVRMLVertexLine()
{
}

// Doc in parent
void
SoVRMLVertexLine::doAction(SoAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->doAction(action);

  node = this->color.getValue();
  if (node) node->doAction(action);
}

// Doc in parent
void
SoVRMLVertexLine::GLRender(SoGLRenderAction * action)
{
  SoNode * node;

  node = this->coord.getValue();
  if (node) node->GLRender(action);

  node = this->color.getValue();
  if (node) node->GLRender(action);

}

// Doc in parent
void
SoVRMLVertexLine::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
}

// Doc in parent
void
SoVRMLVertexLine::callback(SoCallbackAction * action)
{
  SoVRMLVertexLine::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLVertexLine::pick(SoPickAction * action)
{
  SoVRMLVertexLine::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLVertexLine::notify(SoNotList * list)
{
  inherited::notify(list);
}

// Doc in parent
SbBool
SoVRMLVertexLine::shouldGLRender(SoGLRenderAction * action)
{
  if (this->coord.getValue() == NULL) return FALSE;
  return inherited::shouldGLRender(action);
}




