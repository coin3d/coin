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
  \class SoAccumulatedElement Inventor/elements/SoAccumulatedElement.h
  \brief The SoAccumulatedElement class is an abstract class for storing accumulated state.
  \ingroup elements

  The element store node id values for all nodes accumulated during
  traversal for the current state. These id values are used to
  determine when to invalidate caches.
*/

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/nodes/SoNode.h>
#include <assert.h>

/*!
  \fn SoAccumulatedElement::nodeids
  Used to store node ids.
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
  const SoAccumulatedElement * elem = (const SoAccumulatedElement *)element;
  return (elem->nodeids == this->nodeids);
}

/*!
  Empty the list of node ids.
*/
void
SoAccumulatedElement::clearNodeIds(void)
{
  this->nodeids.truncate(0);
  // we do not depend on previous elements any more
  this->recursecapture = FALSE;
}

/*!
  Add the node id of \a node to the list of node ids.
*/
void
SoAccumulatedElement::addNodeId(const SoNode * const node)
{
  this->nodeids.append(node->getNodeId());
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
  this->nodeids = copyfrom->nodeids;
  
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
