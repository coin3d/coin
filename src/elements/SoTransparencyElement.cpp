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
  \class SoTransparencyElement Inventor/elements/SoTransparencyElement.h
  \brief The SoTransparencyElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoTransparencyElement.h>

#include <assert.h>

static const float defaulttransparencyval = 0.0f;

/*!
  \fn SoTransparencyElement::numValues

  FIXME: write doc.
*/

/*!
  \fn SoTransparencyElement::values

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoTransparencyElement);

/*!
  This static method initializes static data for the
  SoTransparencyElement class.
*/

void
SoTransparencyElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoTransparencyElement, inherited);
}

/*!
  The destructor.
*/

SoTransparencyElement::~SoTransparencyElement()
{
}

//! FIXME: write doc.

void
SoTransparencyElement::init(SoState * /* state */)
{
  this->values = &defaulttransparencyval;
  this->numValues = 1;
}

//! FIXME: write doc.

void
SoTransparencyElement::set(SoState * const state, SoNode * const node,
                           const int32_t numValues,
                           const float * const values)
{
  SoTransparencyElement * elem = (SoTransparencyElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (elem) {
    if (numValues)
      elem->setElt(numValues, values);
    else
      elem->setElt(1, &defaulttransparencyval);
  }
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoTransparencyElement::getNum() const
{
  return this->numValues;
}

//! FIXME: write doc.

float
SoTransparencyElement::get(const int index) const
{
  return this->values[SbClamp(index, 0, this->numValues-1)];
}

/*!
  Returns a pointer to the transparency values. This method is not part of the OIV API.
*/
const float *
SoTransparencyElement::getArrayPtr() const
{
  return this->values;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoTransparencyElement *
SoTransparencyElement::getInstance(SoState *state)
{
  return (const SoTransparencyElement *)
    SoElement::getConstElement(state, classStackIndex);
}

/*!
  Sets the value of this element. Can be overridden by subclasses.
*/
void
SoTransparencyElement::setElt(const int numValues, const float * const values)
{
  this->numValues = numValues;
  this->values = values;
}
