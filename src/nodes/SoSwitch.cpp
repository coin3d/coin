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
  \class SoSwitch SoSwitch.h Inventor/nodes/SoSwitch.h
  \brief The SoSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/misc/SoChildList.h>

#include <Inventor/errors/SoDebugError.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
#include <Inventor/actions/SoSearchAction.h>
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
#include <Inventor/elements/SoSwitchElement.h>
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

/*!
  \var SoSFInt32 SoSwitch::whichChild
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoSwitch);

/*!
  Constructor.
*/
SoSwitch::SoSwitch()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSwitch);

  SO_NODE_ADD_FIELD(whichChild, (SO_SWITCH_NONE));
}

/*!
  Destructor.
*/
SoSwitch::~SoSwitch()
{
}

/*!
  Does initialization common for all objects of the
  SoSwitch class. This includes setting up the
  type system, among other things.
*/
void
SoSwitch::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSwitch);

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  SO_ENABLE(SoSearchAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  SO_ENABLE(SoGetMatrixAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOPICKACTION

  SO_ENABLE(SoCallbackAction, SoSwitchElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoSwitchElement);
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoSwitch::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoSwitch::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: update this method to use a doAction() kind of
  // traversal. This traversal code is not correct i think.
  // pederb, 990618

  int numIndices;
  const int * indices;
  int idx;

  if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
    idx = indices[numIndices-1];
  else
    idx = whichChild.getValue();

  if (idx == SO_SWITCH_INHERIT) idx = SoSwitchElement::get(action->getState());
  else SoSwitchElement::set(action->getState(), idx);

  if (idx == SO_SWITCH_NONE) return;

  // FIXME: handle gracefully. 19990324 mortene.
  assert((idx < this->getNumChildren() && idx >= 0)
         || idx == SO_SWITCH_ALL);

  if (idx == SO_SWITCH_ALL) {
    // Initialize accumulation variables.
    SbVec3f acccenter(0.0f, 0.0f, 0.0f);
    int numCenters = 0;

    for (int i = 0; i < this->getNumChildren(); i++) {
      this->getChildren()->traverse(action, i);

      // If center point is set, accumulate.
      if (action->isCenterSet()) {
        acccenter += action->getCenter();
        numCenters++;
        action->resetCenter();
      }
    }

    if (numCenters != 0)
      action->setCenter(acccenter / numCenters, FALSE);
  }
  else {
    this->getChildren()->traverse(action, idx);
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write function documentation
*/
void
SoSwitch::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  // TRAVERSE
  if (action->isSearchingAll()) { // traverse all children no matter
    getChildren()->traverse(action);
  } else {
    doAction(action);
  }
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoSwitch::doAction(SoAction * action)
{
  SoState *state = action->getState();

  int numIndices;
  const int *indices;
  int idx;

  idx = whichChild.getValue();
  SbBool inheritIdx = FALSE;
  if (idx == SO_SWITCH_INHERIT) {
    inheritIdx = TRUE;
    idx = SoSwitchElement::get(action->getState());
  }

  int startIdx, endIdx;

  switch (action->getPathCode(numIndices, indices)) {
  case SoAction::IN_PATH:
    startIdx = 0;
    endIdx = indices[numIndices-1];
    break;
  case SoAction::OFF_PATH:
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    if (idx == SO_SWITCH_ALL) {
      startIdx = 0;
      endIdx = this->getNumChildren() - 1;
    }
    else if (idx < 0) {
      startIdx = 0;
      endIdx = -1; // don't traverse
    }
    else {
      startIdx = endIdx = idx;
    }
    break;
  default:
    startIdx = 0;
    endIdx = -1;
    assert(0 && "Unknown path code");
  }
  for (int i = startIdx; i <= endIdx; i++) {
    if (!inheritIdx) SoSwitchElement::set(state, this, i);
    this->children->traverse(action, i);
  }
}
#endif // !COIN_EXCLUDE_SOACTION



/*!
  FIXME: write doc
 */
SbBool
SoSwitch::affectsState(void) const
{
  int idx = whichChild.getValue();
  if (idx == SO_SWITCH_NONE) return FALSE;
  if (idx >= this->getNumChildren()) return FALSE;
  if (idx >= 0 && !this->getChild(idx)->affectsState()) return FALSE;
  // FIXME: cover SO_SWITCH_INHERIT and SO_SWITCH_ALL.

  return TRUE;
}

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::callback(SoCallbackAction *action)
{
  SoSwitch::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::pick(SoPickAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::handleEvent(SoHandleEventAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::getMatrix(SoGetMatrixAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::write(SoWriteAction * action)
{
  // FIXME: anything missing? 19991112 mortene.
  inherited::write(action);
}
#endif // !COIN_EXCLUDE_SOWRITEACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::traverseChildren(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
