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
  \class SoDiffuseColorElement Inventor/elements/SoDiffuseColorElement.h
  \brief The SoDiffuseColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/SbColor.h>
#include <Inventor/errors/SoDebugError.h>
#include <../tidbits.h> // coin_atexit()
#include <assert.h>
#include <stdlib.h>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbColor * defaultdiffusecolor = NULL;

static void 
SoDiffuseColorElement_cleanup_func(void)
{
  delete defaultdiffusecolor;
}

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
SoDiffuseColorElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDiffuseColorElement, inherited);
  defaultdiffusecolor = new SbColor;
  defaultdiffusecolor->setValue(0.8f, 0.8f, 0.8f);
  coin_atexit((coin_atexit_f *)SoDiffuseColorElement_cleanup_func);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::init(SoState * state)
{
  inherited::init(state);
  this->colors = defaultdiffusecolor;
  this->packedColors = NULL;
  this->numColors = 1;
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
  if (elem) {
    if (numColors > 0)
      elem->setElt(numColors, colors);
    else
      elem->setElt(1, defaultdiffusecolor);
  }
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
  if (elem) {
    elem->setElt(numColors, colors, packedtransparency);
  }
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
  Returns the color array. Don't use this unless
  SoDiffuseColorElement::isPacked() returns \c FALSE.

  This method is not part of the original SGI Open Inventor v2.1 API.

  \since Coin 1.0.0
*/
const SbColor *
SoDiffuseColorElement::getColorArrayPtr() const
{
#if COIN_DEBUG
  if (this->isPacked()) {
    SoDebugError::postWarning("SoDiffuseColorElement::getColorArrayPtr",
                              "colors are packed -- use getPackedArrayPtr() "
                              "instead");
  }
#endif // COIN_DEBUG
  return this->colors;
}

/*!
  Returns the packed color array. Don't use this unless
  SoDiffuseColorElement::isPacked() returns \c TRUE.

  This method is not part of the original SGI Open Inventor v2.1 API.

  \since Coin 1.0.0
*/
const uint32_t *
SoDiffuseColorElement::getPackedArrayPtr() const
{
#if COIN_DEBUG
  if (!this->isPacked()) {
    SoDebugError::postWarning("SoDiffuseColorElement::getPackedArrayPtr",
                              "colors are *not* packed -- use "
                              "getColorArrayPtr() instead");
  }
#endif // COIN_DEBUG
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
