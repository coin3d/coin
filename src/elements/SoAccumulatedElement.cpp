/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoAccumulatedElement Inventor/elements/SoAccumulatedElement.h
  \brief The SoAccumulatedElement class is an abstract class for storing accumulated state.

  It stores node ids for all nodes accumulating the current
  state. These ids are used to quickly determine when to invalidate
  caches.
*/

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

/*!
  \fn SoAccumulatedElement::nodeids
  Used to store node ids.
*/

/*!
  \fn SoAccumulatedElement::checksum
  Used to optimize the matches() method.
*/

SO_ELEMENT_ABSTRACT_SOURCE(SoAccumulatedElement);

// doc from parent
void
SoAccumulatedElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoAccumulatedElement, inherited);
}

/*!
  The destructor.
*/

SoAccumulatedElement::~SoAccumulatedElement(void)
{
}

/*!
  Overloaded to compare node ids.
*/
SbBool
SoAccumulatedElement::matches(const SoElement * element) const
{
  const SoAccumulatedElement * elem =
    (SoAccumulatedElement*) element;
  if (elem->checksum != this->checksum) return FALSE;
  const int n = this->nodeids.getLength();
  if (n != elem->nodeids.getLength()) return FALSE;
  const uint32_t * p0 = this->nodeids.getArrayPtr();
  const uint32_t * p1 = elem->nodeids.getArrayPtr();
  for (int i = 0; i < n; i++) {
    if (p0[i] != p1[i]) return FALSE;
  }
  return TRUE;
}

// doc from parent
void
SoAccumulatedElement::print(FILE * file) const
{
  fprintf(file, "SoAccumulatedElement(%p)\n", this);
}

/*!
  Empty the list of node ids.
*/
void
SoAccumulatedElement::clearNodeIds(void)
{
  this->nodeids.truncate(0);
  this->checksum = 0;
}

/*!
  Add the node id of \a node to the list of node ids.
*/
void
SoAccumulatedElement::addNodeId(const SoNode * const node)
{
  uint32_t id = node->getNodeId();
  this->checksum += id;
  this->nodeids.append(id);
}

/*!
  Empty the list of node ids, and add the id of \a node.
*/
void
SoAccumulatedElement::setNodeId(const SoNode * const node)
{
  this->clearNodeIds();
  this->addNodeId(node);
}

/*!
  Overloaded to copy node ids.
*/
SoElement *
SoAccumulatedElement::copyMatchInfo() const
{
  SoAccumulatedElement * element =
    (SoAccumulatedElement *) this->getTypeId().createInstance();
  const int n = this->nodeids.getLength();
  for (int i = 0; i < n; i++) {
    element->nodeids.append(this->nodeids[i]);
  }
  element->checksum = this->checksum;
  return element;
}

/*!
  Convenience method which copies the node ids from \a copyfrom to
  this element. This method is not part of the OIV API.
*/
void
SoAccumulatedElement::copyNodeIds(const SoAccumulatedElement * copyfrom)
{
  this->checksum = 0;
  this->nodeids.truncate(0);
  const int n = copyfrom->nodeids.getLength();
  for (int i = 0; i < n; i++) {
    uint32_t id = copyfrom->nodeids[i];
    this->checksum += id;
    this->nodeids.append(id);
  }
}

/*!
  Overloaded to capture more elements.
*/
void
SoAccumulatedElement::captureThis(SoState * state) const
{
  // FIXME: not quite sure what is needed here. pederb, 20000608
  inherited::captureThis(state);
}
