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
  \class SoWriteAction SoWriteAction.h Inventor/actions/SoWriteAction.h
  \brief The SoWriteAction class writes a scene graph to file.

  When applied to a scene, this action writes its contents to the
  stream contained within an SoOutput instance. This can be a file, a
  memory buffer or a system filehandle like \c stdout, for instance.

  \e All information considered part of the scene graph should be
  written out, including not only nodes, but also the nodes' field
  values, global fields (at least those with connections inside the
  scene the action is applied to), engines in the scene, paths, etc.

  The scene is written in the Open Inventor file format. Files in this
  format can be parsed into their scene graph structures by using the
  SoDB::readAll() method (SoDB also contains a few other import
  methods you can use).

  \sa SoOutput
*/

// Metadon doc
/*¡
  FIXME: the implementation for continueApply() on paths is
  missing. 20000306 mortene.
 */

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSubActionP.h>

#include <Inventor/SoOutput.h>
#include <Inventor/nodes/SoNode.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoWriteAction);


// Overridden from parent class.
void
SoWriteAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoWriteAction, SoAction);
}

/*!
  Default constructor. Output will be written to \c stdout in ASCII
  format.
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

  this->outobj = out;
}

/*!
  Destructor.
*/
SoWriteAction::~SoWriteAction(void)
{
  if (this->localoutputalloc) delete this->outobj;
}

/*!
  Returns a pointer to the SoOutput object we're using when writing
  the scene graph.
 */
SoOutput *
SoWriteAction::getOutput(void) const
{
  return this->outobj;
}

/*!
  Applies the write method to the subgraph starting at \a node with
  the current SoOutput instance, without resetting any of the internal
  state of the action instance.

  This should normally be for internal use only.
*/
void
SoWriteAction::continueToApply(SoNode * node)
{
  this->traverse(node);
}

/*!
  Applies the write method to \a path with the current SoOutput
  instance, without resetting any of the internal state of the action
  instance.

  This should normally be for internal use only.
*/
void
SoWriteAction::continueToApply(SoPath * path)
{
  COIN_STUB(); // FIXME
}

/*!
  Overloaded from parent class, as the write action is actually done
  in two passes.

  The first pass is done to count the references of the objects in the
  scene graph and otherwise prepare instance in the scene for export.
  The second pass does the actual writing.
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
  \internal

  Compact path lists are not implemented in Coin (yet), but if they
  are, SoWriteAction should return \c FALSE here -- it would only be
  extra overhead for the SoWriteAction to have pathlists compacted
  before traversal.

  Seems like a silly optimization to me, though.. :^/  20000306 mortene.
*/
SbBool
SoWriteAction::shouldCompactPathLists(void) const
{
  return FALSE;
}
