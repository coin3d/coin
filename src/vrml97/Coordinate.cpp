/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVRMLCoordinate SoVRMLCoordinate.h Inventor/VRMLnodes/SoVRMLCoordinate.h
  \brief The SoVRMLCoordinate class is used to define 3D coordinates for shapes.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim
  Coordinate { 
    exposedField MFVec3f point  []      # (-inf, inf) 
  }
  \endverbatim
 
  This node defines a set of 3D coordinates to be used in the coord
  field of vertex-based geometry nodes including SoVRMLIndexedFaceSet,
  SoVRMLIndexedLineSet, and SoVRMLPointSet.

*/

/*!
  \var SoMFVec3f SoVRMLCoordinate::point
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
