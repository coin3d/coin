/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoSelection SoSelection.h Inventor/nodes/SoSelection.h
  \brief The SoSelection class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/events/SoMouseButtonEvent.h>

/*!
  \enum SoSelection::Policy
  FIXME: write documentation for enum
*/
/*!
  \var SoSelection::Policy SoSelection::SINGLE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSelection::Policy SoSelection::TOGGLE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSelection::Policy SoSelection::SHIFT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoSelection::policy
  FIXME: write documentation for field
*/


/*!
  \var SoPathList SoSelection::selectionList
  \internal
*/
/*!
  \var SoCallbackList * SoSelection::selCBList
  \internal
*/
/*!
  \var SoCallbackList * SoSelection::deselCBList
  \internal
*/
/*!
  \var SoCallbackList * SoSelection::startCBList
  \internal
*/
/*!
  \var SoCallbackList * SoSelection::finishCBList
  \internal
*/
/*!
  \var SoSelectionPickCB * SoSelection::pickCBFunc
  \internal
*/
/*!
  \var void * SoSelection::pickCBData
  \internal
*/
/*!
  \var SbBool SoSelection::callPickCBOnlyIfSelectable
  \internal
*/
/*!
  \var SoCallbackList * SoSelection::changeCBList
  \internal
*/
/*!
  \var SoPath * SoSelection::mouseDownPickPath
  \internal
*/
/*!
  \var SbBool SoSelection::pickMatching
  \internal
*/


static SoSearchAction * searchAction; // used to search for nodes

// *************************************************************************

SO_NODE_SOURCE(SoSelection);

/*!
  Constructor.
*/
SoSelection::SoSelection()
{
  this->init();
}

/*!
  Destructor.
*/
SoSelection::~SoSelection()
{
  delete this->selCBList;
  delete this->deselCBList;
  delete this->startCBList;
  delete this->finishCBList;
  delete this->changeCBList;
  if (this->mouseDownPickPath) this->mouseDownPickPath->unref();
}

/*!
  Does initialization common for all objects of the
  SoSelection class. This includes setting up the
  type system, among other things.
*/
void
SoSelection::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSelection);
}



/*!
  FIXME: write doc
 */
SoSelection::SoSelection(const int nChildren)
  : inherited(nChildren)
{
  this->init();
}

