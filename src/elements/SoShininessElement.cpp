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
  \class SoShininessElement Inventor/elements/SoShininessElement.h
  \brief The SoShininessElement class is yet to be documented.

  FIXME: write doc.

  This element is part of the SoLazyElement in some other OI implementetions.
*/

#include <Inventor/elements/SoShininessElement.h>


#include <assert.h>

static const float defaultValue = 0.2f;

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
  this->values = &defaultValue;
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
  if (numValues > 0)
    elem->setElt(numValues, values);
  else
    elem->setElt(1, &defaultValue);
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
