/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
#include <coindefs.h> // COIN_OBSOLETED()

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

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
  SoSwitch::doAction(action);
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

  if (action->isSearchingAll()) {
    this->children->traverse(action);
  } 
  else {
    SoSwitch::doAction(action);
  }
}

/*!
  FIXME: write function documentation
*/
void
SoSwitch::doAction(SoAction * action)
{
  SoState * state = action->getState();
  int idx = this->whichChild.isIgnored() ? 
    SO_SWITCH_NONE : this->whichChild.getValue();
  if (idx == SO_SWITCH_INHERIT) {
    idx = SoSwitchElement::get(action->getState());
    // when we inherit, idx might be out of range. Use modulo.
    if (idx >= this->getNumChildren()) idx %= this->getNumChildren();
  }
  else {
    SoSwitchElement::set(state, idx);
  }
  
  int numindices;
  const int * indices;
  SoAction::PathCode pathcode = action->getPathCode(numindices, indices);
  
  if (idx == SO_SWITCH_ALL) {
    if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) {
      // calculate center of bbox if bboxaction. This makes the
      // switch node behave exactly like a group node
      SoGetBoundingBoxAction * bbaction = (SoGetBoundingBoxAction*) action;
      // Initialize accumulation variables.
      SbVec3f acccenter(0.0f, 0.0f, 0.0f);
      int numcenters = 0;
      // only traverse nodes in path(s) for IN_PATH traversal
      int n = pathcode == SoAction::IN_PATH ? numindices : this->getNumChildren();
      
      for (int i = 0; i < n; i++) {
        this->children->traverse(bbaction, 
                                 pathcode == SoAction::IN_PATH ? indices[i] : i);
          
        // If center point is set, accumulate.
        if (bbaction->isCenterSet()) {
          acccenter += bbaction->getCenter();
          numcenters++;
          bbaction->resetCenter();
        }
      }
      if (numcenters != 0) {
        bbaction->setCenter(acccenter / float(numcenters), FALSE);
      }
    }
    else { // not a getBoundingBoxAction
      if (pathcode == SoAction::IN_PATH) {
        this->children->traverseInPath(action, numindices, indices);
      }
      else {
        this->children->traverse(action);
      }
    }
  }
  else if (idx >= 0) { // should only traverse one child
    if (pathcode == SoAction::IN_PATH) {
      // traverse only if one path matches idx
      for (int i = 0; i < numindices; i++) {
        if (indices[i] == idx) {
          this->children->traverse(action, idx);
          break;
        }
      }
    }
    else { // off, below or no path traversal
      // be robust for index out of range
      if (idx >= this->getNumChildren()) {
#if COIN_DEBUG
        SoDebugError::post("SoSwitch::doAction",
                           "whichChild %d out of range (0-%d).",
                           idx, this->getNumChildren());
#endif // COIN_DEBUG
      }
      else {
        this->children->traverse(action, idx);
      }
    }
  }
}

/*!
  FIXME: write doc
 */
SbBool
SoSwitch::affectsState(void) const
{
  int idx = this->whichChild.getValue();
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
  switch (action->getCurPathCode()) {
  case SoAction::OFF_PATH:
  case SoAction::IN_PATH:
    SoSwitch::doAction((SoAction*)action);
    break;
  default:
    break;
  }
}

/*!
  FIXME: write doc
 */
void
SoSwitch::write(SoWriteAction * action)
{
  // to keep child numbering, always write out all children for a
  // switch

  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
    // Only increase number of writereferences to the top level node
    // in a tree which is used multiple times.
    if (!this->hasMultipleWriteRefs()) this->getChildren()->traverse(action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, TRUE, FALSE)) return;
    this->writeInstance(out);
    if (out->isBinary()) out->write(this->getNumChildren());
    this->getChildren()->traverse(action);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
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
  Not supported in Coin. Should probably have been private in OIV.
*/
void
SoSwitch::traverseChildren(SoAction * action)
{
  COIN_OBSOLETED();
}
