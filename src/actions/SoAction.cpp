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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
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

#include <Inventor/SoType.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoGroup.h>
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
SoAction::getTypeId() const // virtual
{
  return SoAction::classTypeId;
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

/*!
  This static method should clean up after the SoAction class.  No clean-up
  is implemented.
*/

void
SoAction::cleanClass()
{
  // FIXME: delete enabledElements & methods?

  SoAction::cleanActions();
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
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SoCallbackAction::initClass();
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SoGLRenderAction::initClass();
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION)
  SoBoxHighlightRenderAction::initClass();
#endif // !COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION)
  SoLineHighlightRenderAction::initClass();
#endif // !COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SoGetBoundingBoxAction::initClass();
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  SoGetMatrixAction::initClass();
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  SoGetPrimitiveCountAction::initClass();
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  SoHandleEventAction::initClass();
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SoPickAction::initClass();
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  SoRayPickAction::initClass();
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  SoSearchAction::initClass();
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOSIMPLIFYACTION)
  SoSimplifyAction::initClass();
#endif // !COIN_EXCLUDE_SOSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION)
  SoGlobalSimplifyAction::initClass();
#endif // !COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOREORGANIZEACTION)
  SoReorganizeAction::initClass();
#endif // !COIN_EXCLUDE_SOREORGANIZEACTION
#if !defined(COIN_EXCLUDE_SOSHAPESIMPLIFYACTION)
  SoShapeSimplifyAction::initClass();
#endif // !COIN_EXCLUDE_SOSHAPESIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOTOVRMLACTION)
  SoToVRMLAction::initClass();
#endif // !COIN_EXCLUDE_SOTOVRMLACTION
#if !defined(COIN_EXCLUDE_SOTOVRML2ACTION)
  SoToVRML2Action::initClass();
#endif // !COIN_EXCLUDE_SOTOVRML2ACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  SoWriteAction::initClass();
#endif // !COIN_EXCLUDE_SOWRITEACTION
}

/*!
  This static method cleans up all the SoAction classes.
*/

void
SoAction::cleanActions()
{
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  SoWriteAction::cleanClass();
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOTOVRML2ACTION)
  SoToVRML2Action::cleanClass();
#endif // !COIN_EXCLUDE_SOTOVRML2ACTION
#if !defined(COIN_EXCLUDE_SOTOVRMLACTION)
  SoToVRMLAction::cleanClass();
#endif // !COIN_EXCLUDE_SOTOVRMLACTION
#if !defined(COIN_EXCLUDE_SOSHAPESIMPLIFYACTION)
  SoShapeSimplifyAction::cleanClass();
#endif // !COIN_EXCLUDE_SOSHAPESIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOREORGANIZEACTION)
  SoReorganizeAction::cleanClass();
#endif // !COIN_EXCLUDE_SOREORGANIZEACTION
#if !defined(COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION)
  SoGlobalSimplifyAction::cleanClass();
#endif // !COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOSIMPLIFYACTION)
  SoSimplifyAction::cleanClass();
#endif // !COIN_EXCLUDE_SOSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  SoSearchAction::cleanClass();
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  SoRayPickAction::cleanClass();
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SoPickAction::cleanClass();
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  SoHandleEventAction::cleanClass();
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  SoGetPrimitiveCountAction::cleanClass();
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  SoGetMatrixAction::cleanClass();
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SoGetBoundingBoxAction::cleanClass();
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION)
  SoLineHighlightRenderAction::cleanClass();
#endif // !COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION)
  SoBoxHighlightRenderAction::cleanClass();
#endif // !COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SoGLRenderAction::cleanClass();
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SoCallbackAction::cleanClass();
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
}

// *************************************************************************

/*!
  The constructor.
*/

SoAction::SoAction()
  : state(NULL), 
    traversalMethods(NULL),
    appliedCode(NODE),
    currentPathCode(NO_PATH),
    isTerminated(TRUE)
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
#if 0 // FIXME: doesn't work with a rootNode == NULL. 19990819 mortene.
  assert(rootNode);
  rootNode->ref();
  this->currentPathCode = NO_PATH;
  this->appliedData.node = rootNode;
  this->appliedCode = NODE;
  this->isTerminated = FALSE;
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());
  this->beginTraversal(rootNode);
  this->endTraversal(rootNode);
  this->appliedData.node = NULL;
  this->isTerminated = TRUE;
  // An action should not trigger node (or node tree) destruction.
  rootNode->unrefNoDelete();
