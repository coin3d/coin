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
  \class SoWriteAction Inventor/actions/SoWriteAction.h
  \brief The SoWriteAction class dumps the scene graph to a file.

  FIXME: more doc

  \sa SoOutput
*/

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSubAction.h>

#include <Inventor/SbName.h>
#include <Inventor/SoOutput.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoWriteAction )

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
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoWriteAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoWriteAction::enableElement(const SoType type, const int stackIndex)
{
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
//$ BEGIN TEMPLATE InitActionSource( SoWriteAction )
  assert(SoWriteAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoWriteAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoWriteAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  methods->setDefault((void *)SoNode::writeS);
}

/*!
  This static method cleans up static data for the SoWriteAction class.
*/
void
SoWriteAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  Default constructor. Output will be written to stdout in ASCII format.
*/
SoWriteAction::SoWriteAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoWriteAction);

  this->outobj = new SoOutput;
  this->localoutputalloc = TRUE;
}

/*!
  Constructor. Output will be written via the \a out object.
*/
SoWriteAction::SoWriteAction(SoOutput * out)
{
  SO_ACTION_CONSTRUCTOR(SoWriteAction);

  this->outobj = out;
  this->localoutputalloc = FALSE;
}

/*!
  Destructor.
*/
SoWriteAction::~SoWriteAction(void)
{
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
SoWriteAction::writePROTO(SoVRMLPROTODef * protoDef)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::writeEXTERNPROTO(SoVRMLEXTERNPROTODef * externprotoDef)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::writeAllPROTO(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::continueToApply(SoNode * node)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoWriteAction::continueToApply(SoPath * path)
{
  assert(0 && "FIXME: not implemented yet");
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
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}
