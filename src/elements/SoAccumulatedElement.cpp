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
  \class SoAccumulatedElement Inventor/elements/SoAccumulatedElement.h
  \brief The SoAccumulatedElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAccumulatedElement.h>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \fn SoAccumulatedElement::nodeIds

  This is a SoMFNodeField.
*/

SO_ELEMENT_ABSTRACT_SOURCE(SoAccumulatedElement);

/*!
  This static method initializes static data for the SoAccumulatedElement
  class.
*/

void
SoAccumulatedElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoAccumulatedElement, inherited);
}

/*!
  A constructor.  Can't be used directly.  This is an abstract base class.
*/

SoAccumulatedElement::SoAccumulatedElement()
{
  setTypeId(SoAccumulatedElement::classTypeId);
  setStackIndex(SoAccumulatedElement::classStackIndex);

  // FIXME: there's no init method which cleans out the nodeIds
  // SbPList, won't this result in a memory leak? Or is this taken
  // care of through the use of setNodeId()? 19990418 mortene.
}

/*!
  The destructor.
*/

SoAccumulatedElement::~SoAccumulatedElement(void)
{
}

//! FIXME: write doc.

SbBool
SoAccumulatedElement::matches(const SoElement * /* element */) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

void
SoAccumulatedElement::print(FILE * file) const
{
  fprintf(file, "SoAccumulatedElement(%p)\n", this);
}

//! FIXME: write doc.

void
SoAccumulatedElement::clearNodeIds(void)
{
  this->nodeIds.truncate(0);
}

//! FIXME: write doc.

void
SoAccumulatedElement::addNodeId(const SoNode * const node)
{
  this->nodeIds.append((void *)node->getNodeId());
}

//! FIXME: write doc.

void
SoAccumulatedElement::setNodeId(const SoNode * const node)
{
  this->clearNodeIds();
  this->nodeIds.append((void *)node->getNodeId());
}

//! FIXME: write doc.

SoElement *
SoAccumulatedElement::copyMatchInfo() const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

//! FIXME: write doc.

void
SoAccumulatedElement::captureThis(SoState * /* state */) const
{
  assert(0 && "FIXME: not implemented");
}
