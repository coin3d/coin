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
  \class SoSwitch SoSwitch.h Inventor/nodes/SoSwitch.h
  \brief The SoSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoChildList.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFInt32 SoSwitch::whichChild

  Selects which child to traverse during rendering (and some other)
  actions.

  The value should be either \c SO_SWITCH_NONE (for traversing no
  children, like it was an empty SoGroup node), \c SO_SWITCH_ALL (for
  traversing like if we were an SoGroup node), \c SO_SWITCH_INHERIT
  (for traversing the same child as the last SoSwitch node to parent
  this in the graph), or an index value for a child.

  Default value for the field is \c SO_SWITCH_NONE.
*/

// *************************************************************************

SO_NODE_SOURCE(SoSwitch);

/*!
  Default constructor.
*/
SoSwitch::SoSwitch(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSwitch);

  SO_NODE_ADD_FIELD(whichChild, (SO_SWITCH_NONE));
}

/*!
  Constructor taking as an argument the approximate number of children
  which will be inserted below this SoSwitch node instance. The number
  need not be exact, as it is only used as a hint for better memory
  allocation.
*/
SoSwitch::SoSwitch(int numchildren)
  : inherited(numchildren)
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

  SO_ENABLE(SoGetBoundingBoxAction, SoSwitchElement);
  SO_ENABLE(SoSearchAction, SoSwitchElement);
  SO_ENABLE(SoGetMatrixAction, SoSwitchElement);
  SO_ENABLE(SoGLRenderAction, SoSwitchElement);
  SO_ENABLE(SoPickAction, SoSwitchElement);

  SO_ENABLE(SoCallbackAction, SoSwitchElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoSwitchElement);
  SO_ENABLE(SoHandleEventAction, SoSwitchElement);
}

/*!
  FIXME: write function documentation
*/
void
SoSwitch::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}

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
    idx = this->whichChild.getValue();

  if (idx == SO_SWITCH_INHERIT) idx = SoSwitchElement::get(action->getState());
  else SoSwitchElement::set(action->getState(), idx);

  if (idx == SO_SWITCH_NONE) return;

  if (! ((idx < this->getNumChildren() && idx >= 0) || idx == SO_SWITCH_ALL)) {
#if COIN_DEBUG
    SoDebugError::post("SoSwitch::getBoundingBox",
                       "invalid whichChild value (%d) or switch index (%d)",
                       this->whichChild.getValue(), idx);
    this->whichChild = SO_SWITCH_NONE;
#endif // COIN_DEBUG
    return;
  }

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
      action->setCenter(acccenter / float(numCenters), FALSE);
  }
  else {
    this->getChildren()->traverse(action, idx);
  }
}

/*!
  This method must be overridden in SoSwitch nodes to take into account
  if the search involves every single node, or just the nodes involved in
  normal graph traversal.
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
    if (i < 0 || i >= this->getNumChildren()) {
#if COIN_DEBUG
      SoDebugError::post("SoSwitch::doAction",
                         "invalid whichChild value (%d) or switch index (%d)",
                         this->whichChild.getValue(), i);
      this->whichChild = SO_SWITCH_NONE;
#endif // COIN_DEBUG
      return;
    }
    this->children->traverse(action, i);
  }
}

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

/*!
  FIXME: write doc
 */
void
SoSwitch::callback(SoCallbackAction *action)
{
  SoSwitch::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoSwitch::pick(SoPickAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoSwitch::handleEvent(SoHandleEventAction *action)
{
  SoSwitch::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoSwitch::getMatrix(SoGetMatrixAction *action)
{
  // FIXME: is this correct? SoGetMatrixAction should normally not
  // traverse below the node it has been applied to. 20000302 mortene.
  SoSwitch::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoSwitch::write(SoWriteAction * action)
{
  // FIXME: anything missing? 19991112 mortene.
  inherited::write(action);
}

/*!
  FIXME: write doc
 */
void
SoSwitch::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
*/
void
SoSwitch::traverseChildren(SoAction * action)
{
  COIN_STUB();
}
