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
  \class SoTransparencyElement Inventor/elements/SoTransparencyElement.h
  \brief The SoTransparencyElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTransparencyElement.h>

#include <assert.h>

static const float defaultValue = 0.0f;

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
  A constructor.  Can't be used directly.

  \sa void * SoTransparencyElement::createInstance(void)
*/

SoTransparencyElement::SoTransparencyElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
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
  this->values = &defaultValue;
  this->numValues = 1;
}

//! FIXME: write doc.

void
SoTransparencyElement::set(SoState * const state, SoNode * const node,
                           const int32_t numValues,
                           const float * const values)
{
  SoTransparencyElement *elem = (SoTransparencyElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (numValues)
    elem->setElt(numValues, values);
  else
    elem->setElt(1, &defaultValue);
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
  assert(index >= 0 && index <= this->numValues);
  return this->values[index];
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
  Sets the value of this element. Can be overloaded by subclasses.
*/
void
SoTransparencyElement::setElt(const int numValues, const float * const values)
{
  this->numValues = numValues;
  this->values = values;
}
