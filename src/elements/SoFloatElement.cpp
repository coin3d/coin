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
  \class SoFloatElement Inventor/elements/SoFloatElement.h
  \brief SoFloatElement is an abstract base class for elements that consists of a single float value.
  \ingroup elements

  This is the superclass of elements where the new element data \e
  replaces the old data, and where the data the element stores is a
  simple single precision floating point value.

  This element is like a convenient light-weight version of the
  SoReplacedElement. It differs from the SoReplacedElement in that the
  set() and get() methods are already implemented, since it is known
  that subclasses will still contain just a single float value.

  \sa SoReplacedElement, SoInt32Element, SoAccumulatedElement
*/

#include "coindefs.h"
#include "SbBasicP.h"

#include <Inventor/elements/SoFloatElement.h>
#include <cassert>

SO_ELEMENT_ABSTRACT_SOURCE(SoFloatElement);

/*!
  \var float SoFloatElement::data
  The element's value.
*/

// doc in super
void
SoFloatElement::initClass(void)
{
  SO_ELEMENT_INIT_ABSTRACT_CLASS(SoFloatElement, inherited);
}

/*!
  Destructor.
*/
SoFloatElement::~SoFloatElement(void)
{
}

// doc in super
SbBool
SoFloatElement::matches(const SoElement * element) const
{
  assert(element);
  if (getTypeId() != element->getTypeId()) { return FALSE; }
  if (this->data != (coin_assert_cast<const SoFloatElement *>(element)->data)) {
    return FALSE;
  }
  return TRUE;
}

// doc in super
SoElement *
SoFloatElement::copyMatchInfo(void) const
{
  // SoElement::copyMatchInfo is abstract
  //    inherited::copyMatchInfo();
  assert(getTypeId().canCreateInstance());
  SoFloatElement * element = static_cast<SoFloatElement *>(getTypeId().createInstance());
  element->data = this->data;
  // DEPRECATED 980807 pederb. copyMatchInfo() should only copy
  // information needed in matches(). An exact copy is not needed.
  //    element->dataNode = this->dataNode;
  return element;
}

// doc in super
void
SoFloatElement::print(FILE * file) const
{
  (void)fprintf(file, "%s[%p]: data = %f\n",
                this->getTypeId().getName().getString(), this, this->data);
}

/*!
  Static method for setting the \a value of an element in the given \a
  state at the given stack \a index.
 */
void
SoFloatElement::set(const int index,
                    SoState * const state,
                    SoNode * const COIN_UNUSED_ARG(node),
                    const float value)
{
  SoFloatElement * element =
    coin_safe_cast<SoFloatElement *>
    (
     SoFloatElement::getElement(state, index)
     );
  if (element) {
    element->setElt(value);
  }
}

/*!
  Static method for setting the \a value of an element in the given \a
  state at the given \a stackIndex.
 */
void
SoFloatElement::set(const int stackIndex, SoState * const state,
                    const float value)
{
  SoFloatElement::set(stackIndex, state, NULL, value);
}

/*!
  Static method to fetch the value of the element of this type from
  the given \a state at the given stack \a index.
 */
float
SoFloatElement::get(const int index, SoState * const state)
{
  const SoFloatElement * element = coin_safe_cast<const SoFloatElement *>
    (
     getConstElement(state, index)
     ); //, NULL );
  if (element) { return element->data; }
  return 0.0f;
}

/*!
  Set element value.
 */
void
SoFloatElement::setElt(float value)
{
  this->data = value;
}

// doc in super
void
SoFloatElement::init(SoState * state)
{
  inherited::init(state);
  this->data = 0.0f;
}
