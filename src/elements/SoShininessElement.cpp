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
  \class SoShininessElement Inventor/elements/SoShininessElement.h
  \brief The SoShininessElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  This element is part of the SoLazyElement in some other OI implementetions.
*/

#include <Inventor/elements/SoShininessElement.h>


#include <assert.h>

static const float defaultshininessval = 0.2f;

/*!
  \fn SoShininessElement::numValues

  FIXME: write doc.
*/

/*!
  \fn SoShininessElement::values

  FIXME: write doc.
*/

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
SoShininessElement::init(SoState * /* state */)
{
  this->values = &defaultshininessval;
  this->numValues = 1;
}

//! FIXME: write doc.

void
SoShininessElement::set(SoState * const state, SoNode * const node,
                           const int32_t numValues,
                           const float * const values)
{
  SoShininessElement *elem = (SoShininessElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (elem) {
    if (numValues > 0)
      elem->setElt(numValues, values);
    else
      elem->setElt(1, &defaultshininessval);
  }
}

//! FIXME: write doc.

void
SoShininessElement::setElt(const int32_t numValues,
                           const float * const values)
{
  this->values = values;
  this->numValues = numValues;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoShininessElement::getNum() const
{
  return this->numValues;
}

//! FIXME: write doc.

float
SoShininessElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numValues);
  return this->values[index];
}

/*!
  Returns a pointer to the shininess values. This method is not part of the OIV API.
*/
const float *
SoShininessElement::getArrayPtr() const
{
  return this->values;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoShininessElement *
SoShininessElement::getInstance(SoState *state)
{
  return (const SoShininessElement *)
    SoElement::getConstElement(state, classStackIndex);
}
