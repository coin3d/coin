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
  \class SoAction Inventor/actions/SoAction.h
  \brief The SoAction class is the base class for all traversal methods.

  TODO: doc

  ...When traversing, lookup is done based on the node type and the
  action type...
*/

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoActions.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/SoType.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


#include <assert.h>

// *************************************************************************

// static variables
SoEnabledElementsList * SoAction::enabledElements;
SoActionMethodList * SoAction::methods;
SoType SoAction::classTypeId;

// *************************************************************************

/*!
  \fn SoAction::AppliedCode

  FIXME: write doc
*/

/*!
  \fn SoAction::PathCode

  FIXME: write doc.
*/

/*!
  \fn SoAction::state

  FIXME: write doc.
*/

/*!
  \fn SoAction::traversalMethods

  FIXME: write doc.
*/

/*!
  \fn SoAction::enabledElements

  FIXME: write doc.
*/

/*!
  \fn SoAction::methods

  FIXME: write doc.
*/

// *************************************************************************

/*!
  This static method returns the SoType run-time type object for the given
  action class.
*/

SoType
SoAction::getClassTypeId()
{
  return SoAction::classTypeId;
}

/*!
  This virtual method returns the SoType run-time type object for the given
  action instance.
*/

SoType
SoAction::getTypeId() const
{
  return SoAction::classTypeId;
}

/*!
    Returns \c TRUE if the type of this object is either of the same
    type or inherited from \a type.
*/
SbBool
SoAction::isOfType(SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  This static method initializes the run-time type system for the SoAction
  class, and sets up / prepares the enabled elements and action method list.
*/

void
SoAction::initClass()
{
  SoAction::classTypeId =
    SoType::createType(SoType::badType(),
                       SbName("SoAction"));

  SoAction::enabledElements = new SoEnabledElementsList(NULL);
  SoAction::methods = new SoActionMethodList(NULL);

  // Only element enabled in SoAction. Confirmed correct against
  // OIV. 19990213 mortene.
  SoAction::enabledElements->enable(SoOverrideElement::getClassTypeId(),
                                    SoOverrideElement::getClassStackIndex());

  SoAction::initActions();
}

// *************************************************************************

/*!
  \fn void SoAction::initClasses(void)

  For OI compatibility - just forwards the call to SoAction::initActions().

  \sa void SoAction::initActions()
*/

/*!
  This static method initializes all the SoAction classes.
*/

void
SoAction::initActions()
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
  SoSimplifyAction::initClass();
  SoGlobalSimplifyAction::initClass();
  SoReorganizeAction::initClass();
  SoShapeSimplifyAction::initClass();
  SoToVRMLAction::initClass();
  SoToVRML2Action::initClass();
  SoWriteAction::initClass();
}

// *************************************************************************

/*!
  The constructor.
*/

SoAction::SoAction()
  : state(NULL),
    traversalMethods(NULL),
    appliedCode(NODE),
    currentPath(8),
    isTerminated(FALSE),
    currentPathCode(NO_PATH)
{
  appliedData.node = NULL;
}

/*!
  The destructor.
*/

SoAction::~SoAction(void)
{
  delete this->state;
}

// *************************************************************************

/*!
  This method applies action to the graph rooted at rootNode.
*/

void
SoAction::apply(SoNode * rootNode)
{
  this->isTerminated = FALSE;
#if 0 // FIXME: doesn't work with a rootNode == NULL. 19990819 mortene.
  assert(rootNode);
  rootNode->ref();
  this->currentPathCode = NO_PATH;
  this->appliedData.node = rootNode;
  this->appliedCode = NODE;
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());
  this->beginTraversal(rootNode);
  this->endTraversal(rootNode);
  this->appliedData.node = NULL;
  // An action should not trigger node (or node tree) destruction.
  rootNode->unrefNoDelete();
