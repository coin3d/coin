/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoFloatElement Inventor/elements/SoFloatElement.h
  \brief SoFloatElement is an abstract base class for elements that consists of a single float value.
  \ingroup elements
*/

#include <Inventor/elements/SoFloatElement.h>
#include <assert.h>

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
  if (this->data != ((SoFloatElement *)element)->data) { return FALSE; }
  return TRUE;
}

// doc in super
SoElement *
SoFloatElement::copyMatchInfo(void) const
{
  // SoElement::copyMatchInfo is abstract
  //    inherited::copyMatchInfo();
  assert(getTypeId().canCreateInstance());
  SoFloatElement * element = (SoFloatElement *) getTypeId().createInstance();
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
                    SoNode * const node, // unused
                    const float value)
{
  SoFloatElement * element = (SoFloatElement *)
    (SoFloatElement::getElement(state, index));
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
  SoFloatElement * element = (SoFloatElement *)
    (getConstElement(state, index)); //, NULL );
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
