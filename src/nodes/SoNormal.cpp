/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoNormal SoNormal.h Inventor/nodes/SoNormal.h
  \brief The SoNormal class is a node for providing normals to the state.
  \ingroup nodes

  Coin will automatically calculate normals for you if no SoNormal
  nodes are present in the scene graph, but explicitly setting normals
  is useful for at least two purposes: 1) a potential increase in
  performance, 2) you can calculate and use "incorrect" normals to do
  various special effects.

  \sa SoNormalBinding
*/

#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \var SoMFVec3f SoNormal::vector
  Sets a pool of normal vectors in the traversal state.
*/

// *************************************************************************

SO_NODE_SOURCE(SoNormal);

/*!
  Constructor.
*/
SoNormal::SoNormal(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNormal);

  SO_NODE_ADD_FIELD(vector, (NULL));
}

/*!
  Destructor.
*/
SoNormal::~SoNormal()
{
}

// Doc in superclass.
void
SoNormal::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNormal);

  SO_ENABLE(SoCallbackAction, SoNormalElement);
  SO_ENABLE(SoGLRenderAction, SoNormalElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoNormalElement);
  SO_ENABLE(SoPickAction, SoNormalElement);
}

// Doc in superclass.
void
SoNormal::GLRender(SoGLRenderAction * action)
{
  //
  // FIXME: code to test if all normals are unit length, and store
  // this in some cached variable.  should be passed on to
  // SoGLNormalizeElement to optimize rendering (pederb)
  //
  SoNormal::doAction(action);
}

// Doc in superclass.
void
SoNormal::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (!this->vector.isIgnored() &&
      !SoOverrideElement::getNormalVectorOverride(state)) {
    SoNormalElement::set(state, this,
                         this->vector.getNum(), this->vector.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setNormalVectorOverride(state, this, TRUE);
    }
  }
}

// Doc in superclass.
void
SoNormal::callback(SoCallbackAction * action)
{
  SoNormal::doAction(action);
}

// Doc in superclass.
void
SoNormal::pick(SoPickAction * action)
{
  SoNormal::doAction(action);
}

// Doc in superclass.
void
SoNormal::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoNormal::doAction(action);
}
