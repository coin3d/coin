/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLVertexLine SoVRMLVertexLine.h Inventor/VRMLnodes/SovRMLVertexLine.h
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

#include <stddef.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include "nodes/SoSubNodeP.h"

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
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLVertexLine);

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
  inherited::callback(action);
}

// Doc in parent
void
SoVRMLVertexLine::pick(SoPickAction * action)
{
  inherited::pick(action);
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

#endif // HAVE_VRML97
