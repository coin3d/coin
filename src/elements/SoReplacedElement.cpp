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
  \class SoReplacedElement Inventor/elements/SoReplacedElement.h
  \brief The SoReplacedElement class is yet to be documented.

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
  The constructor.  Can't be used directly.  This is an abstract base class.
*/

SoReplacedElement::SoReplacedElement(void)
  : nodeId(0)
{
  setTypeId(SoReplacedElement::classTypeId);
  setStackIndex(SoReplacedElement::classStackIndex);
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

  Originally, this function overrides SoElement::getElement() to set the
  nodeId in the instance before returning it, but I don't think that is
  necessary (should happen automatically) in push().
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
