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
  \class SoVRMLSwitch SoVRMLSwitch.h Inventor/VRMLnodes/SoVRMLSwitch.h
  \brief The SoVRMLSwitch class is a group node for traversing selected children.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Switch {
    exposedField    MFNode  choice      []
    exposedField    SFInt32 whichChoice -1    # [-1, inf)
  }
  \endverbatim

  The Switch grouping node traverses zero or one of the nodes
  specified in the choice field.  4.6.5, Grouping and children nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5>),
  describes details on the types of nodes that are legal values for
  choice.

  The \e whichChoice field specifies the index of the child to
  traverse, with the first child having index 0. If whichChoice is
  less than zero or greater than the number of nodes in the choice
  field, nothing is chosen.  All nodes under a Switch continue to
  receive and send events regardless of the value of whichChoice. For
  example, if an active SoVRMLTimeSensor is contained within an
  inactive choice of an Switch, the SoVRMLTimeSensor sends events
  regardless of the Switch's state.

*/

/*!
  \var SoMFNode SoVRMLSwitch::choice
  Contains the children.
*/

/*!
  \var SoSFInt32 SoVRMLSwitch::whichChoice

  Selected choice. Can be a positive number from 0 to <num
  children-1>, or one of the constants SO_SWITCH_NODE, SO_SWITCH_ALL or
  SO_SWITCH_INHERIT.  Default value is SO_SWITCH_NONE.

*/

#include <Inventor/VRMLnodes/SoVRMLSwitch.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/nodes/SoSubNodeP.h>

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
#include <Inventor/misc/SoChildList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <stddef.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#ifndef DOXYGEN_SKIP_THIS
class SoVRMLSwitchP {
public:
  SbBool childlistvalid;
#ifdef COIN_THREADSAFE
  SbMutex childlistmutex;
#endif // COIN_THREADSAFE
  void lockChildList(void) {
#ifdef COIN_THREADSAFE
    this->childlistmutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlockChildList(void) {
#ifdef COIN_THREADSAFE
    this->childlistmutex.unlock();
#endif // COIN_THREADSAFE
  }
};
#endif // DOXYGEN_SKIP_THIS

#define PRIVATE(thisp) ((thisp)->pimpl)

SO_NODE_SOURCE(SoVRMLSwitch);

// Doc in parent
void
SoVRMLSwitch::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSwitch, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLSwitch::SoVRMLSwitch(void)
{
  this->commonConstructor();
}

/*!
  Constructor. \a choices is the expected number of children.
*/
SoVRMLSwitch::SoVRMLSwitch(int choices)
  : SoGroup(choices)

{
  this->commonConstructor();
}

// commen constructor
void
SoVRMLSwitch::commonConstructor(void)
{
  PRIVATE(this) = new SoVRMLSwitchP;
  PRIVATE(this)->childlistvalid = FALSE;

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLSwitch);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(whichChoice, (SO_SWITCH_NONE));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(choice);
}

/*!
  Destructor.
*/
SoVRMLSwitch::~SoVRMLSwitch(void)
{
  delete PRIVATE(this);
}

// Doc in parent
SbBool
SoVRMLSwitch::affectsState(void) const // virtual
{
  int idx = this->whichChoice.getValue();
  if (idx == SO_SWITCH_NONE) return FALSE;
  if (idx >= this->getNumChildren()) return FALSE;
  if (idx >= 0 && !this->getChild(idx)->affectsState()) return FALSE;

  // FIXME: cover SO_SWITCH_INHERIT and SO_SWITCH_ALL.
  return TRUE;
}

/*!
  Adds \a choice to the \a choice field.
*/
void
SoVRMLSwitch::addChoice(SoNode * choice)
{
  this->addChild(choice);
}

/*!
  Inserts \a choice at index \a idx.
*/
void
SoVRMLSwitch::insertChoice(SoNode * choice,
                           int idx)
{
  this->insertChild(choice, idx);
}

/*!
  Returns the choice at index \a idx.
*/
SoNode *
SoVRMLSwitch::getChoice(int idx) const
{
  return this->getChild(idx);
}

/*!
  Finds the index for \a choice, or -1 if not found.
*/
int
SoVRMLSwitch::findChoice(SoNode * choice) const
{
  return this->findChild(choice);
}

/*!
  Returns the number of choices.
*/
int
SoVRMLSwitch::getNumChoices(void) const
{
  return this->getNumChildren();
}

/*!
  Removes the choice at index \a idx.
*/
void
SoVRMLSwitch::removeChoice(int idx)
{
  this->removeChild(idx);
}

/*!
  If \a choice is found, remove it.
*/
void
SoVRMLSwitch::removeChoice(SoNode * choice)
{
  this->removeChild(choice);
}

/*!
  Removes all choices.
*/
void
SoVRMLSwitch::removeAllChoices(void)
{
  this->removeAllChildren();
}

/*!
  Replace the choice at index \a idx with \a choice.
*/
void
SoVRMLSwitch::replaceChoice(int idx,
                            SoNode * choice)
{
  this->replaceChild(idx, choice);
}

/*!
  Find \a old, and replace it with \a choice.
*/
void
SoVRMLSwitch::replaceChoice(SoNode * old,
                            SoNode * choice)
{
  this->replaceChild(old, choice);
}

