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
#include <coindefs.h> // COIN_STUB()


#include <assert.h>

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
*/
void
SoAction::apply(SoNode * root)
{
  this->traversalMethods->setUp();
  this->terminated = FALSE;

  // So the graph is not deallocated during traversal.
  if (root) root->ref();

  this->currentpathcode = SoAction::NO_PATH;
  this->applieddata.node = root;
  this->appliedcode = SoAction::NODE;
  if (root) {
    if (!this->state) {
      const SoTypeList & elementtypes =
        this->getEnabledElements().getElements();
      this->state = new SoState(this, elementtypes);
    }

    this->beginTraversal(root);
    this->endTraversal(root);
    this->applieddata.node = NULL;
  }

  if (root) root->unrefNoDelete();
}

/*!
  Applies the action to the parts of the graph defined by \a path.
*/
void
SoAction::apply(SoPath * path)
{
  this->traversalMethods->setUp();
  this->terminated = FALSE;

  // So the path is not deallocated during traversal.
  path->ref();

  this->currentpathcode = SoAction::IN_PATH;
  this->applieddata.path = path;
  this->appliedcode = SoAction::PATH;

  if (!this->state) {
    const SoTypeList & elementtypes = this->getEnabledElements().getElements();
    this->state = new SoState(this, elementtypes);
  }

  if (path->getLength() && path->getNode(0)) {
    SoNode * node = path->getNode(0);
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
  this->traversalMethods->setUp();
  if (pathlist.getLength() == 0) return;

  // FIXME: doesn't check obeysrules. 20000301 mortene.

  this->terminated = FALSE;

  // FIXME: temporary code until proper pathlist traversal is implemented
  assert(pathlist[0]->getNode(0));

  int n = pathlist.getLength();
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());

  for (int i = 0; i < n; i++) {
    SoPath * path = pathlist[i];
    this->currentpathcode = IN_PATH;
    this->applieddata.path = path;
    this->appliedcode = PATH;
    this->state->push();
    if (i == 0) {
      this->beginTraversal(path->getNode(0));
    }
    else this->traverse(path->getNode(0));
    this->state->pop();
    this->applieddata.node = NULL;
  }
  this->endTraversal(pathlist[0]->getNode(0));
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
SoAction::getOriginalPathListAppliedto(void) const
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
  COIN_STUB(); // FIXME
  return FALSE;
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
  assert(node);

  // FIXME: if no action method, try parents in inheritance tree up to
  // SoNode.

  PathCode storedpathcode = this->currentpathcode;

  // FIXME: write code for PathList traversal.

  this->currentpath.append(node);

  switch (this->currentpathcode) {
  case SoAction::IN_PATH:
    {
      int idx = this->currentpath.getFullLength();

      if (this->applieddata.path->getNode(idx - 1) != node) {
        this->currentpathcode = SoAction::OFF_PATH;
      }
      else { // either in or below path
        if (idx == this->applieddata.path->getFullLength()) {
          this->currentpathcode = SoAction::BELOW_PATH;
        }
        else {
          assert(idx < this->applieddata.path->getFullLength());
          assert(node->getChildren());
        }
      }
      break;
    }

  case SoAction::OFF_PATH:
  case SoAction::BELOW_PATH:
  case SoAction::NO_PATH:
    // will stay in this state forever (or until popped)
    break;

  default:
    assert(0 && "Unknown path code");
    break;
  }

  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);

  (*this->traversalMethods)
    [SoNode::getActionMethodIndex(node->getTypeId())](this, node);

  this->currentpathcode = storedpathcode; // restore current path code

  this->currentpath.pop();
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

  Returns the current path code for path traversal.
*/
SoAction::PathCode
SoAction::getCurPathCode(void) const
{
  return this->currentpathcode;
}

/*!
  \internal
*/
void
SoAction::pushCurPath(const int childindex)
{
  this->currentpath.push(childindex);
}

/*!
  internal
*/
void
SoAction::popCurPath(const PathCode prevpathcode)
{
  this->currentpath.pop();
  this->currentpathcode = prevpathcode;
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
  // FIXME: this is for path-traversal only. Code for PathList
  // traversal needs to be written.

  numindices = 1;
  this->pathcodearray[0] =
    this->applieddata.path->getIndex(this->currentpath.getFullLength());
  indices = this->pathcodearray;
}

/*!
  \internal
*/
void
SoAction::pushCurPath(void)
{
  COIN_STUB(); // FIXME
}

/*!
  \internal
*/
void
SoAction::popPushCurPath(const int childindex)
{
  this->currentpath.pop();
  this->currentpath.push(childindex);
}

/*!
  \internal
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
