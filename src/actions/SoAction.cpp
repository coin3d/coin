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
  \class SoAction SoAction.h Inventor/actions/SoAction.h
  \brief The SoAction class is the base class for all traversal actions.
  \ingroup actions

  Applying actions is the basic mechanism in Coin for executing
  various operations on scene graphs or paths within scene graphs,
  including search operations, rendering, interaction through picking,
  etc.

  See the builtin actions for further information, or look at the
  example code applications of the Coin library to see how actions are
  generally used.

  For extending the Coin library with your own action classes, we
  strongly recommend that you make yourself acquainted with the
  excellent "The Inventor Toolmaker" book, which describes the tasks
  involved in detail.
*/

// Metadon doc
/*¡
  FIXME: the capability to do correct traversal of the paths in a path
  list is not implemented yet.
 */

#include <Inventor/actions/SoActions.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/misc/SoState.h>

#include <assert.h>
#include <stdlib.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_OBSOLETED

// define this to debug path traversal
// #define COIN_DEBUG_PATH_TRAVERAL 1

// *************************************************************************

SoEnabledElementsList * SoAction::enabledElements = NULL;
SoActionMethodList * SoAction::methods = NULL;
// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoAction::classTypeId;

// *************************************************************************

/*!
  \fn SoType SoAction::getTypeId(void) const
  Returns the actual type id of an object derived from a class
  inheriting SoAction. Needs to be overloaded in \e all subclasses.
*/

/*!
  \enum SoAction::AppliedCode
  Enumerated values for what the action was applied to.
*/

/*!
  \enum SoAction::PathCode
  Enumerated values for how the action is applied to a scene graph.
*/

/*!
  \var SoAction::state
  Pointer to the traversal state instance of the action.
*/

/*!
  \var SoAction::traversalMethods

  Stores the list of "nodetype to actionmethod" mappings for the
  particular action instance.
*/

/*!
  \var SoAction::methods

  Stores the list of default "nodetype to actionmethod" mappings for
  the action class.
*/

/*!
  \var SoAction::enabledElements

  The list of elements enabled during traversals with actions of this
  type.
*/

/*!
  \fn SoAction::PathCode SoAction::getCurPathCode(void) const
  Returns the current traversal path code.
*/

/*!
  \fn void SoAction::popCurPath(const PathCode prevpathcode)
  Pops the current path, and sets the path code to \a prevpathcode.

  This method is very internal. Do not use unless you know
  what you're doing.
*/

// *************************************************************************

/*!
  Default constructor, does all necessary toplevel initialization.
*/
SoAction::SoAction(void)
  : state(NULL),
    traversalMethods(NULL),
    appliedcode(NODE),
    currentpath(8),
    terminated(FALSE),
    currentpathcode(NO_PATH)
{
  this->applieddata.node = NULL;
}

/*!
  Destructor, free resources.
*/
SoAction::~SoAction(void)
{
  int n = this->pathcodearray.getLength();
  for (int i = 0; i < n; i++) delete this->pathcodearray[i];
  delete this->state;
}

// *************************************************************************

/*!
  Initializes the run-time type system for this class, and sets up the
  enabled elements and action method list.
*/
void
SoAction::initClass(void)
{
  SoAction::classTypeId = SoType::createType(SoType::badType(), "SoAction");

  // Pass NULL pointers for the parent lists.
  SoAction::enabledElements = new SoEnabledElementsList(NULL);
  SoAction::methods = new SoActionMethodList(NULL);

  // Override element is used everywhere.
  SoAction::enabledElements->enable(SoOverrideElement::getClassTypeId(),
                                    SoOverrideElement::getClassStackIndex());

  SoAction::initClasses();
}

/*!
  Initialize all the SoAction subclasses. Automatically called from
  SoAction::initClass().
*/
void
SoAction::initClasses(void)
{
  SoCallbackAction::initClass();
  SoGLRenderAction::initClass();
  SoBoxHighlightRenderAction::initClass();
  SoLineHighlightRenderAction::initClass();
  SoGetBoundingBoxAction::initClass();
  SoGetMatrixAction::initClass();
  SoGetPrimitiveCountAction::initClass();
  SoHandleEventAction::initClass();
  SoPickAction::initClass();
  SoRayPickAction::initClass();
  SoSearchAction::initClass();
  SoWriteAction::initClass();
}