//
// common code for both constructors
//
void
SoSelection::init(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSelection);

  SO_NODE_ADD_FIELD(policy, (SoSelection::SHIFT));

  SO_NODE_DEFINE_ENUM_VALUE(Policy, SINGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, TOGGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SHIFT);
  SO_NODE_SET_SF_ENUM_TYPE(policy, Policy);

  this->selCBList = new SoCallbackList;
  this->deselCBList = new SoCallbackList;
  this->startCBList = new SoCallbackList;
  this->finishCBList = new SoCallbackList;
  this->changeCBList = new SoCallbackList;

  this->pickCBFunc = NULL;
  this->pickCBData = NULL;
  this->callPickCBOnlyIfSelectable = FALSE;

  this->mouseDownPickPath = NULL;
  this->pickMatching = TRUE;
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(const SoPath * path)
{
  SoPath * newpath = this->copyFromThis(path);
  if (newpath && this->findPath(newpath) < 0) {
    newpath->ref();
    this->addPath(newpath);
    newpath->unrefNoDelete();
  }
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(SoNode * node)
{
  SoPath * path = this->searchNode(node);
  if (path && this->findPath(path) < 0) {
    path->ref();
    this->addPath(path);
    path->unrefNoDelete();
  }
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(const SoPath * path)
{
  int idx = this->findPath(path);
  if (idx >= 0) this->removePath(idx);
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(const int which)
{
  this->removePath(which);
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(SoNode * node)
{
  SoPath * path = this->searchNode(node);
  if (path) {
    path->ref();
    this->deselect(path);
    path->unref();
  }
}

/*!
  FIXME: write doc
 */
void
SoSelection::toggle(const SoPath * path)
{
  int idx = this->findPath(path);
  if (idx >= 0) this->removePath(idx);
  else this->select(path); // call select instead of addPath to copy path before adding
}


/*!
  FIXME: write doc
 */
void
SoSelection::toggle(SoNode * node)
{
  SoPath * path = this->searchNode(node);
  if (path) {
    path->ref();
    this->toggle(path);
    path->unref();
  }
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isSelected(const SoPath * path) const
{
  return this->findPath(path) >= 0;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isSelected(SoNode * node) const
{
  SoPath * path = this->searchNode(node);
  if (path) {
    path->ref();
    SbBool ret = this->isSelected(path);
    path->unref();
    return ret;
  }
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselectAll(void)
{
  this->selectionList.truncate(0);
}

/*!
  FIXME: write doc
 */
int
SoSelection::getNumSelected(void) const
{
  return this->selectionList.getLength();
}

/*!
  FIXME: write doc
 */
const SoPathList *
SoSelection::getList(void) const
{
  return &this->selectionList;
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::getPath(const int index) const
{
  return this->selectionList[index];
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::operator[](const int i) const
{
  return this->selectionList[i];
}

/*!
  FIXME: write doc
 */
void
SoSelection::addSelectionCallback(SoSelectionPathCB * f, void * userData)
{
  this->selCBList->addCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeSelectionCallback(SoSelectionPathCB * f, void * userData)
{
  this->selCBList->removeCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addDeselectionCallback(SoSelectionPathCB * f, void * userData)
{
  this->deselCBList->addCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeDeselectionCallback(SoSelectionPathCB * f, void * userData)
{
  this->deselCBList->removeCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addStartCallback(SoSelectionClassCB * f, void * userData)
{
  this->startCBList->addCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeStartCallback(SoSelectionClassCB * f, void * userData)
{
  this->startCBList->removeCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addFinishCallback(SoSelectionClassCB * f, void * userData)
{
  this->finishCBList->addCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeFinishCallback(SoSelectionClassCB * f, void * userData)
{
  this->finishCBList->removeCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::setPickFilterCallback(SoSelectionPickCB * f,
                                   void * userData,
                                   const SbBool callOnlyIfSelectable)
{
  this->pickCBFunc = f;
  this->pickCBData = userData;
  this->callPickCBOnlyIfSelectable = callOnlyIfSelectable;
}

/*!
  FIXME: write doc
 */
void
SoSelection::setPickMatching(const SbBool pickMatching)
{
  this->pickMatching = pickMatching;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isPickMatching(void) const
{
  return this->pickMatching;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::getPickMatching(void) const
{
  return this->pickMatching;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addChangeCallback(SoSelectionClassCB * f, void * userData)
{
  this->changeCBList->addCallback((SoCallbackListCB *)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeChangeCallback(SoSelectionClassCB * f, void * userData)
{
  this->changeCBList->removeCallback((SoCallbackListCB *)f, userData);
}
/*!
  FIXME: write doc
*/
void
SoSelection::invokeSelectionPolicy(SoPath * path,
                                   SbBool shiftdown)
{
  SbBool toggle = this->policy.getValue() == SoSelection::TOGGLE ||
    (this->policy.getValue() == SoSelection::SHIFT && shiftdown);

  if (toggle)
    this->performToggleSelection(path);
  else
    this->performSingleSelection(path);
}

/*!
  FIXME: write doc
 */
void
SoSelection::performSingleSelection(SoPath * path)
{
  while (this->getNumSelected()) {
    this->removePath(this->getNumSelected()-1);
  }
  if (path) this->select(path);
}

/*!
  FIXME: write doc
 */
void
SoSelection::performToggleSelection(SoPath * path)
{
  if (path) {
    int idx = this->findPath(path);
    if (idx >= 0) {
      this->removePath(idx);
    }
    else if (path->findNode(this) >= 0) {
      this->select(path);
    }
  }
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::copyFromThis(const SoPath * path) const
{
  SoPath * newpath = NULL;
  path->ref();
  int i = path->findNode(this);
  if (i >= 0) {
    newpath = path->copy(i);
  }
  path->unrefNoDelete();
  return newpath;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addPath(SoPath * path)
{
  this->selectionList.append(path);
  this->selCBList->invokeCallbacks(path);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removePath(const int which)
{
  SoPath * path = this->selectionList[which];
  path->ref();
  this->selectionList.remove(which);
  this->deselCBList->invokeCallbacks(path);
  path->unref();
}

/*!
  FIXME: write doc
 */
int
SoSelection::findPath(const SoPath * path) const
{
  int idx = -1;

  // make copy only if necessary
  if (path->getHead() != (SoNode *)this) {
    SoPath * newpath = this->copyFromThis(path);
    if (newpath) {
      newpath->ref();
      idx = this->selectionList.findPath(*newpath);
      newpath->unref();
    }
    else idx = -1;
  }
  else idx = this->selectionList.findPath(*path);
  return idx;
}

/*!
  FIXME: write doc
 */
void
SoSelection::handleEvent(SoHandleEventAction * action)
{
  const SoEvent * event = action->getEvent();

  SbBool haltaction = FALSE;
  if (SO_MOUSE_PRESS_EVENT(event, BUTTON1)) {
    if (this->mouseDownPickPath) {
      this->mouseDownPickPath->unref();
      this->mouseDownPickPath = NULL;
    }
    const SoPickedPoint * pp = action->getPickedPoint();
    if (pp) {
      this->mouseDownPickPath = pp->getPath();
      this->mouseDownPickPath->ref();
    }
    inherited::handleEvent(action);
  }
  else if (SO_MOUSE_RELEASE_EVENT(event, BUTTON1)) {
    SbBool ignorepick = FALSE;
    SoPath * selpath = this->getSelectionPath(action, ignorepick, haltaction);

    if (haltaction) {
      action->isHandled();
    }
    else {
      inherited::handleEvent(action);
    }

    if (!ignorepick) {
      if (selpath) selpath->ref();
      this->startCBList->invokeCallbacks(this);
      this->invokeSelectionPolicy(selpath, event->wasShiftDown());
      this->finishCBList->invokeCallbacks(this);
      if (selpath) selpath->unref();
    }
    if (this->mouseDownPickPath) {
      this->mouseDownPickPath->unref();
      this->mouseDownPickPath = NULL;
    }
  }
  else {
    inherited::handleEvent(action);
  }
}

//
// uses search action to find path to node from this
//
SoPath *
SoSelection::searchNode(SoNode * node) const
{
  if (searchAction == NULL) {
    searchAction = new SoSearchAction;
    searchAction->setInterest(SoSearchAction::FIRST);
  }
  searchAction->setNode(node);
  searchAction->apply((SoNode *)this);
  return searchAction->getPath();
}

SoPath *
SoSelection::getSelectionPath(SoHandleEventAction * action, SbBool & ignorepick,
                              SbBool & haltaction)
{
  //
  // handled like described in the man-pages for SoSelection
  //

  haltaction = FALSE;
  ignorepick = FALSE;
  if (!this->mouseDownPickPath) return NULL;

  const SoPickedPoint * pp = action->getPickedPoint();
  SoPath * selectionpath = NULL;
  if (pp) {
    selectionpath = pp->getPath();
    int forkpos = selectionpath->findFork(this->mouseDownPickPath);
    if (forkpos < selectionpath->getLength()-1) {
      ignorepick = TRUE;
      return NULL;
    }
    if (this->pickCBFunc && (!this->callPickCBOnlyIfSelectable ||
                             selectionpath->findNode(this) >= 0)) {
      selectionpath = this->pickCBFunc(this->pickCBData, pp);
      if (selectionpath) {
        if (selectionpath->getLength() == 1 &&
            selectionpath->getNode(0) == this) {
          selectionpath->ref();
          selectionpath->unref();
          selectionpath = NULL;
        }
        else if (selectionpath->findNode(this) >= 0) {
          haltaction = TRUE;
        }
        else { // path with this not in the path (most probably an empty path)
          selectionpath->ref();
          selectionpath->unref();
          selectionpath = NULL;
          ignorepick = TRUE;
        }
      }
      else { // pickCBFunc returned NULL
        haltaction = TRUE;
      }
    }
    else { // no pickCBFunc or not valid path
      haltaction = FALSE;
    }
  }
  else if (this->mouseDownPickPath) {
    ignorepick = TRUE;
  }
  return selectionpath;
}
