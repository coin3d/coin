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
  \class SoVRMLTextureCoordinate SoVRMLTextureCoordinate.h Inventor/VRMLnodes/SoVRMLTextureCoordinate.h
  \brief The SoVRMLTextureCoordinate class binds texture coordinates to vertex-based geometry.
*/

/*!
  \var SoMFVec2f SoVRMLTextureCoordinate::point
  The texture coordinates. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLTextureCoordinate.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>

SO_NODE_SOURCE(SoVRMLTextureCoordinate);

// Doc in parent
void
SoVRMLTextureCoordinate::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLTextureCoordinate, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLTextureCoordinate::SoVRMLTextureCoordinate(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLTextureCoordinate);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(point);
}

/*!
  Destructor.
*/
SoVRMLTextureCoordinate::~SoVRMLTextureCoordinate()
{
}

// Doc in parent
void
SoVRMLTextureCoordinate::doAction(SoAction * action)
{
  SoTextureCoordinateElement::set2(action->getState(), this,
                                   point.getNum(),
                                   point.getValues(0));
}

// Doc in parent
void
SoVRMLTextureCoordinate::callback(SoCallbackAction * action)
{
  SoVRMLTextureCoordinate::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLTextureCoordinate::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
                                          this, NULL);
  SoVRMLTextureCoordinate::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLTextureCoordinate::pick(SoPickAction * action)
{
  SoVRMLTextureCoordinate::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLTextureCoordinate::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLTextureCoordinate::doAction((SoAction*)action);
}
