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
  \class SoWriteAction Inventor/actions/SoWriteAction.h
  \brief The SoWriteAction class writes the scene graph to a file.

  FIXME: more doc

  \sa SoOutput
*/

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()

#include <Inventor/SbName.h>
#include <Inventor/SoOutput.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoWriteAction)

SoType SoWriteAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoWriteAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoWriteAction::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>

// static variables
SoEnabledElementsList * SoWriteAction::enabledElements;
SoActionMethodList * SoWriteAction::methods;

/*!
  \fn SoWriteAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoWriteAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoWriteAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoWriteAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoWriteAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes static data for the
  SoWriteAction class.
*/
void
SoWriteAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoWriteAction)
  assert(SoWriteAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoWriteAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoWriteAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

// *************************************************************************

/*!
  Default constructor. Output will be written to stdout in ASCII format.
*/
SoWriteAction::SoWriteAction(void)
{
  this->commonConstructor(new SoOutput);
  this->localoutputalloc = TRUE;
}

/*!
  Constructor. Output will be written via the \a out object.
*/
SoWriteAction::SoWriteAction(SoOutput * out)
{
  this->commonConstructor(out);
  this->localoutputalloc = FALSE;
}

void
SoWriteAction::commonConstructor(SoOutput * out)
{
  SO_ACTION_CONSTRUCTOR(SoWriteAction);

  static SbBool first = TRUE;
  if (first) {
    first = FALSE;
    SO_ACTION_ADD_METHOD(SoNode, SoNode::writeS);
  }
  methods->setUp(); // FIXME: not sure if this should be called here...

  this->outobj = out;
}

/*!
  Destructor.
*/
SoWriteAction::~SoWriteAction(void)
{
#if 0 // debug
  SoDebugError::postInfo("SoWriteAction::~SoWriteAction", "%p", this);
#endif // debug
  if (this->localoutputalloc) delete this->outobj;
}

/*!
  Returns the SoOutput object we're using when writing the scene graph.
 */
SoOutput *
SoWriteAction::getOutput(void) const
{
  return this->outobj;
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::writePROTO(SoVRMLPROTODef * /* protoDef */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::writeEXTERNPROTO(SoVRMLEXTERNPROTODef * /* externprotoDef */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::writeAllPROTO(void)
{
  COIN_STUB();
}

/*!
  Applies the write method at \a node (and its children) with the
  current SoOutput instance.
*/
void
SoWriteAction::continueToApply(SoNode * node)
{
  this->traverse(node);
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::continueToApply(SoPath * /* path */)
{
  COIN_STUB();
}

/*!
  Starts traversal at \a node. The write action is actually done in two
  passes, one to count the references of the objects in the scene graph,
  and one to do the actual writing.
*/
void
SoWriteAction::beginTraversal(SoNode * node)
{
  this->outobj->setStage(SoOutput::COUNT_REFS);
  this->traverse(node);
  this->outobj->setStage(SoOutput::WRITE);
  this->traverse(node);
  if (!this->outobj->isBinary()) outobj->write('\n');
}

/*!
  FIXME: write function documentation
*/
SbBool
SoWriteAction::shouldCompactPathLists(void) const
{
  COIN_STUB();
  return FALSE;
}