/*!
  Returns the run-time type object associated with instances of this
  class.
*/
SoType
SoAction::getClassTypeId(void)
{
  return SoAction::classTypeId;
}

/*!
  Returns \c TRUE if the type of this object is either of the same
  type or a subclass of \a type.
*/
SbBool
SoAction::isOfType(SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

// *************************************************************************

/*!
  Applies the action to the scene graph rooted at \a root.

  Note that you should \e not apply an action to a node with a zero
  reference count. The behavior in that case is undefined.
*/
void
SoAction::apply(SoNode * root)
{
  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  this->traversalMethods->setUp();
  this->terminated = FALSE;

  this->currentpathcode = SoAction::NO_PATH;
  this->applieddata.node = root;
  this->appliedcode = SoAction::NODE;

  if (root) {
#if COIN_DEBUG
    if (root->getRefCount() == 0) {
      SoDebugError::postWarning("SoAction::apply",
                                "root node has reference count equal to zero");
    }
#endif // COIN_DEBUG
    // So the graph is not deallocated during traversal.
    root->ref();
    this->currentpath.setHead(root);
    // make sure state is created before traversing
    (void) this->getState();
    this->beginTraversal(root);
    this->endTraversal(root);
    this->applieddata.node = NULL;
    root->unrefNoDelete();
  }
}

/*!
  Applies the action to the parts of the graph defined by \a path.
*/
void
SoAction::apply(SoPath * path)
{
  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  this->traversalMethods->setUp();
  this->terminated = FALSE;

#if COIN_DEBUG
  if (path->getRefCount() == 0) {
    SoDebugError::postWarning("SoAction::apply",
                              "path has reference count equal to zero");
  }
#endif // COIN_DEBUG

  // So the path is not deallocated during traversal.
  path->ref();

  this->currentpathcode =
    path->getFullLength() > 1 ? SoAction::IN_PATH : SoAction::BELOW_PATH;
  this->applieddata.path = path;
  this->appliedcode = SoAction::PATH;

  // make sure state is created before traversing
  (void) this->getState();

  if (path->getLength() && path->getNode(0)) {
    SoNode * node = path->getNode(0);
    this->currentpath.setHead(node);
    this->beginTraversal(node);
    this->endTraversal(node);
  }

  path->unrefNoDelete();
}

/*!
  Applies action to the graphs defined by \a pathlist.  If \a
  obeysrules is set to \c TRUE, \a pathlist must obey the following
  four conditions (which is the case for path lists returned from
  search actions for non-group nodes and path lists returned from
  picking actions):

  All paths must start at the same head node. All paths must be sorted
  in traversal order. The paths must be unique. No path can continue
  through the end point of another path.
*/
void
SoAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  this->traversalMethods->setUp();
  if (pathlist.getLength() == 0) return;

  this->terminated = FALSE;

  // make sure state is created before traversing
  (void) this->getState();

  this->applieddata.pathlistdata.origpathlist = &pathlist;
  this->applieddata.pathlistdata.pathlist = &pathlist;
  this->appliedcode = PATH_LIST;
  this->currentpathcode = pathlist[0]->getFullLength() > 1 ?
    SoAction::IN_PATH : SoAction::BELOW_PATH;

  if (obeysrules) {
    // GoGoGo
    this->currentpath.setHead(pathlist[0]->getHead());
    this->beginTraversal(pathlist[0]->getHead());
    this->endTraversal(pathlist[0]->getHead());
  }
  else {
    // make copy of path list and make sure it obeys rules
    SoPathList sortedlist(pathlist);
    // sort
    sortedlist.sort();
    // remove unnecessary paths
    sortedlist.uniquify();
    int num = sortedlist.getLength();

    // if all head nodes are the same we can traverse in one go
    if (sortedlist[0]->getHead() == sortedlist[num-1]->getHead()) {
      this->currentpath.setHead(sortedlist[0]->getHead());
      this->applieddata.pathlistdata.pathlist = &sortedlist;
      this->beginTraversal(sortedlist[0]->getHead());
      this->endTraversal(sortedlist[0]->getHead());
    }
    else {
      // make one pass per head node. sortedlist is sorted on
      // different head nodes first, so this is very easy
      SoNode * head;
      SoPathList templist;
      int i = 0;
      while (i < num && !this->hasTerminated()) {
        head = sortedlist[i]->getHead();
        templist.append(sortedlist[i]);
        i++;
        while (i < num && sortedlist[i]->getHead() == head) {
          templist.append(sortedlist[i]);
          i++;
        }
        this->applieddata.pathlistdata.pathlist = &templist;
        this->appliedcode = PATH_LIST;
        this->currentpathcode = templist[0]->getFullLength() > 1 ?
          SoAction::IN_PATH : SoAction::BELOW_PATH;
        this->currentpath.setHead(templist[0]->getHead());
        this->beginTraversal(templist[0]->getHead());
        templist.truncate(0);
      }
    }
  }
}

