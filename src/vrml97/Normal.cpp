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
  \class SoVRMLNormal SoVRMLNormal.h Inventor/VRMLnodes/SoVRMLNormal.h
  \brief The SoVRMLNormal class is used to bind normals to geometry.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Normal {
    exposedField MFVec3f vector  []   # (-,)
  }
  \endverbatim

  This node defines a set of 3D surface normal vectors to be used in
  the vector field of some geometry nodes (e.g., SoVRMLIndexedFaceSet
  and SoVRMLElevationGrid). This node contains one multiple-valued
  field that contains the normal vectors. Normals shall be of unit
  length.
*/

/*!
  \var SoMFVec3f SoVRMLNormal::vector
  The normal vectors. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/elements/SoNormalElement.h>

SO_NODE_SOURCE(SoVRMLNormal);

// Doc in parent
void
SoVRMLNormal::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLNormal, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLNormal::SoVRMLNormal(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLNormal);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(vector);
}

/*!
  Destructor.
*/
SoVRMLNormal::~SoVRMLNormal()
{
}

// Doc in parent
void
SoVRMLNormal::doAction(SoAction * action)
{
  SoNormalElement::set(action->getState(), this,
                       this->vector.getNum(), this->vector.getValues(0));
}

// Doc in parent
void
SoVRMLNormal::GLRender(SoGLRenderAction * action)
{
  //
  // FIXME: code to test if all normals are unit length, and store
  // this in some cached variable.  should be passed on to
  // SoGLNormalizeElement to optimize rendering (pederb)
  //
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::callback(SoCallbackAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::pick(SoPickAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}