#else // FIXME: Handles rootNode == NULL gracefully, not sure if that is a Good Thing, though. 19990819 mortene.

  if (rootNode) rootNode->ref();
  this->currentPathCode = NO_PATH;
  this->appliedData.node = rootNode;
  this->appliedCode = NODE;
  if (rootNode) {
    if (this->state == NULL)
      this->state = new SoState(this, getEnabledElements().getElements());
    this->beginTraversal(rootNode);
    this->endTraversal(rootNode);
    this->appliedData.node = NULL;
  }
  // An action should not trigger node (or node tree) destruction.
  if (rootNode) rootNode->unrefNoDelete();
#endif
}

/*!
  This function applies action to the graph defined by path.
*/

void
SoAction::apply(SoPath * path)
{
  this->isTerminated = FALSE;

  // ref path's root node?
  assert(path->getLength() > 0);
  assert(path->getNode(0));
  this->currentPathCode = IN_PATH;
  this->appliedData.path = path;
  this->appliedCode = PATH;
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());
  this->beginTraversal(path->getNode(0));
  this->endTraversal(path->getNode(0));
}

/*!
  This function applies action to the graphs defined by a path list.  If
  obeysRules is set to TRUE, pathList must obey the following four conditions
  (which is the case for path lists returned from search actions for non-group
  nodes and path lists returned from picking actions).

  *) all paths must have the same head node.
  *) all paths must be sorted in traversal order.
  *) all paths must be unique.
  *) no path must continue through the end point of another path.
*/

void
SoAction::apply(const SoPathList & pathList, SbBool /* obeysRules */)
{
  this->isTerminated = FALSE;

  // FIXME: temporary code until proper pathlist traversal is implemented
  assert(pathList.getLength() > 0);
  assert(pathList[0]->getNode(0));

  int n = pathList.getLength();
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());

  for (int i = 0; i < n; i++) {
    SoPath * path = pathList[i];
    this->currentPathCode = IN_PATH;
    this->appliedData.path = path;
    this->appliedCode = PATH;
    this->state->push();
    if (i == 0) {
      this->beginTraversal(path->getNode(0));
    }
    else this->traverse(path->getNode(0));
    this->state->pop();
    this->appliedData.node = NULL;
  }
  this->endTraversal(pathList[0]->getNode(0));
}

/*!
  This function invalidates the state, forcing it to be recreated at the
  next apply.  This is typically unnecessary in most applications.
*/

void
SoAction::invalidateState()
{
  COIN_STUB();
  // do what here?
  // probable use - if the list of enabled elements changes for an action,
  // the state must be destroyed and recreated at next apply()
}

// *************************************************************************

/*!
  This method is for filling into the lookup tables.
  It does nothing, except for traversing the graph.
*/

void
SoAction::nullAction(SoAction *, SoNode *)
{
}

/*!
  This method returns a code indicating what (node, path, or pathlist)
  the action instance is being applied to.
*/

SoAction::AppliedCode
SoAction::getWhatAppliedTo() const
{
  return this->appliedCode;
}

/*!
  This method returns a pointer to the node the action is being applied to.
  If action was not applied to a node (but a path or a pathlist), the method
  returns NULL.
*/

SoNode *
SoAction::getNodeAppliedTo() const
{
  return this->appliedData.node;
}

/*!
  This method returns a pointer to the path the action is being applied to.
  If action was not applied to a path (but a node or a pathlist), the method
  returns NULL.
*/

SoPath *
SoAction::getPathAppliedTo() const
{
  return this->appliedData.path;
}

/*!
  This method returns a pointer to the path list the action is currently
  being applied to.  If action was not applied to a path list (but a node
  or a path), the method returns NULL.  The path list is not necessarily the
  one SoAction::apply() was called with - the action may have reorganized
  the path list for efficiency reasons (to conform with the four rules
  mentioned for SoAction::apply()).

  \sa void SoAction::apply(const SoPathList & pathList, SbBool obeysRules)
*/

