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
  \class SoVRMLNormal SoVRMLNormal.h Inventor/VRMLnodes/SoVRMLNormal.h
  \brief The SoVRMLNormal class is used to bind normals to geometry.
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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLNormal);

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
SoVRMLNormal::pick(SoRayPickAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

