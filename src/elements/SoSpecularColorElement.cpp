/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoSpecularColorElement Inventor/elements/SoSpecularColorElement.h
  \brief The SoSpecularColorElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoSpecularColorElement.h>

#include <Inventor/SbColor.h>
#include <Inventor/SbName.h>

#include <assert.h>

static const SbColor defaultColor(0.0f, 0.0f, 0.0f);

/*!
  \fn SoSpecularColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoSpecularColorElement::colors

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoSpecularColorElement);

/*!
  This static method initializes static data for the SoSpecularColorElement
  class.
*/

void
SoSpecularColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoSpecularColorElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoSpecularColorElement::createInstance(void)
*/

SoSpecularColorElement::SoSpecularColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoSpecularColorElement::~SoSpecularColorElement()
{
}

//! FIXME: write doc.

void
SoSpecularColorElement::init(SoState * /* state */)
{
  this->colors = &defaultColor;
  this->numColors = 1;
}

//! FIXME: write doc.

void
SoSpecularColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const SbColor * const colors)
{
  SoSpecularColorElement *elem = (SoSpecularColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (numColors > 0)
    elem->setElt(numColors, colors);
  else 
    elem->setElt(1, &defaultColor);
}

//! FIXME: write doc.

void
SoSpecularColorElement::setElt(const int32_t numColors,
                              const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoSpecularColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoSpecularColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

/*!
  Returns a pointer to the color array. This method is not part of the OIV API.
*/
const SbColor *
SoSpecularColorElement::getArrayPtr() const
{
  return this->colors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoSpecularColorElement *
SoSpecularColorElement::getInstance(SoState *state)
{
  return (const SoSpecularColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}