const SoPathList *
SoAction::getPathListAppliedTo() const
{
  return this->appliedData.pathListData.pathList;
}

/*!
  This method returns a pointer to the original path list the action is
  being applied to.  If the action was not applied to a path list (but a node
  or a path), the method returns NULL.
*/

const SoPathList *
SoAction::getOriginalPathListAppliedto() const
{
  return this->appliedData.pathListData.origPathList;
}

/*!
  This method returns TRUE if the current path list being applied is the
  last path list (generated) from the original path list.
*/

SbBool
SoAction::isLastPathListAppliedTo() const
{
  COIN_STUB();
  return FALSE;
}

/*!
  This method returns a code that indicates where the current node lies with
  respect to the path(s) the action is being applied to.  The arguments
  \a indices and \a numIndices are only set if the method returns IN_PATH.
*/

SoAction::PathCode
SoAction::getPathCode(int & numIndices, const int * & indices)
{
  if (this->currentPathCode == IN_PATH) {
    usePathCode(numIndices, indices);
  }
  return this->currentPathCode;
}

/*!
  This method traverses a graph rooted at node.
*/

void
SoAction::traverse(SoNode * const node)
{
  //FIXME: if no action method, try parents in inheritance tree up to SoNode
  //SoDebugError::postInfo("SoAction::traverse", "%p\n", node);
  assert(node != NULL);
  PathCode storedPathCode = this->currentPathCode; // save current path code
  int storedIndex = this->nextInPathChildIndex;

  //
  // TODO: write code for PathList
  //

  this->currentPath.append(node);

  switch (this->currentPathCode) {
  case IN_PATH:
    {
      int idx = this->currentPath.getFullLength();

      if (this->appliedData.path->getNode(idx-1) != node) {
        this->currentPathCode = OFF_PATH;
      }
      else { // either in or below path
        if (idx == this->appliedData.path->getFullLength()) {
          this->currentPathCode = BELOW_PATH;
        }
        else {
          assert(idx < this->appliedData.path->getFullLength());
          assert(node->getChildren());
          this->nextInPathChildIndex = this->appliedData.path->getIndex(idx);
        }
      }
      break;
    }
  case OFF_PATH:
  case BELOW_PATH:
  case NO_PATH:
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

#if 0 // debug
  {
    int midx = SoNode::getActionMethodIndex(node->getTypeId());
    const SoActionMethod m = (*this->traversalMethods)[midx];
    SbString mname;
    if (m == SoNode::pickS) mname = "pickS";
    else if (m == SoNode::rayPickS) mname = "rayPickS";
    else if (m == SoNode::handleEventS) mname = "handleEventS";
    else mname.sprintf("%p", m);

    SoDebugError::postInfo("SoAction::traverse",
                           "%s %s actionmethodindex: %d (func: %s)",
                           node->getTypeId().getName().getString(),
                           this->getTypeId().getName().getString(),
                           midx,
                           mname.getString());

    SoDebugError::postInfo("SoAction::traverse",
                           "traversalMethods: %p", traversalMethods);

    traversalMethods->dump_list();
  }
#endif // debug
  (*this->traversalMethods)
    [SoNode::getActionMethodIndex(node->getTypeId())](this, node);

  this->currentPathCode = storedPathCode; // restore current path code
  this->nextInPathChildIndex = storedIndex;

  // FIXME: code to traverse VRML2 node fields.

  this->currentPath.pop();
}

/*!
  This method returns TRUE if the action was prematurely terminated.

  Note that the termination flag will be FALSE if the action simply
  completed its run over the scene graph in the "ordinary" fashion,
  i.e. was not explicitly aborted from any of the nodes in the graph.

  \sa setTerminated()
*/
SbBool
SoAction::hasTerminated() const
{
  return this->isTerminated;
}

/*!
  This method returns a pointer to the state of the action instance.
*/

SoState *
SoAction::getState() const
{
  return this->state;
}

/*!
  This method returns a pointer to the path from the root of the traversed
  graph to the current node.
*/

const SoPath *
SoAction::getCurPath()
{
  return &this->currentPath;
}

/*!
  FIXME: write doc.
*/

SoAction::PathCode
SoAction::getCurPathCode() const
{
  return this->currentPathCode;
}

/*!
  FIXME: write doc.
*/

void
SoAction::pushCurPath(const int childIndex)
{
  this->currentPath.push(childIndex);
}

/*!
  FIXME: write doc.
*/

void
SoAction::popCurPath(const PathCode prevPathCode)
{
  this->currentPath.pop();
  this->currentPathCode = prevPathCode;
}

/*!
  FIXME: write doc.
*/

SoNode *
SoAction::getCurPathTail()
{
  return this->currentPath.getTail();
}

/*!
  FIXME: write doc.
*/

void
SoAction::usePathCode(int & numIndices, const int * & indices)
{
  //
  // FIXME: this is for path-traversal only.
  // code for PathList traversal needs to be written
  //

  numIndices = 1;
#if 0 // OBSOLETED 19991108, code won't work for SoArray and SoMultipleCopy
  dummyArray[0] = this->nextInPathChildIndex;
#else // new, more flexible code
  dummyArray[0] = this->appliedData.path->getIndex(this->currentPath.getFullLength());
#endif // end of new code
  indices = dummyArray;
}

/*!
  FIXME: write doc.
*/

void
SoAction::pushCurPath(void)
{
  COIN_STUB();
}

/*!
  FIXME: write doc.
*/

void
SoAction::popPushCurPath(const int childIndex)
{
  this->currentPath.pop();
  this->currentPath.push(childIndex);
}

/*!
  FIXME: write doc.
*/

void
SoAction::popCurPath(void)
{
  this->currentPath.pop();
}

// *************************************************************************

/*!
  FIXME: write doc.
*/

const SoEnabledElementsList &
SoAction::getEnabledElements() const
{
  return *(this->enabledElements);
}

/*!
  This virtual method can be overloaded to initialize the action at traversal
  start.  Default method just calls traverse(node), which any overloaded
  method must do too (or call inherited::beginTraversal(node)).
*/

void
SoAction::beginTraversal(SoNode * node)
{
  this->traverse(node);
}

/*!
  This virtual method can be overloaded to execute code after the scene graph
  traversal.  Default method does nothing.
*/

void
SoAction::endTraversal(SoNode * /* node */)
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
  this->isTerminated = flag;
}

