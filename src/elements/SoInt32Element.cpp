/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoInt32Element SoInt32Element.h Inventor/elements/SoInt32Element.h
  \brief The SoInt32Element class is the base class for elements that simply store a 32-bit integer.
  \ingroup elements

  This is the superclass of elements where the new element data \e
  replaces the old data, and where the data the element stores is a
  simple 32-bit integer value.

  This element is like a convenient light-weight version of the
  SoReplacedElement. It differs from the SoReplacedElement in that the
  set() and get() methods are already implemented, since it is known
  that subclasses will still contain just a single 32-bit integer
  value.

  \sa SoReplacedElement, SoFloatElement, SoAccumulatedElement
*/

#include "SbBasicP.h"

#include <Inventor/elements/SoInt32Element.h>


#include <cassert>

/*!
  \fn SoInt32Element::data

  The 32-bit integer value of the element.
*/

SO_ELEMENT_ABSTRACT_SOURCE(SoInt32Element);

// documented in superclass
void
SoInt32Element::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoInt32Element, inherited);
}

SoInt32Element::~SoInt32Element(void)
{
}

// documented in superclass
SbBool
SoInt32Element::matches(const SoElement * element) const
{
    assert(element);
    if (getTypeId() != element->getTypeId())
        return FALSE;
    if (coin_assert_cast<const SoInt32Element *>(element)->data != this->data)
        return FALSE;
    return TRUE;
}

// documented in superclass
SoElement *
SoInt32Element::copyMatchInfo(void) const
{
    assert(getTypeId().canCreateInstance());
    SoInt32Element * element =
        static_cast<SoInt32Element *>(getTypeId().createInstance());
    element->data = this->data;

    // DEPRECATED 19980807 pederb. copyMatchInfo should only copy
    // information needed in matches(). An exact copy is not needed.
    //
    //    element->dataNode = this->dataNode;
    return element;
}

// documented in superclass
void
SoInt32Element::print(FILE * file) const
{
  (void)fprintf(file, "%s[%p]: data = %d\n",
                getTypeId().getName().getString(), this, this->data);
}

// documented in superclass
void
SoInt32Element::set(const int index,
                    SoState * const state,
                    SoNode * const /* node */,
                    const int32_t value)
{
  SoInt32Element * element;
  element = coin_safe_cast<SoInt32Element *>(getElement(state, index));
  if (element)
    element->setElt(value);
}

/*!
  FIXME: write doc.
*/
void
SoInt32Element::set(const int index, SoState * const state,
                    const int32_t value)
{
  set(index, state, NULL, value);
}

//! FIXME: write doc.

int32_t
SoInt32Element::get(const int index,
                    SoState * const state)
{
  const SoInt32Element * element;
  element = coin_safe_cast<const SoInt32Element *>(getConstElement(state, index)); //, NULL );
  if (element)
    return element->data;
  return 0;
}

//! FIXME: write doc.

void
SoInt32Element::setElt(int32_t value)
{
  this->data = value;
}

//! FIXME: write doc.

void
SoInt32Element::init(SoState * state)
{
  inherited::init(state);
  this->data = 0;
}
