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
  \class SoAction SoAction.h Inventor/actions/SoAction.h
  \brief The SoAction class is the base class for all traversal actions.
  \ingroup actions

  Applying actions is the basic mechanism in Coin for executing
  various operations on scene graphs or paths within scene graphs,
  including search operations, rendering, interaction through picking,
  etc.

  The basic operation is to instantiate an action, set it up with
  miscellaneous parameters if necessary, then call it's apply() method
  on the root node of the scenegraph (or sub-graph of a scenegraph).
  The action then traverses the scenegraph from the root node,
  depth-first and left-to-right, applying it's specific processing at
  the nodes where it is applicable.

  (The SoAction and it's derived classes in Coin is an implementation
  of the design pattern commonly known as the "Visitor" pattern.)

  Here's a simple example that shows how to use the SoWriteAction to
  dump a scenegraph in the Inventor format to a file:

  \code
   int write_scenegraph(const char * filename, SoNode * root)
   {
     SoOutput output;
     if (output.openFile(filename)) return 0;

     // This is where the action is.  ;-)
     SoWriteAction wa(&output);
     wa.apply(root);

     return 1;
   }
  \endcode

  After traversal, some action types have stored information about the
  (sub-)scenegraph that was traversed, which you can then inquire
  about through methods like SoGetBoundingBoxAction::getBoundingBox(),
  SoRayPickAction::getPickedPoint(),
  SoGetPrimitiveCountAction::getTriangleCount(), etc etc.

  See the various built-in actions for further information (ie the
  subclasses of this class), or look at the example code applications
  of the Coin library to see how actions are generally used.

  \TOOLMAKER_REF

  The following example shows the basic outline on how to set up your
  own extension action class:

  \code
  // This is sample code on how you can get progress indication on Coin
  // export operations by extending the library with your own action
  // class. The new class inherits SoWriteAction. The code is presented
  // as a stand-alone example.
  //
  // The general technique is to inherit SoWriteAction and override it's
  // "entry point" into each node of the scenegraph. The granularity of
  // the progress callbacks is on a per-node basis, which should usually
  // be good enough.
  
  #include <Inventor/SoDB.h>
  #include <Inventor/actions/SoWriteAction.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  
  //// Definition of extension class "MyWriteAction" ///////////////

  class MyWriteAction : public SoWriteAction {
    SO_ACTION_HEADER(SoWriteAction);
  
  public:
    MyWriteAction(SoOutput * out);
    virtual ~MyWriteAction();
  
    static void initClass(void);
  
  protected:
    virtual void beginTraversal(SoNode * node);
  
  private:
    static void actionMethod(SoAction *, SoNode *);
    int nrnodes;
    int totalnrnodes;
  };
  
  //// Implementation of extension class "MyWriteAction" ///////////

  SO_ACTION_SOURCE(MyWriteAction);
  
  MyWriteAction::MyWriteAction(SoOutput * out)
    : SoWriteAction(out)
  {
    SO_ACTION_CONSTRUCTOR(MyWriteAction);
  }
  
  MyWriteAction::~MyWriteAction()
  {
  }
  
  void
  MyWriteAction::initClass(void)
  {
    SO_ACTION_INIT_CLASS(MyWriteAction, SoWriteAction);
  
    SO_ACTION_ADD_METHOD(SoNode, MyWriteAction::actionMethod);
  }
  
  void
  MyWriteAction::beginTraversal(SoNode * node)
  {
    this->nrnodes = 0;
    this->totalnrnodes = 0;
    SoWriteAction::beginTraversal(node);
  }
  
  void
  MyWriteAction::actionMethod(SoAction * a, SoNode * n)
  {
    // To abort the export process in mid-writing, we could just avoid
    // calling in to the SoNode::writeS() method.
    SoNode::writeS(a, n);
  
    MyWriteAction * mwa = (MyWriteAction *)a;
    SoOutput * out = mwa->getOutput();
    if (out->getStage() == SoOutput::COUNT_REFS) {
      mwa->totalnrnodes++;
    }
    else { //  (out->getStage() == SoOutput::WRITE)
      mwa->nrnodes++;
      SbString s;
      s.sprintf(" # wrote node %p (%d/%d) \n", n, mwa->nrnodes, mwa->totalnrnodes);
      out->write(s.getString());
    }
  }
  
  //// main ////////////////////////////////////////////////////////

  int
  main(int argc, char ** argv)
  {
    if (argc < 2) {
      (void)fprintf(stderr, "\n\nUsage: %s <filename>\n\n", argv[0]);
      exit(1);
    }
  
    SoDB::init();
    MyWriteAction::initClass();
  
    SoInput in;
    if (!in.openFile(argv[1])) { exit(1); }
  
    SoSeparator * root = SoDB::readAll(&in);
    if (!root) { exit(1); }
  
    root->ref();
  
    SoOutput out;
    MyWriteAction mwa(&out);
    mwa.apply(root);
  
    root->unref();
  
    return 0;
  }
  \endcode