/*!
  FIXME: write doc.
*/

SbBool
SoAction::shouldCompactPathList() const
{
  COIN_STUB();
  return FALSE;
}

void
SoAction::switchToPathTraversal(SoPath *path)
{
  AppliedData oldData = this->appliedData;
  AppliedCode oldCode = this->appliedCode;
  PathCode oldPathCode = this->currentPathCode;
  SoTempPath oldPath = this->currentPath;

  this->appliedCode = SoAction::PATH;
  this->appliedData.path = path;
  this->currentPathCode = SoAction::IN_PATH;

  this->traverse(path->getNode(0));

  // restore previous state
  this->currentPath = oldPath;
  this->currentPathCode = oldPathCode;
  this->appliedData = oldData;
  this->appliedCode = oldCode;
}

void
SoAction::switchToNodeTraversal(SoNode *node)
{
  AppliedData oldData = this->appliedData;
  AppliedCode oldCode = this->appliedCode;
  PathCode oldPathCode = this->currentPathCode;
  SoTempPath oldPath = this->currentPath;

  this->appliedCode = SoAction::NODE;
  this->appliedData.node = node;
  this->currentPathCode = SoAction::NO_PATH;
  this->currentPath.truncate(0);

  this->traverse(node);

  // restore previous state
  this->currentPath = oldPath;
  this->currentPathCode = oldPathCode;
  this->appliedData = oldData;
  this->appliedCode = oldCode;
}
