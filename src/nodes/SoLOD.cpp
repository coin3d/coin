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
  \class SoLOD SoLOD.h Inventor/nodes/SoLOD.h
  \brief The SoLOD class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

/*!
  \var SoMFFloat SoLOD::range
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoLOD::center
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLOD);

/*!
  Default constructor.
*/
SoLOD::SoLOD(void)
{
  this->commonConstructor();
}

/*!
  Specify the expected number of children this node will have, to make
  it possible to do more efficient resource allocation.
*/
SoLOD::SoLOD(int numchildren)
  : inherited(numchildren)
{
  this->commonConstructor();
}

// private
void
SoLOD::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLOD);

  SO_NODE_ADD_FIELD(center, (SbVec3f(0, 0, 0)));
  SO_NODE_ADD_FIELD(range, (0.0f));

  // Make multivalue field empty.
  this->range.setNum(0);
}

/*!
  Destructor.
*/
SoLOD::~SoLOD()
{
}

// doc in super
void
SoLOD::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLOD);
}


/*!
  FIXME: write doc
 */
void
SoLOD::doAction(SoAction *action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
  else {
    int idx = this->whichToTraverse(action);;
    if (idx >= 0) this->children->traverse(action, idx);
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::callback(SoCallbackAction *action)
{
  SoLOD::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    SoLOD::GLRenderBelowPath(action);
    break;
  case SoAction::IN_PATH:
    SoLOD::GLRenderInPath(action);
    break;
  case SoAction::OFF_PATH:
    SoLOD::GLRenderOffPath(action);
    break;
  default:
    assert(0 && "unknown path code.");
    break;
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderBelowPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);
  if (idx >= 0) {
    if (!action->abortNow()) {
      SoNode * child = (SoNode*) this->children->get(idx);
      action->pushCurPath(idx, child);
      child->GLRenderBelowPath(action);
      action->popCurPath();
    }
  }
  // don't auto cache LOD nodes.
  SoGLCacheContextElement::shouldAutoCache(action->getState(),
                                           SoGLCacheContextElement::DONT_AUTO_CACHE);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderInPath(SoGLRenderAction * action)
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
    SoLOD::GLRenderBelowPath(action);
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderOffPath(SoGLRenderAction * action)
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

/*!
  FIXME: write doc
 */
void
SoLOD::rayPick(SoRayPickAction *action)
{
  SoLOD::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoLOD::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: SGI OIV seems to do some extra work here, but the manual
  // pages states that it should do a normal SoGroup traversal.
  // we should _not_ use whichToTraverse() to calculate bbox as
  // this would cause cache dependencies on the camera and
  // the model matrix.                       pederb, 2001-02-21
  inherited::getBoundingBox(action);
}

/*!
  FIXME: write doc
 */
void
SoLOD::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoLOD::doAction((SoAction*)action);
}

/*!
  Returns the child to traverse based on the ranges in
  SoLOD::range. Will clamp to index to the number of children.
  This method will return -1 if no child should be traversed.
  This will only happen if the node has no children though.
*/
int
SoLOD::whichToTraverse(SoAction *action)
{
  SoState *state = action->getState();
  const SbMatrix &mat = SoModelMatrixElement::get(state);
  const SbViewVolume &vv = SoViewVolumeElement::get(state);

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
