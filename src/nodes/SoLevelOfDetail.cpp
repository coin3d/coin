/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoLevelOfDetail SoLevelOfDetail.h Inventor/nodes/SoLevelOfDetail.h
  \brief The SoLevelOfDetail class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// FIXME: this node should have a cleanUp-func
static SoGetBoundingBoxAction *bboxAction = NULL;

/*!
  \var SoMFFloat SoLevelOfDetail::screenArea
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLevelOfDetail);

/*!
  Constructor.
*/
SoLevelOfDetail::SoLevelOfDetail()
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

/*!
  Does initialization common for all objects of the
  SoLevelOfDetail class. This includes setting up the
  type system, among other things.
*/
void
SoLevelOfDetail::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLevelOfDetail);
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
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
    assert(0 && "FIXME: unknown path code");
    return;
  }

  // for some strange reason, gcc (egcs-2.91.66) won't accept the code
  // below inside a case (yes, I did use brackets).
  // That's the reason for the strange switch/case above. pederb 19991116

  SoState * state = action->getState();
  int n = this->getNumChildren();
  if (n == 0) return;
  float complexity = SoComplexityElement::get(state);

  if ((SoComplexityTypeElement::get(state) ==
       SoComplexityTypeElement::BOUNDING_BOX) ||
      complexity == 0.0f) {
    state->push();
    this->getChildren()->traverse(action, n-1);
    state->pop();
    return;
  }
  if (!bboxAction) {
    SbViewportRegion dummy;
    bboxAction = new SoGetBoundingBoxAction(dummy);
  }
  bboxAction->setViewportRegion(SoViewportRegionElement::get(state));
  bboxAction->apply(this); // find bbox of all children
  SbVec2s size;
  SbBox3f bbox = bboxAction->getBoundingBox();
  SoShape::getScreenSize(state, bbox, size);

  float area = float(size[0])*float(size[1])*complexity;

  // in case there are too few screenArea values
  n = SbMin(n, this->screenArea.getNum());

  for (int i = 0; i < n; i++) {
    if (area > this->screenArea[i]) {
      state->push();
      this->getChildren()->traverse(action, i);
      state->pop();
      return;
    }
  }
  // if we get here, the last child should be traversed
  state->push();
  this->getChildren()->traverse(action, this->getNumChildren()-1);
  state->pop();
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::callback(SoCallbackAction *action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoLevelOfDetail::doAction((SoAction*)action);
    action->invokePostCallbacks(this);
  }
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::GLRender(SoGLRenderAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::rayPick(SoRayPickAction *action)
{
  SoLevelOfDetail::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