// Doc in parent
void
SoVRMLSwitch::doAction(SoAction * action)
{
  SoState * state = action->getState();
  int idx = this->whichChoice.isIgnored() ?
    SO_SWITCH_NONE : this->whichChoice.getValue();
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
        this->getChildren()->traverse(bbaction,
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
        this->getChildren()->traverseInPath(action, numindices, indices);
      }
      else {
        this->getChildren()->traverse(action);
      }
    }
  }
  else if (idx >= 0) { // should only traverse one child
    if (pathcode == SoAction::IN_PATH) {
      // traverse only if one path matches idx
      for (int i = 0; i < numindices; i++) {
        if (indices[i] == idx) {
          this->getChildren()->traverse(action, idx);
          break;
        }
      }
    }
    else { // off, below or no path traversal
      // be robust for index out of range
      if (idx >= this->getNumChildren()) {
#if COIN_DEBUG
        SoDebugError::post("SoSwitch::doAction",
                           "whichChoice %d out of range (0-%d).",
                           idx, this->getNumChildren());
#endif // COIN_DEBUG
      }
      else {
        this->getChildren()->traverse(action, idx);
      }
    }
  }
}

// Doc in parent
void
SoVRMLSwitch::callback(SoCallbackAction * action)
{
  SoVRMLSwitch::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLSwitch::GLRender(SoGLRenderAction * action)
{
  SoVRMLSwitch::doAction(action);
}

// Doc in parent
void
SoVRMLSwitch::pick(SoPickAction * action)
{
  SoVRMLSwitch::doAction(action);
}

// Doc in parent
void
SoVRMLSwitch::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoVRMLSwitch::doAction(action);
}

// Doc in parent
void
SoVRMLSwitch::handleEvent(SoHandleEventAction * action)
{
  SoVRMLSwitch::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLSwitch::audioRender(SoAudioRenderAction * action)
{
  SoVRMLSwitch::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLSwitch::getMatrix(SoGetMatrixAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::OFF_PATH:
  case SoAction::IN_PATH:
    SoVRMLSwitch::doAction((SoAction*)action);
    break;
  default:
    break;
  }
}

// Doc in parent
void
SoVRMLSwitch::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  if (action->isSearchingAll()) {
    this->getChildren()->traverse(action);
  }
  else {
    SoVRMLSwitch::doAction(action);
  }
}

// Doc in parent
void
SoVRMLSwitch::write(SoWriteAction * action)
{
  inherited::write(action);
}

// Doc in parent
void
SoVRMLSwitch::addChild(SoNode * child)
{
  this->choice.addNode(child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLSwitch::insertChild(SoNode * child, int idx)
{
  this->choice.insertNode(child, idx);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
SoNode *
SoVRMLSwitch::getChild(int idx) const
{
  return this->choice.getNode(idx);
}

// Doc in parent
int
SoVRMLSwitch::findChild(const SoNode * child) const
{
  return this->choice.findNode(child);
}

// Doc in parent
int
SoVRMLSwitch::getNumChildren(void) const // virtual
{
  return this->choice.getNumNodes();
}

// Doc in parent
void
SoVRMLSwitch::removeChild(int idx)
{
  this->choice.removeNode(idx);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLSwitch::removeChild(SoNode * child)
{
  this->choice.removeNode(child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLSwitch::removeAllChildren(void)
{
  this->choice.removeAllNodes();
  SoGroup::children->truncate(0);
  PRIVATE(this)->childlistvalid = TRUE;
}

// Doc in parent
void
SoVRMLSwitch::replaceChild(int idx, SoNode * child)
{
  this->choice.replaceNode(idx, child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLSwitch::replaceChild(SoNode * old,
                           SoNode * child)
{
  this->choice.replaceNode(old, child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLSwitch::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->choice) {
    PRIVATE(this)->childlistvalid = FALSE;
  }
  inherited::notify(list);
}

// Doc in parent
SbBool
SoVRMLSwitch::readInstance(SoInput * in,
                           unsigned short flags)
{
  SoGroup::children->truncate(0);
  SbBool oldnot = this->choice.enableNotify(FALSE);
  SbBool ret = inherited::readInstance(in, flags);
  if (oldnot) this->choice.enableNotify(TRUE);
  PRIVATE(this)->childlistvalid = FALSE;
  return ret;
}

// Doc in parent
void
SoVRMLSwitch::copyContents(const SoFieldContainer * from,
                           SbBool copyConn)
{
  SoGroup::children->truncate(0);
  SoNode::copyContents(from, copyConn);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
SoChildList *
SoVRMLSwitch::getChildren(void) const
{
  if (!PRIVATE(this)->childlistvalid) {
    // this is not 100% thread safe. The assumption is that no nodes
    // will be added or removed while a scene graph is being
    // traversed. For Coin, this is an ok assumption.
    PRIVATE(this)->lockChildList();
    // test again after we've locked
    if (!PRIVATE(this)->childlistvalid) {
      SoVRMLParent::updateChildList(this->choice.getValues(0),
                                    this->choice.getNum(),
                                    *SoGroup::children);
      PRIVATE(this)->childlistvalid = TRUE;
    }
    PRIVATE(this)->unlockChildList();
  }
  return SoGroup::children;
}

#undef PRIVATE
