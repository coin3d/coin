/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoReplacedElement Inventor/elements/SoReplacedElement.h
  \brief The SoReplacedElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/nodes/SoNode.h>

#include <assert.h>

/*!
  \fn SoReplacedElement::nodeId

  FIXME: write doc.
*/


SO_ELEMENT_ABSTRACT_SOURCE(SoReplacedElement);


/*!
  This static method initializes static data for the SoReplacedElement class.
*/

void
SoReplacedElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoReplacedElement, inherited);
}

/*!
  The destructor.
*/

SoReplacedElement::~SoReplacedElement(void)
{
}

//! FIXME: write doc.

void
SoReplacedElement::init(SoState * state)
{
  inherited::init(state);
  this->nodeId = 0;
}

//! FIXME: write doc.

SbBool
SoReplacedElement::matches(const SoElement * element) const
{
  if (((const SoReplacedElement *)(element))->nodeId ==
      this->nodeId)
    return TRUE;
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoReplacedElement::copyMatchInfo(void) const
{
  assert(getTypeId().canCreateInstance());
  SoReplacedElement * element =
    (SoReplacedElement *)(getTypeId().createInstance());
  element->nodeId = this->nodeId;
  return element;
}

//! FIXME: write doc.

void
SoReplacedElement::print(FILE * file) const
{
  fprintf(file, "%s[%p]\n", getTypeId().getName().getString(),
          this);
}

/*!
  This function returns the top element on the stack.

  This function overrides SoElement::getElement() to set the nodeId in
  the instance before returning it.  SoReplacedElement subclasses should
  use this method to get writable instances.
*/

SoElement *
SoReplacedElement::getElement(SoState * const state,
                              const int stackIndex,
                              SoNode * const node)
{
  SoReplacedElement *elem =
    (SoReplacedElement*) SoElement::getElement(state, stackIndex);
  if (node)
    elem->nodeId = node->getNodeId();
  else elem->nodeId = 0;
  return elem;
}

/*!
  \fn uint32_t SoReplacedElement::getNodeId(void) const

  This function returns the node identifier for the node that previously
  updated the SoReplacedElement.
*/

//$ EXPORT INLINE
uint32_t
SoReplacedElement::getNodeId() const
{
  return this->nodeId;
}
