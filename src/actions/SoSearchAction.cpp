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
  \class SoSearchAction Inventor/actions/SoSearchAction.h
  \brief The SoSearchAction class provides methods for searching for nodes
  matching certain criterions in Open Inventor scene graphs.

  Nodes can be searched for by pointer, type, and name, or a combination
  of those.  Types can be interpreted as exact types, or the type can match
  nodes derived from it.  Every single node can be searched, or normal
  traversal rules can be followed (especially related to switches) when
  searching.
*/

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoSubAction.h>

//#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoNode.h>

#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  \fn SoSearchAction::LookFor

  FIXME: write doc.
*/

/*!
  \fn SoSearchAction::Interest

  FIXME: write doc.
*/

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoSearchAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoSearchAction )

SoType SoSearchAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoSearchAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoSearchAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoSearchAction::enabledElements;
SoActionMethodList * SoSearchAction::methods;

/*!
  \fn SoSearchAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoSearchAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoSearchAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoSearchAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoSearchAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method is for initializing the static data for the
  SoSearchAction class.
*/

void
SoSearchAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoSearchAction )
  assert(SoSearchAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSearchAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoSearchAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method is for cleaning up static data for the
  SoSearchAction class.
*/

void
SoSearchAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  The constructor.  Initializes all the internals with default values.
*/

SoSearchAction::SoSearchAction(void)
  : lookFor(0), interest( FIRST ), searchAll( FALSE ),
    node(NULL), type( SoType::badType() ), name( "" ),
    path(NULL) // paths(0)
{
  SO_ACTION_CONSTRUCTOR(SoSearchAction);

  static int first = 1;
  if (first) {
    first = 0;
    // with so few methods - this could be removed from the SoNode interface
    // and it's virtual table...
    SO_ACTION_ADD_METHOD(SoNode, SoNode::searchS);
  }
  methods->setUp(); // FIXME: not sure if this should be called here...
}

/*!
  The destructor.
*/

SoSearchAction::~SoSearchAction(void)
{
  reset(); // clears everything
}

// *************************************************************************

/*!
  This method will set the node pointer to search for.  It automatically
  configures the action to search for a node pointer, so there is no need
  to call SoSearchAction::setFind() in anything but special cases.
*/

void
SoSearchAction::setNode(SoNode * const node)
{
#if 0 // debug
  SoDebugError::postInfo("SoSearchAction::setNode",
			 "search for node: %p (type '%s')",
			 node, node->getTypeId().getName().getString());
#endif // debug

  node->ref();
  if (this->node)
    this->node->unref();
  this->node = node;
  this->lookFor |= NODE;
}

/*!
  This method returns the node the SoSearchAction instance is configured
  to search for.
*/

SoNode *
SoSearchAction::getNode(void) const
{
  return this->node;
}

/*!
  This method configures the SoSearchAction instance to search for nodes of
  the given type, and nodes derived from the given type if includeDerived is
  set to TRUE.
*/

void
SoSearchAction::setType(const SoType type, const SbBool includeDerived)
{
  this->type = type;
  this->includeDerived = includeDerived;
  this->lookFor |= TYPE;
}

/*!
  This method returns the node type which is searched for, and wether derived
  classes of that type also returns a match.
*/

SoType
SoSearchAction::getType(SbBool & includeDerived) const
{
  includeDerived = this->includeDerived;
  return this->type;
}

/*!
  This method configures the SoSearchAction instance to search for nodes with
  the given name.  There is no need to call SoSearchAction::setFind() since
  the configuration is updated automatically.
*/

void
SoSearchAction::setName(const SbName name) // was const SbName &, but SbName is 32 bits, so
{
  this->name = name;
  this->lookFor |= NAME;
}

/*!
  This method returns the name the SoSearchAction instance is configured to
  search for.
*/

SbName // was const SbName &
SoSearchAction::getName(void) const
{
  return this->name;
}

/*!
  This method configures what to search for in the scene graph.  what is
  a bitmask of LookFor flags.  Default find configuration is to search for
  nothing, but the configuration updated automatically when
  SoSearchAction::setNode(), SoSearchAction::setType(), and
  SoSearchAction::setName() are called.
*/

void
SoSearchAction::setFind(const int what) // was int
{
  // any sanity checking?
  this->lookFor = what;
}

/*!
  This method returns the search configuration of the SoSearchAction instance.
*/

int
SoSearchAction::getFind(void) const
{
  return this->lookFor;
}

/*!
  This method configures wether only the first, the last, or all the searching
  matches are of interest.  Default configuration is FIRST.
*/

void
SoSearchAction::setInterest(const Interest interest)
{
  // any sanity checking?
  this->interest = interest;
}

/*!
  This method returns wether only the first, the last, or all the searching
  matches will be saved.  Default configuration is FIRST.
*/

SoSearchAction::Interest
SoSearchAction::getInterest(void) const
{
  return this->interest;
}

/*!
  This method specifies wether normal graph traversal should be done
  (searchAll = FALSE) (default), or wether every single node should be
  searched (searchAll = TRUE).
*/

void
SoSearchAction::setSearchingAll(const SbBool searchAll)
{
  this->searchAll = searchAll;
}

/*!
  This method returns the traversal method configuration of the action.
  Default is not to traverse every single node, but to honor switches
  among others...
*/

SbBool
SoSearchAction::isSearchingAll(void) const
{
  return this->searchAll;
}

/*!
  This method returns the path to the node of interest that matched the
  search criterions.  If ALL matches are of interest, NULL is returned, and
  the result must be fetched through SoSearchAction::getPaths().
  If no match was found, NULL is returned.
*/

SoPath *
SoSearchAction::getPath(void) const
{
  return this->path;
}

/*!
  This method returns a path list for all nodes that matched the search
  criterions.  If interest were only FIRST or LAST, this method returns
  an empty path list, and SoSearchAction::getPath() should be used..
*/

SoPathList &
SoSearchAction::getPaths(void)
{
  return this->paths;
}

/*!
  This method sets all the SoSearchAction internals back to their default
  values.
*/

void
SoSearchAction::reset(void)
{
  this->lookFor = 0;
  this->interest = FIRST;
  this->searchAll = FALSE;
  this->includeDerived = TRUE;
  if (this->node) {
    this->node->unref();
    this->node = NULL;
  }
  this->type = SoType::badType();
  this->name = SbName("");
  if (this->path) {
    this->path->unref();
    this->path = NULL;
  }
  this->paths.truncate(0);
}

// *************************************************************************
// extender methods

/*!
  This extender method marks the SoSearchAction instance as having found all
  nodes of interest.
*/

void
SoSearchAction::setFound(void)
{
  setTerminated(TRUE);
}

/*!
  This extender method returns wether the SoSearchAction instance has found
  all nodes of interest.
*/

SbBool
SoSearchAction::isFound(void) const
{
  return hasTerminated();
}

/*!
  This extender method sets the path, or adds the path to the path list,
  depending on the interest configuration.  The path is not copied, so it
  can not be modified after being added without side effects.
*/

void
SoSearchAction::addPath(SoPath * const path)
{
  assert(! this->isFound()); // shouldn't try to add path if found

  switch (this->interest) {
  case FIRST:
    assert(! this->path); // should be NULL
    this->path = path;
    this->path->ref();
    setFound();
    break;

  case LAST:
    if (this->path)
      this->path->unref(); // should delete it if possible
    this->path = path;
    this->path->ref();
    break;

  case ALL:
    this->paths.append(path);
    break;

  default:
    SoDebugError::post("SoSearchAction::addPath()",
      "this->interest is invalid (%d)", this->interest);
    break;
  }
}

// *************************************************************************

/*!
  This method initiates the SoSearchAction instance on the scene graph.
*/

void
SoSearchAction::beginTraversal(SoNode * node)
{
  this->paths.truncate(0);
  if (this->path) {
    this->path->unref();
    this->path = NULL;
  }

  // begin traversal at root node
  this->traverse(node);
}

