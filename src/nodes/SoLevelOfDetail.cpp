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
  \class SoLevelOfDetail SoLevelOfDetail.h Inventor/nodes/SoLevelOfDetail.h
  \brief The SoLevelOfDetail class is used to choose a child based on projected size.
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <stdlib.h>

// FIXME: not safe in an multithreaded environment. 20020106 mortene.
static SoGetBoundingBoxAction * bboxAction = NULL;

static void
SoLevelOfDetail_cleanup_func(void)
{
  delete bboxAction;
  bboxAction = NULL;
}

/*!
  \var SoMFFloat SoLevelOfDetail::screenArea

  The screen areas for each child.
*/

// *************************************************************************

SO_NODE_SOURCE(SoLevelOfDetail);

/*!
  Default constructor.
*/
SoLevelOfDetail::SoLevelOfDetail(void)
{
  this->commonConstructor();
}

/*!
  Specify the expected number of children this node will have, to make
  it possible to do more efficient resource allocation.
*/
SoLevelOfDetail::SoLevelOfDetail(int numchildren)
  : inherited(numchildren)
{
  this->commonConstructor();
}

// private
void
SoLevelOfDetail::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLevelOfDetail);

  SO_NODE_ADD_FIELD(screenArea, (0));
}

/*!
  Destructor.
*/
SoLevelOfDetail::~SoLevelOfDetail()
{
}

// doc in parent
void
SoLevelOfDetail::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLevelOfDetail);
}

// doc in parent
void
SoLevelOfDetail::doAction(SoAction *action)
{
  switch (action->getCurPathCode()) {
  case SoAction::IN_PATH:
    inherited::doAction(action); // normal path traversal
    return;
  case SoAction::OFF_PATH:
    return; // this is a separator node, return.
  case SoAction::BELOW_PATH:
  case SoAction::NO_PATH:
    break; // go on
  default:
    assert(0 && "unknown path code");
    return;
  }

  // for some strange reason, gcc (egcs-2.91.66) won't accept the code
  // below inside a case (yes, I did use brackets).
  // That's the reason for the strange switch/case above. pederb 19991116

  SoState * state = action->getState();
  int n = this->getNumChildren();
  if (n == 0) return;

  SbVec2s size;
  SbBox3f bbox;
  int i;
  int idx = -1;
  float projarea = 0.0f;

  SoComplexityTypeElement::Type complext = SoComplexityTypeElement::get(state);
  float complexity = SbClamp(SoComplexityElement::get(state), 0.0f, 1.0f);

  if (n == 1) { idx = 0; goto traverse; }
  if (complext == SoComplexityTypeElement::BOUNDING_BOX) { idx = n - 1; goto traverse; }
  if (complexity == 0.0f) { idx = n - 1; goto traverse; }
  if (complexity == 1.0f) { idx = 0; goto traverse; }
  if (this->screenArea.getNum() == 0) { idx = 0; goto traverse; }

  if (!bboxAction) {
    // The viewport region will be replaced every time the action is
    // used, so we can just feed it a dummy here.
    bboxAction = new SoGetBoundingBoxAction(SbViewportRegion());
    (void)atexit(SoLevelOfDetail_cleanup_func);
  }

  bboxAction->setViewportRegion(SoViewportRegionElement::get(state));
  bboxAction->apply(this); // find bbox of all children
  bbox = bboxAction->getBoundingBox();
  SoShape::getScreenSize(state, bbox, size);

  // The multiplication factor from the complexity setting is
  // complexity+0.5 because the documented behavior of SoLevelOfDetail
  // is to show lower detail levels than normal when
  // SoComplexity::value < 0.5, and to show higher detail levels when
  // SoComplexity::value > 0.5.
  projarea = float(size[0]) * float(size[1]) * (complexity + 0.5f);

  // in case there are too few screenArea values
  n = SbMin(n, this->screenArea.getNum());

  for (i = 0; i < n; i++) {
    if (projarea > this->screenArea[i]) { idx = i; goto traverse; }
  }

  // If we get here, projected area was lower than any of the
  // screenArea value, so the last child should be traversed.
  idx = this->getNumChildren() - 1;
  // (fall through to traverse:)

 traverse:
  state->push();
  this->getChildren()->traverse(action, idx);
  state->pop();
  return;
}

// doc in parent
void
SoLevelOfDetail::callback(SoCallbackAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}

// doc in parent
void
SoLevelOfDetail::GLRender(SoGLRenderAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}

// doc in parent
void
SoLevelOfDetail::rayPick(SoRayPickAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}