/*!
  Invalidates the state, forcing it to be recreated at the next
  apply() invocation.
*/
void
SoAction::invalidateState(void)
{
  delete this->state;
  this->state = NULL;
}

// *************************************************************************

/*!
  This method is used for filling up the lookup tables with void
  methods.
*/
void
SoAction::nullAction(SoAction *, SoNode *)
{
}

/*!
  Returns a code indicating what (node, path, or pathlist) the action
  instance is being applied to.
*/
SoAction::AppliedCode
SoAction::getWhatAppliedTo(void) const
{
  return this->appliedcode;
}

/*!
  Returns a pointer to the node the action is being applied to.

  If action is not being applied to a node (but a path or a pathlist),
  the method returns \c NULL.
*/
SoNode *
SoAction::getNodeAppliedTo(void) const
{
  return this->appliedcode == SoAction::NODE ? this->applieddata.node : NULL;
}

/*!
  Returns a pointer to the path the action is being applied to.

  If action is not being applied to a path (but a node or a pathlist),
  the method returns \c NULL.
*/
SoPath *
SoAction::getPathAppliedTo(void) const
{
  return this->appliedcode == SoAction::PATH ? this->applieddata.path : NULL;
}

/*!
  Returns a pointer to the path list the action is currently being
  applied to.

  If action is not being applied to a path list (but a node or a
  path), the method returns \c NULL.

  The returned pathlist pointer need not be equal to the list apply()
  was called with, as the action may have reorganized the path list
  for efficiency reasons.

  \sa void SoAction::apply(const SoPathList &, SbBool)
*/
const SoPathList *
SoAction::getPathListAppliedTo(void) const
{
  return this->appliedcode == SoAction::PATH_LIST ?
    this->applieddata.pathlistdata.pathlist : NULL;
}

/*!
  Returns a pointer to the original path list the action is being
  applied to.

  If the action is not being applied to a path list (but a node or a
  path), the method returns \c NULL.
*/

const SoPathList *
SoAction::getOriginalPathListAppliedTo(void) const
{
  return this->appliedcode == SoAction::PATH_LIST ?
    this->applieddata.pathlistdata.origpathlist : NULL;
}

/*!
  Returns \c TRUE if the current path list being applied is the last
  path list from the set passed to apply().
*/
SbBool
SoAction::isLastPathListAppliedTo(void) const
{
  // FIXME: path list traversal is not really supported yet. We just
  // apply the paths one by one, so technically it should be correct
  // to return TRUE here...
  return TRUE;
}

/*!
  Returns a code that indicates where the current node lies with
  respect to the path(s) the action is being applied to.  The
  arguments \a indices and \a numindices are only set if the method
  returns \c IN_PATH.
*/
SoAction::PathCode
SoAction::getPathCode(int & numindices, const int * & indices)
{
  if (this->currentpathcode == SoAction::IN_PATH)
    this->usePathCode(numindices, indices);
  return this->currentpathcode;
}

/*!
  Traverses a scene graph rooted at \a node, invoking the action
  methods of the nodes in the graph.
*/
void
SoAction::traverse(SoNode * const node)
{
  (*this->traversalMethods)
    [SoNode::getActionMethodIndex(node->getTypeId())](this, node);
}

