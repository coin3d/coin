/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

  SO_NODE_ADD_FIELD(range, (0.0f));
  // FIXME: is this the correct way of making an empty multifield?
  // 19990301 mortene.
  range.setNum(0);

  SO_NODE_ADD_FIELD(center, (SbVec3f(0, 0, 0)));
}

/*!
  Destructor.
*/
SoLOD::~SoLOD()
{
}

/*!
  Does initialization common for all objects of the
  SoLOD class. This includes setting up the
  type system, among other things.
*/
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
  int idx = this->whichToTraverse(action);;
  if (idx >= 0) this->children->traverse(action, idx);
}

/*!
  FIXME: write doc
 */
void
SoLOD::callback(SoCallbackAction *action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoLOD::doAction((SoAction*)action);
    action->invokePostCallbacks(this);
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRender(SoGLRenderAction * action)
{
  assert(action->getCurPathCode() == SoAction::NO_PATH);
  int idx = this->whichToTraverse(action);;
  if (idx >= 0) this->children->traverse(action, idx);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderBelowPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);
  if (idx >= 0) this->getChildren()->traverse(action, idx);
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderInPath(SoGLRenderAction * action)
{
  int numIndices;
  const int *indices;
  action->getPathCode(numIndices, indices);

  // no need to traverse OFF_PATH children
  for (int i = 0; i < numIndices; i++) {
    this->children->traverse(action, indices[i]);
  }
}

/*!
  FIXME: write doc
 */
void
SoLOD::GLRenderOffPath(SoGLRenderAction * action)
{
  int idx = this->whichToTraverse(action);;
  if (idx >= 0 && this->getChild(idx)->affectsState())
    this->children->traverse(action, idx);
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
  // FIXME: what should be done here? Calculating a new bbox each
  // time LOD is changed might hurt bbox cache performance.
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