*/

#include <Inventor/actions/SoActions.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SbList.h>

#include <assert.h>
#include <stdlib.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_OBSOLETED

// define this to debug path traversal
// #define DEBUG_PATH_TRAVERSAL

// *************************************************************************

SoEnabledElementsList * SoAction::enabledElements = NULL;
SoActionMethodList * SoAction::methods = NULL;
// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoAction::classTypeId;

// *************************************************************************

// Note: the following documentation for getTypeId() will also be
// visible for subclasses, so keep it general.
/*!
  \fn SoType SoAction::getTypeId(void) const

  Returns the type identification of an action derived from a class
  inheriting SoAction.  This is used for run-time type checking and
  "downward" casting.

  Usage example:

  \code
  void bar(SoAction * action)
  {
    if (action->getTypeId() == SoGLRenderAction::getClassTypeId()) {
      // safe downward cast, know the type
      SoGLRenderAction * glrender = (SoGLRenderAction *)action;
      /// [then something] ///
    }
    return; // ignore if not renderaction
  }
  \endcode


  For application programmers wanting to extend the library with new
  actions: this method needs to be overridden in \e all
  subclasses. This is typically done as part of setting up the full
  type system for extension classes, which is usually accomplished by
  using the pre-defined macros available through
  Inventor/nodes/SoSubAction.h: SO_ACTION_SOURCE, SO_ACTION_INIT_CLASS
  and SO_ACTION_CONSTRUCTOR.

  For more information on writing Coin extensions, see the SoAction
  class documentation.
*/

/*!
  \fn SoType SoAction::getTypeId(void) const

  Returns the actual type id of an object derived from a class
  inheriting SoAction. Needs to be overridden in \e all subclasses.
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


#ifndef DOXYGEN_SKIP_THIS

// Not all private class members are stored in this pimpl. This is
// done because we need to have some of the access function inlined to
// achieve better traversal speed. pederb, 2002-07-02
class SoActionP {
public:
  SoAction::AppliedCode appliedcode;
  union AppliedData {
    SoNode * node;
    SoPath * path;
    struct {
      const SoPathList * pathlist;
      const SoPathList * origpathlist;
    } pathlistdata;
  } applieddata;
  SbBool terminated;
  SbList <SbList<int> *> pathcodearray;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Default constructor, does all necessary toplevel initialization.
*/
SoAction::SoAction(void)
  : state(NULL),
    traversalMethods(NULL),
    currentpath(8),
    currentpathcode(NO_PATH)
{
  THIS = new SoActionP;
  THIS->appliedcode = NODE;
  THIS->applieddata.node = NULL;
  THIS->terminated = FALSE;

  this->currentpath.ref(); // to avoid having a zero refcount instance
}

