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
  \class SoAmbientColorElement Inventor/elements/SoAmbientColorElement.h
  \brief The SoAmbientColorElement class is yet to be documented.
*/

#include <Inventor/elements/SoAmbientColorElement.h>

#include <Inventor/SbColor.h>

#include <assert.h>

static const SbColor defaultColor(0.2f, 0.2f, 0.2f);

/*!
  \fn SoAmbientColorElement::numColors

  This is the number of ambient colors.
*/

/*!
  \fn SoAmbientColorElement::colors

  This is an array of ambient colors.
*/

SO_ELEMENT_SOURCE(SoAmbientColorElement);

/*!
  This static method initializes static data for the
  SoAmbientColorElement class.
*/

void
SoAmbientColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoAmbientColorElement, inherited);
}

//! FIXME: write doc.

void
SoAmbientColorElement::init(SoState * /* state */)
{
  this->colors = &defaultColor;
  this->numColors = 1;
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAmbientColorElement::createInstance()
*/

SoAmbientColorElement::SoAmbientColorElement()
{
  setTypeId(SoAmbientColorElement::classTypeId);
  setStackIndex(SoAmbientColorElement::classStackIndex);
}

/*!
  The destructor.
*/

SoAmbientColorElement::~SoAmbientColorElement()
{
}

//! FIXME: write doc.

void
SoAmbientColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const SbColor * const colors)
{
  SoAmbientColorElement *elem = (SoAmbientColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (numColors > 0)
    elem->setElt(numColors, colors);
  else
    elem->setElt(1, &defaultColor);
}

//! FIXME: write doc.

void
SoAmbientColorElement::setElt(const int32_t numColors,
                              const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoAmbientColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoAmbientColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

/*!
  Return a pointer to the color array. This method is not part of the OIV API.
*/
const SbColor *
SoAmbientColorElement::getArrayPtr() const
{
  return this->colors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoAmbientColorElement *
SoAmbientColorElement::getInstance(SoState *state)
{
  return (const SoAmbientColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}
