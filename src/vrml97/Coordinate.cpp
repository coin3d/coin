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
  \class SoVRMLCoordinate SoVRMLCoordinate.h Inventor/VRMLnodes/SoVRMLCoorinate.h
  \brief The SoVRMLCoordinate class is used define 3D coordinate for shapes.
*/

/*!
  \var SoMFVec3f SoVRMLCoordinate:: point
  The coordinates. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/actions/SoAction.h>

SO_NODE_SOURCE(SoVRMLCoordinate);

// Doc in parent
void
SoVRMLCoordinate::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCoordinate, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCoordinate::SoVRMLCoordinate(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLCoordinate);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(point);
}

/*!
  Destructor.
*/
SoVRMLCoordinate::~SoVRMLCoordinate()
{
}

// Doc in parent
void
SoVRMLCoordinate::doAction(SoAction * action)
{
  SoCoordinateElement::set3(action->getState(), this,
                            point.getNum(), point.getValues(0));
}

// Doc in parent
void
SoVRMLCoordinate::GLRender(SoGLRenderAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLCoordinate::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLCoordinate::callback(SoCallbackAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLCoordinate::pick(SoPickAction * action)
{
  SoVRMLCoordinate::doAction((SoAction*) action);
}
