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
  \class SoSelection SoSelection.h Inventor/nodes/SoSelection.h
  \brief The SoSelection class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoSelection.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/SoPath.h>

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


static SoSearchAction *searchAction; // used to search for nodes

// *************************************************************************

SO_NODE_SOURCE(SoSelection);

/*!
  Constructor.
*/
SoSelection::SoSelection()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSelection);

  SO_NODE_ADD_FIELD(policy, (SoSelection::SHIFT));

  SO_NODE_DEFINE_ENUM_VALUE(Policy, SINGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, TOGGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SHIFT);
  SO_NODE_SET_SF_ENUM_TYPE(policy, Policy);
}

/*!
  Destructor.
*/
SoSelection::~SoSelection()
{
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
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(const SoPath * const path)
{
  SoPath *newpath = this->copyFromThis(path);
  if (newpath) {
    newpath->ref();
    this->addPath(newpath);
    newpath->unref();
  }
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(SoNode * const node)
{
  SoPath *path = this->searchNode(node);
  if (path) {
    path->ref();
    this->addPath(path);
    path->unref();
  }
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(const SoPath * const path)
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
SoSelection::deselect(SoNode * const node)
{
  SoPath *path = this->searchNode(node);
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
SoSelection::toggle(const SoPath * const path)
{
  int idx = this->findPath(path);
  if (idx >= 0) this->removePath(idx);
  else this->select(path);
}


/*!
  FIXME: write doc
 */
void
SoSelection::toggle(SoNode * const node)
{
  SoPath *path = this->searchNode(node);
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
SoSelection::isSelected(const SoPath * const path) const
{
  return this->findPath(path) >= 0;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isSelected(SoNode * const node) const
{
  SoPath *path = this->searchNode(node);
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
SoSelection::addSelectionCallback(SoSelectionPathCB *f, void *userData)
{
  this->selCBList->addCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeSelectionCallback(SoSelectionPathCB *f, void *userData)
{
  this->selCBList->removeCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addDeselectionCallback(SoSelectionPathCB *f, void *userData)
{
  this->deselCBList->addCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeDeselectionCallback(SoSelectionPathCB *f, void *userData)
{
  this->deselCBList->removeCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addStartCallback(SoSelectionClassCB *f, void *userData)
{
  this->startCBList->addCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeStartCallback(SoSelectionClassCB *f, void *userData)
{
  this->startCBList->removeCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::addFinishCallback(SoSelectionClassCB *f, void *userData)
{
  this->finishCBList->addCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeFinishCallback(SoSelectionClassCB *f, void *userData)
{
  this->finishCBList->removeCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::setPickFilterCallback(SoSelectionPickCB *f,
                                   void *userData,
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
SoSelection::addChangeCallback(SoSelectionClassCB *f, void *userData)
{
  this->changeCBList->addCallback((SoCallbackListCB*)f, userData);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeChangeCallback(SoSelectionClassCB *f, void *userData)
{
  this->changeCBList->removeCallback((SoCallbackListCB*)f, userData);
}
/*!
  FIXME: write doc
 */
void
SoSelection::invokeSelectionPolicy(SoPath * const /* path */,
                                   const SbBool /* shiftDown */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::performSingleSelection(SoPath * const /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::performToggleSelection(SoPath * const /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::copyFromThis(const SoPath * const path) const
{
  int i = path->findNode(this);
  if (i >= 0) return path->copy(i);
  return NULL;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addPath(SoPath * const path)
{
  this->selectionList.append(path);
}

/*!
  FIXME: write doc
 */
void
SoSelection::removePath(const int which)
{
  this->selectionList.remove(which);
}

/*!
  FIXME: write doc
 */
int
SoSelection::findPath(const SoPath * const path) const
{
  int idx = -1;

  // make copy only if necessary
  if (path->getHead() != (SoNode*)this) {
    SoPath *newpath = this->copyFromThis(path);
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
SoSelection::handleEvent(SoHandleEventAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}

//
// uses search action to find path to node from this
//
SoPath *
SoSelection::searchNode(SoNode * const node) const
{
  if (searchAction == NULL) {
    searchAction = new SoSearchAction;
    searchAction->setInterest(SoSearchAction::FIRST);
  }
  searchAction->setNode(node);
  searchAction->apply((SoNode*)this);
  return searchAction->getPath();
}