/*!
  Destructor, free resources.
*/
SoAction::~SoAction(void)
{
  int n = THIS->pathcodearray.getLength();
  for (int i = 0; i < n; i++) delete THIS->pathcodearray[i];
  delete this->state;
  
  delete THIS;
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
  SoAudioRenderAction::initClass();
  SoIntersectionDetectionAction::initClass();
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
  // need to store these in case action is re-applied
  AppliedCode storedcode = THIS->appliedcode;
  SoActionP::AppliedData storeddata = THIS->applieddata;
  PathCode storedcurr = this->currentpathcode;

  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  this->traversalMethods->setUp();
  THIS->terminated = FALSE;

  this->currentpathcode = SoAction::NO_PATH;
  THIS->applieddata.node = root;
  THIS->appliedcode = SoAction::NODE;

  if (root) {
#if COIN_DEBUG
    static SbBool first = TRUE;
    if ((root->getRefCount() == 0) && first) {

      // This problem has turned out to be a FAQ, the reason probably
      // being that it "works" under SGI / TGS Inventor with no
      // warning that the client application code is actually buggy.
      //
      // We prefer to spit out a verbose warning to aid the
      // application programmer in finding the bug quickly instead of
      // her having to track down the bug due to some _really_ nasty
      // sideeffects later.

      SoDebugError::postWarning("SoAction::apply",

                                "The root node that the %s was applied to "
                                "has a reference count equal to zero. "

                                "This is a bug in your application code which "
                                "you should rectify: you need to ref() (and "
                                "later unref()) the top-level root node to "
                                "make sure you avoid memory leaks (bad) and "
                                "/ or premature memory destruction (*really* "
                                "bad) under certain conditions. "

                                "Coin has an internal workaround to avoid "
                                "just responding with mysterious crashes, "
                                "but as it is not possible to cover _all_ "
                                "cases of what can go wrong with this "
                                "workaround you are *strongly* advised to "
                                "fix the bug in your application code.",

                                this->getTypeId().getName().getString());
      first = FALSE;
    }
#endif // COIN_DEBUG
    // So the graph is not deallocated during traversal.
    root->ref();
    this->currentpath.setHead(root);
    // make sure state is created before traversing
    (void) this->getState();
    this->beginTraversal(root);
    this->endTraversal(root);
    THIS->applieddata.node = NULL;
    root->unrefNoDelete();
  }
  THIS->appliedcode = storedcode;
  THIS->applieddata = storeddata;
  this->currentpathcode = storedcurr;
}

/*!
  Applies the action to the parts of the graph defined by \a path.

  If the \a path ends in an SoGroup node, the action will also
  traverse the tail node's children.
*/
void
SoAction::apply(SoPath * path)
{
  // need to store these in case action in reapplied
  AppliedCode storedcode = THIS->appliedcode;
  SoActionP::AppliedData storeddata = THIS->applieddata;
  PathCode storedcurr = this->currentpathcode;

  // This is a pretty good indicator on whether or not we remembered
  // to use the SO_ACTION_CONSTRUCTOR() macro in the constructor of
  // the SoAction subclass.
  assert(this->traversalMethods);
  this->traversalMethods->setUp();
  THIS->terminated = FALSE;

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
  THIS->applieddata.path = path;
  THIS->appliedcode = SoAction::PATH;

  // make sure state is created before traversing
  (void) this->getState();

  if (path->getLength() && path->getNode(0)) {
    SoNode * node = path->getNode(0);
    this->currentpath.setHead(node);
    this->beginTraversal(node);
    this->endTraversal(node);
  }

  path->unrefNoDelete();
  THIS->appliedcode = storedcode;
  THIS->applieddata = storeddata;
  this->currentpathcode = storedcurr;
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

  // need to store these in case action in reapplied
  AppliedCode storedcode = THIS->appliedcode;
  SoActionP::AppliedData storeddata = THIS->applieddata;
  PathCode storedcurr = this->currentpathcode;

  THIS->terminated = FALSE;

  // make sure state is created before traversing
  (void) this->getState();

  THIS->applieddata.pathlistdata.origpathlist = &pathlist;
  THIS->applieddata.pathlistdata.pathlist = &pathlist;
  THIS->appliedcode = PATH_LIST;
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
      THIS->applieddata.pathlistdata.pathlist = &sortedlist;
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
        THIS->applieddata.pathlistdata.pathlist = &templist;
        THIS->appliedcode = PATH_LIST;
        this->currentpathcode = templist[0]->getFullLength() > 1 ?
          SoAction::IN_PATH : SoAction::BELOW_PATH;
        this->currentpath.setHead(templist[0]->getHead());
        this->beginTraversal(templist[0]->getHead());
        templist.truncate(0);
      }
    }
  }
  THIS->appliedcode = storedcode;
  THIS->applieddata = storeddata;
  this->currentpathcode = storedcurr;
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
  return THIS->appliedcode;
}

