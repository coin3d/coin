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
  \ingroup VRMLnodes

  WEB3DCOPYRIGHT

  \verbatim
  Collision {
    eventIn      MFNode   addChildren
    eventIn      MFNode   removeChildren
    exposedField MFNode   children        []
    exposedField SFBool   collide         TRUE
    field        SFVec3f  bboxCenter      0 0 0      # (-,)
    field        SFVec3f  bboxSize        -1 -1 -1   # (0,) or -1,-1,-1
    field        SFNode   proxy           NULL
    eventOut     SFTime   collideTime
  }
  \endverbatim

  The Collision node is a grouping node that specifies the collision
  detection properties for its children (and their descendants),
  specifies surrogate objects that replace its children during
  collision detection, and sends events signalling that a collision
  has occurred between the avatar and the Collision node's geometry or
  surrogate. By default, all geometric nodes in the scene are
  collidable with the viewer except IndexedLineSet, PointSet, and
  Text. Browsers shall detect geometric collisions between the avatar
  (see SoVRMLNavigationInfo) and the scene's geometry and prevent the
  avatar from 'entering' the geometry. See 4.13.4, Collision detection
  and terrain following
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.13.4),
  for general information on collision detection.

  If there are no Collision nodes specified in a VRML file, browsers
  shall detect collisions between the avatar and all objects during
  navigation.  

  Subclause 4.6.5, Grouping and children nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5),
  contains a description of the children, addChildren, and
  removeChildren fields and eventIns.

  The Collision node's collide field enables and disables collision
  detection. If collide is set to FALSE, the children and all
  descendants of the Collision node shall not be checked for
  collision, even though they are drawn. This includes any descendent
  Collision nodes that have collide set to TRUE (i.e., setting collide
  to FALSE turns collision off for every node below it). 

  Collision nodes with the collide field set to TRUE detect the
  nearest collision with their descendent geometry (or proxies). When
  the nearest collision is detected, the collided Collision node sends
  the time of the collision through its collideTime eventOut. If a
  Collision node contains a child, descendant, or proxy (see below)
  that is a Collision node, and both Collision nodes detect that a
  collision has occurred, both send a collideTime event at the same
  time. A collideTime event shall be generated if the avatar is
  colliding with collidable geometry when the Collision node is read
  from a VRML file or inserted into the transformation hierarchy.  

  The bboxCenter and bboxSize fields specify a bounding box that
  encloses the Collision node's children. This is a hint that may be
  used for optimization purposes. The results are undefined if the
  specified bounding box is smaller than the actual bounding box of
  the children at any time. A default bboxSize value, (-1, -1, -1),
  implies that the bounding box is not specified and if needed shall
  be calculated by the browser. More details on the bboxCenter and
  bboxSize fields can be found in 4.6.4, Bounding boxes.
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.4),

  The collision proxy, defined in the proxy field, is any legal
  children node as described in 4.6.5, Grouping and children nodes,
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5),
  that is used as a substitute for the Collision node's children
  during collision detection. The proxy is used strictly for collision
  detection; it is not drawn.  If the value of the collide field is
  TRUE and the proxy field is non-NULL, the proxy field defines the
  scene on which collision detection is performed. If the proxy value
  is NULL, collision detection is performed against the children of
  the Collision node.  If proxy is specified, any descendent children
  of the Collision node are ignored during collision detection. If
  children is empty, collide is TRUE, and proxy is specified,
  collision detection is performed against the proxy but nothing is
  displayed. In this manner, invisible collision objects may be
  supported.  

  The collideTime eventOut generates an event specifying the time when
  the avatar (see SoVRMLNavigationInfo) makes contact with the
  collidable children or proxy of the Collision node.  An ideal
  implementation computes the exact time of collision. Implementations
  may approximate the ideal by sampling the positions of collidable
  objects and the user. The SoVRMLNavigationInfo node contains additional
  information for parameters that control the avatar size.
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
