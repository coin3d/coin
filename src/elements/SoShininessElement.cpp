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
  \class SoShininessElement Inventor/elements/SoShininessElement.h
  \brief The SoShininessElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  This element is part of the SoLazyElement in some other OI implementetions.
*/

#include <Inventor/elements/SoShininessElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoShininessElement);

/*!
  This static method initializes static data for the SoShininessElement class.
*/

void
SoShininessElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoShininessElement, inherited);
}

/*!
  The destructor.
*/

SoShininessElement::~SoShininessElement()
{
}

//! FIXME: write doc.

void
SoShininessElement::init(SoState * state)
{
  this->state = state;
}

//! FIXME: write doc.

void
SoShininessElement::set(SoState * const state, SoNode * const node,
                           const int32_t numvalues,
                           const float * const values)
{
  SoLazyElement::setShininess(state, values[0]);
#if COIN_DEBUG
  if (numvalues > 1) {
    SoDebugError::postWarning("SoShininessElement::set",
                              "Multiple shininess values not supported. "
                              "All values except the first will be ignored.");
  }
#endif // COIN_DEBIG

}

//! FIXME: write doc.
int32_t
SoShininessElement::getNum(void) const
{
  return 1;
}

//! FIXME: write doc.

float
SoShininessElement::get(const int index) const
{
  assert(index == 0);
  return SoLazyElement::getShininess(this->state);
}

/*!
  Returns a pointer to the shininess values. This method is not part of the OIV API.
*/
const float *
SoShininessElement::getArrayPtr(void) const
{
  ((SoShininessElement*)this)->dummyvalue = SoLazyElement::getShininess(this->state);
  return &this->dummyvalue;
}

//! FIXME: write doc.

const SoShininessElement *
SoShininessElement::getInstance(SoState *state)
{
  return (const SoShininessElement *)
    state->getElementNoPush(classStackIndex);
}
