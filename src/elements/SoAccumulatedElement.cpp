/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoAccumulatedElement Inventor/elements/SoAccumulatedElement.h
  \brief The SoAccumulatedElement class is an abstract class for storing accumulated state.
  \ingroup elements

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


void 
SoAccumulatedElement::init(SoState * state)
{
  inherited::init(state);
  // this is FALSE until node id's are copied
  this->recursecapture = FALSE;
}

void 
SoAccumulatedElement::push(SoState * state)
{
  inherited::push(state);
  // this is FALSE until node id's are copied
  this->recursecapture = FALSE;
}

// Documented in superclass. Overridden to compare node ids.
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

/*!
  Empty the list of node ids.
*/
void
SoAccumulatedElement::clearNodeIds(void)
{
  this->nodeids.truncate(0);
  this->checksum = 0;
  // we do not depend on previous elements any more
  this->recursecapture = FALSE;
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
  // we do not depend on previous elements any more
  this->recursecapture = FALSE;
}

// Documented in superclass. Overridden to copy node ids.
SoElement *
SoAccumulatedElement::copyMatchInfo(void) const
{
  SoAccumulatedElement * element =
    (SoAccumulatedElement *) this->getTypeId().createInstance();
  const int n = this->nodeids.getLength();
  for (int i = 0; i < n; i++) {
    element->nodeids.append(this->nodeids[i]);
  }
  element->checksum = this->checksum;
  element->recursecapture = TRUE;
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
  
  // this elements uses data from previous element in stack
  this->recursecapture = TRUE;
}

// Documented in superclass. Overridden to capture more elements.
void
SoAccumulatedElement::captureThis(SoState * state) const
{
  inherited::captureThis(state);

  // we need to recurse if element has copied data from previous
  // element in stack (or nextInStack as SGI was silly enough to call
  // it). This is because the depth of this element might not cause
  // cache to depend on this element, but the previous element(s)
  // might have a depth that will trigger a dependency.
  //                                              pederb, 2001-02-21
  if (this->recursecapture) {
    SoAccumulatedElement * elem = (SoAccumulatedElement*)
      this->getNextInStack();
    if (elem) elem->captureThis(state);
  }
}
