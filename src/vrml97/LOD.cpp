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
  \class SoVRMLLOD SoVRMLLOD.h Inventor/VRMLnodes/SoVRMLLOD.h
  \brief The SoVRMLLOD class is used to represent various levels of detail based on distance.
*/

/*!
  SoMFFloat SoVRMLLOD::range
  The range for each level.
*/

/*!
  SoSFVec3f SoVRMLLOD::center
  The center used when calculating distance.
*/

/*!
  SoMFNode SoVRMLLOD::level
  The different levels.
*/

#include <Inventor/VRMLnodes/SoVRMLLOD.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbMatrix.h>

SO_NODE_SOURCE(SoVRMLLOD);

// Doc in parent
void
SoVRMLLOD::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLLOD, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLLOD::SoVRMLLOD(void)
{
  this->commonConstructor();
}

/*!
  Destructor.
*/
SoVRMLLOD::~SoVRMLLOD() // virtual, protected
{
}

/*!
  Constructor. \a levels is the expected number of levels.
*/
SoVRMLLOD::SoVRMLLOD(int levels)
  : SoGroup(levels)
{
  this->commonConstructor();
}

void
SoVRMLLOD::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLLOD);

  SO_VRMLNODE_ADD_EMPTY_MFIELD(range);
  SO_VRMLNODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(level);
  this->childlistvalid = FALSE;
}

// Doc in parent
SbBool
SoVRMLLOD::affectsState(void) const // virtual
{
  return FALSE;
}

/*!
  Adds a new level.
*/
void
SoVRMLLOD::addLevel(SoNode * level)
{
  this->addChild(level);
}

/*!
  Inserts a new level.
*/
void
SoVRMLLOD::insertLevel(SoNode * level,
                       int idx)
{
  this->insertChild(level, idx);
}

/*!
  Returns a level.
*/
SoNode *
SoVRMLLOD::getLevel(int idx) const
{
  return this->getChild(idx);
}

/*!
  Find \a node, and return the level index or -1 if not found.
*/
int
SoVRMLLOD::findLevel(const SoNode * node) const
{
  return this->findChild(node);
}

/*!
  Return the number of levels.
*/
int
SoVRMLLOD::getNumLevels(void) const
{
  return this->level.getNum();
}

/*!
  Removes the level at index \a idx.
*/
void
SoVRMLLOD::removeLevel(int idx)
{
  this->removeChild(idx);
}

/*!
  Find \a level, and remove it if found.
*/
void
SoVRMLLOD::removeLevel(SoNode * level)
{
  this->removeChild(level);
}

/*!
  Remove all levels.
*/
void
SoVRMLLOD::removeAllLevels(void)
{
  this->removeAllChildren();
}

/*!
  Replace the level at \a idx with \a node.
*/
void
SoVRMLLOD::replaceLevel(int idx,
                        SoNode * node)
{
  this->replaceChild(idx, node);
}

/*!
  Find \a oldnode, and replace it with \a newnode.
*/
void
SoVRMLLOD::replaceLevel(SoNode * oldnode,
                        SoNode * newnode)
{
  this->replaceChild(oldnode, newnode);
}

// Doc in parent
void
SoVRMLLOD::doAction(SoAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
  else {
    int idx = this->whichToTraverse(action);;
    if (idx >= 0) this->getChildren()->traverse(action, idx);
  }
}

// Doc in parent
void
SoVRMLLOD::callback(SoCallbackAction * action)
{
  SoVRMLLOD::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLLOD::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    SoVRMLLOD::GLRenderBelowPath(action);
    break;
  case SoAction::IN_PATH:
    SoVRMLLOD::GLRenderInPath(action);
    break;
  case SoAction::OFF_PATH:
    SoVRMLLOD::GLRenderOffPath(action);
    break;
  default:
    assert(0 && "unknown path code.");
    break;
  }
}

// Doc in parent
void
SoVRMLLOD::rayPick(SoRayPickAction * action)
{
  SoVRMLLOD::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLLOD::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: SGI OIV seems to do some extra work here, but the manual
  // pages states that it should do a normal SoGroup traversal.
  // we should _not_ use whichToTraverse() to calculate bbox as
  // this would cause cache dependencies on the camera and
  // the model matrix.                       pederb, 2001-02-21
  inherited::getBoundingBox(action);
}

// Doc in parent
void
SoVRMLLOD::search(SoSearchAction * action)
{
  inherited::search(action);
}

// Doc in parent
void
SoVRMLLOD::write(SoWriteAction * action)
{
  SoNode::write(action);
}

