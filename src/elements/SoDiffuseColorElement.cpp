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
  \class SoDiffuseColorElement Inventor/elements/SoDiffuseColorElement.h
  \brief The SoDiffuseColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDiffuseColorElement.h>

#include <Inventor/SbColor.h>

#include <assert.h>

static const SbColor defaultColor(0.8f, 0.8f, 0.8f);

/*!
  \fn SoDiffuseColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoDiffuseColorElement::colors

  FIXME: write doc.
*/

/*!
  \fn SoDiffuseColorElement::packedColors

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoDiffuseColorElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoDiffuseColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoDiffuseColorElement, inherited);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::init(SoState * /* state */)
{
  this->colors = &defaultColor;
  this->packedColors = NULL;
  this->numColors = 1;
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoDiffuseColorElement::createInstance(void)
*/

SoDiffuseColorElement::SoDiffuseColorElement()
{
  setTypeId(SoDiffuseColorElement::classTypeId);
  setStackIndex(SoDiffuseColorElement::classStackIndex);
}

/*!
  The destructor.
*/

SoDiffuseColorElement::~SoDiffuseColorElement()
{
}

//! FIXME: write doc.

void
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const SbColor * const colors)
{
  SoDiffuseColorElement *elem = (SoDiffuseColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (numColors > 0)
    elem->setElt(numColors, colors);
  else
    elem->setElt(1, &defaultColor);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const uint32_t * const colors,
                           const SbBool packedtransparency)
{
  SoDiffuseColorElement *elem = (SoDiffuseColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(numColors, colors, packedtransparency);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::setElt(const int32_t numColors,
                              const SbColor * const colors)
{
  this->packedColors = NULL;
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

void
SoDiffuseColorElement::setElt(const int32_t numColors,
                              const uint32_t * const packedcolors,
                              const SbBool packedtransparency)
{
  this->packedColors = packedcolors;
  this->numColors = numColors;
  this->colors = NULL;
  this->packedTransparency = packedtransparency;
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoDiffuseColorElement::isPacked() const
{
  return this->packedColors != NULL;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoDiffuseColorElement::getNum() const
{
  return this->numColors;
}

/*!
  Returns the color array. This method is not part of the OIV API.
*/
const SbColor *
SoDiffuseColorElement::getColorArrayPtr() const
{
  return this->colors;
}

/*!
  Returns the packed color array. This method is not part of the OIV API.
*/
const uint32_t *
SoDiffuseColorElement::getPackedArrayPtr() const
{
  return this->packedColors;
}

//! FIXME: write doc.

const SbColor &
SoDiffuseColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  assert(this->colors != NULL);
  return this->colors[index];
}

SbBool 
SoDiffuseColorElement::hasPackedTransparency(void) const
{
  return this->packedTransparency;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoDiffuseColorElement *
SoDiffuseColorElement::getInstance(SoState *state)
{
  return (const SoDiffuseColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}