/*!
  Returns a pointer to the node the action is being applied to.

  If action is not being applied to a node (but a path or a pathlist),
  the method returns \c NULL.
*/
SoNode *
SoAction::getNodeAppliedTo(void) const
{
  return THIS->appliedcode == SoAction::NODE ? THIS->applieddata.node : NULL;
}

/*!
  Returns the pointer to the path the action is being applied to.
  The path is managed by the action instance and should not be destroyed or
  modified by the caller.

  If action is not being applied to a path (but a node or a pathlist),
  the method returns \c NULL.
*/
SoPath *
SoAction::getPathAppliedTo(void) const
{
  return THIS->appliedcode == SoAction::PATH ? THIS->applieddata.path : NULL;
}

/*!
  Returns the pointer to the path list the action is currently being
  applied to.  The path list is managed by the action instance and
  should not be destroyed or modified by the caller.

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
  return THIS->appliedcode == SoAction::PATH_LIST ?
    THIS->applieddata.pathlistdata.pathlist : NULL;
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
  return THIS->appliedcode == SoAction::PATH_LIST ?
    THIS->applieddata.pathlistdata.origpathlist : NULL;
}

/*!
  This method is not supported in Coin. It should probably
  have been private in OIV.
*/
SbBool
SoAction::isLastPathListAppliedTo(void) const
{
  COIN_OBSOLETED();
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
  SoType t = node->getTypeId();
  int idx = SoNode::getActionMethodIndex(t);
  SoActionMethod func = (*this->traversalMethods)[idx];

  func(this, node);
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
  if (node) this->currentpath.simpleAppend(node, childindex);
  else {
    this->currentpath.append(childindex);
  }
  int curlen = this->currentpath.getFullLength();

  if (this->currentpathcode == IN_PATH) {
    if (this->getWhatAppliedTo() == PATH) {
      assert(curlen <= THIS->applieddata.path->getFullLength());
      if (this->currentpath.getIndex(curlen-1) !=
          THIS->applieddata.path->getIndex(curlen-1)) {
#ifdef DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"off path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // DEBUG_PATH_TRAVERSAL
        this->currentpathcode = OFF_PATH;
      }
      else if (curlen == THIS->applieddata.path->getFullLength()) {
        this->currentpathcode = BELOW_PATH;
#ifdef DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"below path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(),curlen);
#endif // DEBUG_PATH_TRAVERSAL
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
      const SoPathList * pl = THIS->applieddata.pathlistdata.pathlist;
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
#ifdef DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"off path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // DEBUG_PATH_TRAVERSAL
      }
      else if (len == curlen) {
        this->currentpathcode = BELOW_PATH;
#ifdef DEBUG_PATH_TRAVERSAL
        fprintf(stderr,"below path at: %d (%s), depth: %d\n",
                childindex, node->getName().getString(), curlen);
#endif // DEBUG_PATH_TRAVERSAL
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
  return THIS->terminated;
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
  \COININTERNAL
*/
SoNode *
SoAction::getCurPathTail(void)
{
  return this->currentpath.getTail();
}

/*!
  \COININTERNAL
*/
void
SoAction::usePathCode(int & numindices, const int * & indices)
{
  int curlen = this->currentpath.getFullLength();

  while (THIS->pathcodearray.getLength() < curlen) {
    THIS->pathcodearray.append(new SbList<int>);
  }

  SbList <int> * myarray = THIS->pathcodearray[curlen-1];
  myarray->truncate(0);

  if (this->getWhatAppliedTo() == PATH_LIST) {
    // this might be very slow if the list contains a lot of
    // paths. See comment in pushCurPath(int, SoNode*) about this.
    const SoPathList * pl = THIS->applieddata.pathlistdata.pathlist;
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
    myarray->append(THIS->applieddata.path->getIndex(curlen));
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
  this->currentpath.simpleAppend((SoNode*) NULL, -1);
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
  if (node == NULL) {
    this->currentpath.pop(); // pop off previous or NULL node
    this->currentpath.append(childindex);
  }
  else {
    this->currentpath.replaceTail(node, childindex);
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
  \COININTERNAL

  This method not available in the original OIV API, see SoSubAction.h
  for explanation.
 */
SoEnabledElementsList *
SoAction::getClassEnabledElements(void)
{
  return SoAction::enabledElements;
}

/*!
  \COININTERNAL

  This method not available in the original OIV API, see SoSubAction.h
  for explanation.
 */
SoActionMethodList *
SoAction::getClassActionMethods(void)
{
  return SoAction::methods;
}

// This is common doc for SoAction and all SoAction-derived classes,
// so keep it general.
/*!
  This virtual method is called from SoAction::apply(), and is the
  entry point for the actual scenegraph traversal.

  It can be overridden to initialize the action at traversal start,
  for specific initializations in the action subclasses inheriting
  SoAction.

  Default method just calls traverse(), which any overridden
  implementation of the method must do too (or call
  SoAction::beginTraversal()) to trigger the scenegraph traversal.
*/
void
SoAction::beginTraversal(SoNode * node)
{
  this->traverse(node);
}

/*!
  This virtual method can be overridden to execute code after the
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
  THIS->terminated = flag;
}

/*!
  \COININTERNAL
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
  SoActionP::AppliedData storeddata = THIS->applieddata;
  AppliedCode storedcode = THIS->appliedcode;
  PathCode storedpathcode = this->currentpathcode;
  SoTempPath storedpath = this->currentpath;

  // Start path traversal. Don't use beginTraversal() (the user might
  // have overridden it).
  THIS->appliedcode = SoAction::PATH;
  THIS->applieddata.path = path;
  this->currentpathcode = SoAction::IN_PATH;
  this->traverse(path->getNode(0));

  // Restore previous state.
  this->currentpath = storedpath;
  this->currentpathcode = storedpathcode;
  THIS->applieddata = storeddata;
  THIS->appliedcode = storedcode;
}

/*!
  Store our state, traverse the subgraph rooted at the given \a node,
  restore our state and continue traversal.
 */
void
SoAction::switchToNodeTraversal(SoNode * node)
{
  // Store current state.
  SoActionP::AppliedData storeddata = THIS->applieddata;
  AppliedCode storedcode = THIS->appliedcode;
  PathCode storedpathcode = this->currentpathcode;
  SoTempPath storedpath = this->currentpath;

  THIS->appliedcode = SoAction::NODE;
  THIS->applieddata.node = node;
  this->currentpathcode = SoAction::NO_PATH;
  this->currentpath.truncate(0);

  this->traverse(node);

  // Restore previous state.
  this->currentpath = storedpath;
  this->currentpathcode = storedpathcode;
  THIS->applieddata = storeddata;
  THIS->appliedcode = storedcode;
}
