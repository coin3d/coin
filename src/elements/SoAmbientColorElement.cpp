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
  \class SoAmbientColorElement Inventor/elements/SoAmbientColorElement.h
  \brief The SoAmbientColorElement class is yet to be documented.
  \ingroup elements
*/

#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/SbColor.h>
#include <assert.h>
#include <stdlib.h>
#include <../tidbits.h> // coin_atexit()

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbColor * defaultambientcolor = NULL;

static void
SoAmbientColorElement_cleanup_func(void)
{
  delete defaultambientcolor;
}

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
SoAmbientColorElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAmbientColorElement, inherited);
  defaultambientcolor = new SbColor;
  defaultambientcolor->setValue(0.2f, 0.2f, 0.2f);
  coin_atexit(SoAmbientColorElement_cleanup_func);
}

//! FIXME: write doc.

void
SoAmbientColorElement::init(SoState * state)
{
  inherited::init(state);
  this->colors = defaultambientcolor;
  this->numColors = 1;
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
  if (elem) {
    if (numColors > 0)
      elem->setElt(numColors, colors);
    else
      elem->setElt(1, defaultambientcolor);
  }
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
