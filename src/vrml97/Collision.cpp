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
  \class SoVRMLCollision SoVRMLCollision.h Inventor/VRMLnodes/SoVRMLCollision.h
  \brief The SoVRMLCollision class is used for collision detection with the avatar.
*/

/*!
  \var SoSFBool SoVRMLCollision::collide
  Enable/disable collision.
*/

/*!
  \var SoSFNode SoVRMLCollision::proxy
  Proxy node(s) used for collision testing.
*/

/*!
  \var SoSFTime SoVRMLCollision::collideTime
  
  An eventOut sent for each collision that occurs.
*/

#include <Inventor/VRMLnodes/SoVRMLCollision.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/elements/SoCacheElement.h>

SO_NODE_SOURCE(SoVRMLCollision);

// Doc in parent
void
SoVRMLCollision::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCollision, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCollision::SoVRMLCollision(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLCollision);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(collide, (TRUE));
  SO_VRMLNODE_ADD_FIELD(proxy, (NULL));

  SO_VRMLNODE_ADD_EVENT_OUT(collideTime);
}

/*!
  Constructor. \a numchildren is the expected number of children.
*/
SoVRMLCollision::SoVRMLCollision(int numchildren)
{
}

/*!
  Destructor.
*/
SoVRMLCollision::~SoVRMLCollision() // virtual, protected
{
}

// Doc in parent
void
SoVRMLCollision::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();

  int numindices;
  const int * indices;
  SoAction::PathCode pathcode = action->getPathCode(numindices, indices);

  SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();

  if (pathcode == SoAction::IN_PATH) {
    int lastchild = indices[numindices - 1];
    for (int i = 0; i <= lastchild && !action->hasTerminated(); i++) {
      SoNode * child = childarray[i];
      action->pushCurPath(i, child);
      if (action->getCurPathCode() != SoAction::OFF_PATH ||
          child->affectsState()) {
        if (!action->abortNow()) {
          child->GLRender(action);
        }
        else {
          SoCacheElement::invalidate(state);
        }
      }
      action->popCurPath(pathcode);
    }
  }
  else {
    action->pushCurPath();
    int n = this->getChildren()->getLength();
    for (int i = 0; i < n && !action->hasTerminated(); i++) {
      if (action->abortNow()) {
        // only cache if we do a full traversal
        SoCacheElement::invalidate(state);
        break;
      }
      action->popPushCurPath(i, childarray[i]);
      childarray[i]->GLRender(action);
    }
    action->popCurPath();
  }
  state->pop();
}

// Doc in parent
void
SoVRMLCollision::notify(SoNotList * list)
{
  inherited::notify(list);
}
