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
  \class SoVRMLGeometry SoVRMLGeometry.h Inventor/VRMLnodes/SoVRMLGeometry.h
  \brief The SoVRMLGeometry class is a superclass for VRML shapes.
*/

#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoSearchAction.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLGeometry);

// Doc in parent
void
SoVRMLGeometry::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLGeometry, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLGeometry::SoVRMLGeometry(void)
  : childlist(NULL), childlistvalid(FALSE)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLGeometry);
  this->childlist = new SoChildList(this);
}

/*!
  Destructor.
*/
SoVRMLGeometry::~SoVRMLGeometry()
{
  delete this->childlist;
}

// Doc in parent
SbBool
SoVRMLGeometry::shouldGLRender(SoGLRenderAction * action)
{
  return inherited::shouldGLRender(action);
}

/*!
  Convenience method that updates the shape hints element.
*/
void
SoVRMLGeometry::setupShapeHints(SoState * state, const SbBool ccw, const SbBool solid)
{
  SoGLShapeHintsElement::forceSend(state, ccw, solid, !solid);
}

// Doc in parent
SoChildList *
SoVRMLGeometry::getChildren(void) const
{
  if (!this->childlistvalid) {
    SoVRMLGeometry * thisp = (SoVRMLGeometry*) this;
    if (thisp->childlist == NULL) thisp->childlist = new SoChildList(thisp);
    SoVRMLParent::updateChildList(thisp, *thisp->childlist);
    thisp->childlistvalid = TRUE;
  }
  return this->childlist;
}

// Doc in parent
void
SoVRMLGeometry::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound() || this->getChildren() == NULL) return;

  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
  else {
    this->getChildren()->traverse(action); // traverse all children
  }
}

// Doc in parent
void
SoVRMLGeometry::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f && f->getTypeId() == SoSFNode::getClassTypeId()) {
    this->childlistvalid = FALSE;
  }
  inherited::notify(list);
}

// Doc in parent
void
SoVRMLGeometry::copyContents(const SoFieldContainer * from,
                             SbBool copyConn)
{
  inherited::copyContents(from, copyConn);
  this->childlistvalid = FALSE;
  this->childlist->truncate(0);
}