/*!
  Get ready to traverse the \a childindex'th child. Use this method
  if the path code might change as a result of this.

  This method is very internal. Do not use unless you know
  what you're doing.
*/
void
SoAction::pushCurPath(const int childindex, SoNode * node)
{
  if (node) this->currentpath.append(node, childindex);
  else this->currentpath.append(childindex);

  int curlen = this->currentpath.getFullLength();

  if (this->currentpathcode == IN_PATH) {
    if (this->getWhatAppliedTo() == PATH) {
      assert(curlen <= this->applieddata.path->getFullLength());
      if (this->currentpath.getIndex(curlen-1) !=
          this->applieddata.path->getIndex(curlen-1)) {
#ifdef COIN_DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"off path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // COIN_DEBUG_PATH_TRAVERSAL
        this->currentpathcode = OFF_PATH;
      }
      else if (curlen == this->applieddata.path->getFullLength()) {
        this->currentpathcode = BELOW_PATH;
#ifdef COIN_DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"below path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(),curlen);
#endif // COIN_DEBUG_PATH_TRAVERSAL
      }
    }
    else {
      // test for below path by testing for one path that contains
      // current path, and is longer than current.  at the same time,
      // test for off path by testing if there is no paths that
      // contains current path.  this is a lame and slow way to do it,
      // but I don't think path list traversal is used that often,
      // and not for a huge amount of paths. If I'm wrong, we'll just
      // have to optimize this, for instance by implementing
      // the SGI's SoCompactPathList class.
      //                                       pederb, 2001-03-28
      const SoPathList * pl = this->applieddata.pathlistdata.pathlist;
      int i, n = pl->getLength();
      int len = -1;

      for (i = 0; i < n; i++) {
        const SoPath * path = (*pl)[i];
        len = path->getFullLength();
        // small optimization, no use testing if path is shorter
        if (len >= curlen) {
          if (path->containsPath(&this->currentpath)) break;
        }
      }
      // if no path is found, we're off path
      if (i == n) {
        this->currentpathcode = OFF_PATH;
#ifdef COIN_DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"off path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // COIN_DEBUG_PATH_TRAVERSAL
      }
      else if (len == curlen) {
        this->currentpathcode = BELOW_PATH;
#ifdef COIN_DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"below path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // COIN_DEBUG_PATH_TRAVERSAL
      }
    }
  }
}

/*!
  Returns \c TRUE if the action was prematurely terminated.

  Note that the termination flag will be \c FALSE if the action simply
  completed its run over the scene graph in the "ordinary" fashion,
  i.e. was not explicitly aborted from any of the nodes in the graph.

  \sa setTerminated()
*/
SbBool
SoAction::hasTerminated(void) const
{
  return this->terminated;
}

/*!
  Returns a pointer to the state of the action instance. The state
  contains the current set of elements used during traversal.
*/
SoState *
SoAction::getState(void) const
{
  if (this->state == NULL) {
    // cast away constness to set state
    ((SoAction*)this)->state =
      new SoState((SoAction*)this, this->getEnabledElements().getElements());
  }
  return this->state;
}

/*!
  Returns a pointer to the path generated during traversal, from the
  root of the traversed graph to the current node.
*/
const SoPath *
SoAction::getCurPath(void)
{
  return &this->currentpath;
}

/*!
  \internal
*/
SoNode *
SoAction::getCurPathTail(void)
{
  return this->currentpath.getTail();
}

/*!
  \internal
*/
void
SoAction::usePathCode(int & numindices, const int * & indices)
{
  int curlen = this->currentpath.getFullLength();

  while (this->pathcodearray.getLength() < curlen) {
    this->pathcodearray.append(new SbList<int>);
  }

  SbList <int> * myarray = this->pathcodearray[curlen-1];
  myarray->truncate(0);

  if (this->getWhatAppliedTo() == PATH_LIST) {
    // this might be very slow if the list contains a lot of
    // paths. See comment in pushCurPath(int, SoNode*) about this.
    const SoPathList * pl = this->applieddata.pathlistdata.pathlist;
    int n = pl->getLength();
    int previdx = -1;
    myarray->truncate(0);
    for (int i = 0; i < n; i++) {
      const SoPath * path = (*pl)[i];
      if (path->getFullLength() > curlen &&
          path->containsPath(&this->currentpath)) {
        int idx = path->getIndex(curlen);
        if (idx != previdx) {
          myarray->append(idx);
          previdx = idx;
        }
      }
    }
    numindices = myarray->getLength();
    indices = myarray->getArrayPtr();
  }
  else {
    numindices = 1;
    myarray->append(this->applieddata.path->getIndex(curlen));
    indices = myarray->getArrayPtr();
  }
}