// Doc in parent
void
SoVRMLLOD::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLLOD::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLLOD::GLRenderBelowPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);
  if (idx >= 0) {
    if (!action->abortNow()) {
      SoNode * child = (SoNode*) this->getChildren()->get(idx);
      action->pushCurPath(idx, child);
      child->GLRenderBelowPath(action);
      action->popCurPath();
    }
  }
  // don't auto cache LOD nodes.
  SoGLCacheContextElement::shouldAutoCache(action->getState(),
                                           SoGLCacheContextElement::DONT_AUTO_CACHE);
}

// Doc in parent
void
SoVRMLLOD::GLRenderInPath(SoGLRenderAction * action)
{
  int numindices;
  const int * indices;
  SoAction::PathCode pathcode = action->getPathCode(numindices, indices);

  if (pathcode == SoAction::IN_PATH) {
    for (int i = 0; i < numindices; i++) {
      if (action->abortNow()) break;
      int idx = indices[i];
      SoNode * node = this->getChild(idx);
      action->pushCurPath(idx, node);
      node->GLRenderInPath(action);
      action->popCurPath(pathcode);
    }
  }
  else {
    assert(pathcode == SoAction::BELOW_PATH);
    SoVRMLLOD::GLRenderBelowPath(action);
  }
}

// Doc in parent
void
SoVRMLLOD::GLRenderOffPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);;
  if (idx >= 0) {
    SoNode * node = this->getChild(idx);
    if (node->affectsState()) {
      action->pushCurPath(idx, node);
      node->GLRenderOffPath(action);
      action->popCurPath();
    }
  }
}

// Doc in parent
void
SoVRMLLOD::addChild(SoNode * child)
{
  this->level.addNode(child);
  this->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLLOD::insertChild(SoNode * child, int idx)
{
  this->level.insertNode(child, idx);
  this->childlistvalid = FALSE;
}

// Doc in parent
SoNode *
SoVRMLLOD::getChild(int idx) const
{
  return this->level.getNode(idx);
}

// Doc in parent
int
SoVRMLLOD::findChild(const SoNode * child) const
{
  return this->level.findNode(child);
}

// Doc in parent
int
SoVRMLLOD::getNumChildren(void) const // virtual
{
  return this->level.getNumNodes();
}

// Doc in parent
void
SoVRMLLOD::removeChild(int idx)
{
  this->level.removeNode(idx);
  this->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLLOD::removeChild(SoNode * child)
{
  this->level.removeNode(child);
  this->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLLOD::removeAllChildren(void)
{
  this->level.removeAllNodes();
  SoGroup::children->truncate(0);
  this->childlistvalid = TRUE;
}

// Doc in parent
void
SoVRMLLOD::replaceChild(int idx, SoNode * child)
{
  this->level.replaceNode(idx, child);
  this->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLLOD::replaceChild(SoNode * old,
                           SoNode * child)
{
  this->level.replaceNode(old, child);
  this->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLLOD::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->level) {
    this->childlistvalid = FALSE;
  }
  inherited::notify(list);
}

// Doc in parent
SbBool
SoVRMLLOD::readInstance(SoInput * in,
                        unsigned short flags)
{
  SoGroup::children->truncate(0);
  SbBool oldnot = this->level.enableNotify(FALSE);
  SbBool ret = inherited::readInstance(in, flags);
  if (oldnot) this->level.enableNotify(TRUE);
  this->childlistvalid = FALSE;
  return ret;
}

// Doc in parent
void
SoVRMLLOD::copyContents(const SoFieldContainer * from,
                        SbBool copyConn)
{
  SoGroup::children->truncate(0);
  SoNode::copyContents(from, copyConn);
  this->childlistvalid = FALSE;
}

/*!
  Returns the child to traverse based on distance to current viewpoint.
*/
int
SoVRMLLOD::whichToTraverse(SoAction * action)
{
  SoState * state = action->getState();
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  const SbViewVolume & vv = SoViewVolumeElement::get(state);

  SbVec3f worldcenter;
  mat.multVecMatrix(this->center.getValue(), worldcenter);

  float dist = (vv.getProjectionPoint() - worldcenter).length();

  int i;
  int n = this->range.getNum();

  for (i = 0; i < n; i++) {
    if (dist < this->range[i]) break;
  }
  if (i >= this->getNumChildren()) i = this->getNumChildren() - 1;
  return i;
}

// Doc in parent
SoChildList *
SoVRMLLOD::getChildren(void) const
{
  if (!this->childlistvalid) {
    SoVRMLParent::updateChildList(this->level.getValues(0),
                                  this->level.getNum(),
                                  *SoGroup::children);
    ((SoVRMLLOD*)this)->childlistvalid = TRUE;
  }
  return SoGroup::children;
}
