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
#include <Inventor/actions/SoSubActionP.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/SoOutput.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>


SO_ACTION_SOURCE(SoWriteAction);

// Overridden from parent class.
void
SoWriteAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoWriteAction, SoAction);
}

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

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::writeS);

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