/*!
  Pushes a NULL node onto the current path. Use this before
  traversing all children when you know that the path code will not
  change while traversing children.

  This method is very internal. Do not use unless you know
  what you're doing.
*/
void
SoAction::pushCurPath(void)
{
  this->currentpath.append((SoNode*) NULL, -1);
}

/*!
  Get ready to traverse the \a childindex'th child. Use this method
  if you know the path code will not change as a result of this.

  This method is very internal. Do not use unless you know
  what you're doing.
*/
void
SoAction::popPushCurPath(const int childindex, SoNode * node)
{
  this->currentpath.pop(); // pop off previous or NULL node
  if (node == NULL) {
    this->currentpath.append(childindex);
  }
  else {
    this->currentpath.append(node, childindex);
  }
}

/*!
  Pops of the last child in the current path. Use this if
  you know the path code hasn't changed since the current
  path was pushed.

  This method is very internal. Do not use unless you know
  what you're doing.
*/
void
SoAction::popCurPath(void)
{
  this->currentpath.pop();
}

// *************************************************************************

/*!
  Returns a list of the elements used by action instances of this
  class upon traversal operations.
*/
const SoEnabledElementsList &
SoAction::getEnabledElements(void) const
{
  return *(this->enabledElements);
}

/*!
  \internal

  This method not available in the original OIV API, see SoSubAction.h
  for explanation.
 */
SoEnabledElementsList *
SoAction::getClassEnabledElements(void)
{
  return SoAction::enabledElements;
}

/*!
  \internal

  This method not available in the original OIV API, see SoSubAction.h
  for explanation.
 */
SoActionMethodList *
SoAction::getClassActionMethods(void)
{
  return SoAction::methods;
}

/*!
  This virtual method can be overloaded to initialize the action at
  traversal start.  Default method just calls traverse(), which any
  overloaded method must do too (or call SoAction::beginTraversal()).
*/
void
SoAction::beginTraversal(SoNode * node)
{
  this->traverse(node);
}

/*!
  This virtual method can be overloaded to execute code after the
  scene graph traversal.  Default method does nothing.
*/
void
SoAction::endTraversal(SoNode * node)
{
}

/*!
  Set the termination flag.

  Typically set to TRUE from nodes upon special
  conditions being met during scene graph traversal -- like the
  correct node being found when doing SoSearchAction traversal or
  when grabbing the event from an SoHandleEventAction.

  \sa hasTerminated()
*/
void
SoAction::setTerminated(const SbBool flag)
{
  this->terminated = flag;
}

/*!
  \internal
*/
SbBool
SoAction::shouldCompactPathList(void) const
{
  return TRUE;
}

/*!
  Store our state, traverse the given \a path, restore our state and
  continue traversal.
 */
void
SoAction::switchToPathTraversal(SoPath * path)
{
  // Store current state.
  AppliedData storeddata = this->applieddata;
  AppliedCode storedcode = this->appliedcode;
  PathCode storedpathcode = this->currentpathcode;
  SoTempPath storedpath = this->currentpath;

  // Start path traversal. Don't use beginTraversal() (the user might
  // have overloaded it).
  this->appliedcode = SoAction::PATH;
  this->applieddata.path = path;
  this->currentpathcode = SoAction::IN_PATH;
  this->traverse(path->getNode(0));

  // Restore previous state.
  this->currentpath = storedpath;
  this->currentpathcode = storedpathcode;
  this->applieddata = storeddata;
  this->appliedcode = storedcode;
}

/*!
  Store our state, traverse the subgraph rooted at the given \a node,
  restore our state and continue traversal.
 */
void
SoAction::switchToNodeTraversal(SoNode * node)
{
  // Store current state.
  AppliedData storeddata = this->applieddata;
  AppliedCode storedcode = this->appliedcode;
  PathCode storedpathcode = this->currentpathcode;
  SoTempPath storedpath = this->currentpath;

  this->appliedcode = SoAction::NODE;
  this->applieddata.node = node;
  this->currentpathcode = SoAction::NO_PATH;
  this->currentpath.truncate(0);

  this->traverse(node);

  // Restore previous state.
  this->currentpath = storedpath;
  this->currentpathcode = storedpathcode;
  this->applieddata = storeddata;
  this->appliedcode = storedcode;
}