#else // FIXME: Handles rootNode == NULL gracefully, not sure if that is a Good Thing, though. 19990819 mortene.
  if (rootNode) rootNode->ref();
  this->currentPathCode = NO_PATH;
  this->appliedData.node = rootNode;
  this->appliedCode = NODE;
  if (rootNode) {
    this->isTerminated = FALSE;
    if (this->state == NULL)
      this->state = new SoState(this, getEnabledElements().getElements());
    this->beginTraversal(rootNode);
    this->endTraversal(rootNode);
    this->appliedData.node = NULL;
  }
  this->isTerminated = TRUE;
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
  // ref path's root node?
  assert(path->getLength() > 0);
  assert(path->getNode(0));
  this->currentPathCode = IN_PATH;
  this->appliedData.path = path;
  this->appliedCode = PATH;
  this->isTerminated = FALSE;
  if (this->state == NULL)
    this->state = new SoState(this, getEnabledElements().getElements());
  this->beginTraversal(path->getNode(0));
  this->endTraversal(path->getNode(0));
  this->isTerminated = TRUE;
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
SoAction::apply(const SoPathList & pathList, SbBool obeysRules)
{
  // FIXME: temporary code until proper pathlist traversal is implemented
  assert(pathList.getLength() > 0);
  assert(pathList[0]->getNode(0));

  int n = pathList.getLength();
  this->isTerminated = FALSE;
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
  this->isTerminated = TRUE;
}

/*!
  This function invalidates the state, forcing it to be recreated at the
  next apply.  This is typically unnecessary in most applications.
*/

void
SoAction::invalidateState()
{
  assert(0 && "FIXME: not implemented");
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
  assert(0 && "FIXME: Not implemented");
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
      int idx = this->currentPath.getLength();

#if 0 // debug
      fprintf(stderr, "currpathlen: %d, pathlen: %d, node: %p, pathnode %p\n",
	      idx,
	      this->appliedData.path->getLength(),
	      node,
	      this->appliedData.path->getNode(idx-1));
#endif      

      if (this->appliedData.path->getNode(idx-1) != node) {
	this->currentPathCode = OFF_PATH;
      }
      else { // either in or below path
	if (idx == this->appliedData.path->getLength()) {
	  this->currentPathCode = BELOW_PATH;
	}
	else {
	  assert(idx < this->appliedData.path->getLength());
	  assert(node->isOfType(SoGroup::getClassTypeId()));
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
  

#if 0 // debug
  
  if (this->currentPathCode)
    fprintf(stderr, "path code: %d\n", this->currentPathCode);
  
#endif

  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  
  (*this->traversalMethods)
    [SoNode::getActionMethodIndex(node->getTypeId())](this, node); 

  this->currentPathCode = storedPathCode; // restore current path code
  this->nextInPathChildIndex = storedIndex;

  // FIXME: code to traverse VRML2 node fields.

  this->currentPath.pop();
}

/*!
  This method returns TRUE if the action has terminated.
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
  assert(0 && "FIXME: Not implemented");
}

/*!
  FIXME: write doc.
*/

void
SoAction::popCurPath(const PathCode prevPathCode)
{
  assert(0 && "FIXME: Not implemented");
}

/*!
  FIXME: write doc.
*/

SoNode *
SoAction::getCurPathTail()
{
  assert(0 && "FIXME: not implemented");
  return NULL;
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
  dummyArray[0] = this->nextInPathChildIndex;
  indices = dummyArray;
}

/*!
  FIXME: write doc.
*/

void 
SoAction::pushCurPath()
{ 
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc.
*/

void 
SoAction::popPushCurPath(const int childIndex)
{
  assert(0 && "FIXME: not implemented"); 
}

/*!
  FIXME: write doc.
*/

void 
SoAction::popCurPath()
{ 
  assert(0 && "FIXME: not implemented");
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
  traverse(node);
}

/*!
  This virtual method can be overloaded to execute code after the scene graph
  traversal.  Default method does nothing.
*/

void
SoAction::endTraversal(SoNode * node)
{
}

/*!
  FIXME: write doc.
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
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
