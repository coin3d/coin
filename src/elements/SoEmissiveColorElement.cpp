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
  \class SoEmissiveColorElement Inventor/elements/SoEmissiveColorElement.h
  \brief The SoEmissiveColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoEmissiveColorElement.h>

#include <Inventor/SbColor.h>
#include <assert.h>
#include <stdlib.h>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbColor * defaultemissivecolor = NULL;

static void
SoEmissiveColorElement_cleanup_func(void)
{
  delete defaultemissivecolor;
}

/*!
  \fn SoEmissiveColorElement::numColors

  FIXME: write doc.
*/

/*!
  \fn SoEmissiveColorElement::colors

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoEmissiveColorElement);

/*!
  This static method initializes static data for the SoEmissiveColorElement
  class.
*/

void
SoEmissiveColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoEmissiveColorElement, inherited);
  defaultemissivecolor = new SbColor;
  defaultemissivecolor->setValue(0.0f, 0.0f, 0.0f);
  (void)atexit(SoEmissiveColorElement_cleanup_func);
}

/*!
  The destructor.
*/

SoEmissiveColorElement::~SoEmissiveColorElement()
{
}

//! FIXME: write doc.

void
SoEmissiveColorElement::init(SoState * state)
{
  inherited::init(state);
  this->colors = defaultemissivecolor;
  this->numColors = 1;
}

//! FIXME: write doc.

void
SoEmissiveColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const SbColor * const colors)
{
  SoEmissiveColorElement * elem = (SoEmissiveColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (elem) {
    if (numColors > 0)
      elem->setElt(numColors, colors);
    else
      elem->setElt(1, defaultemissivecolor);
  }
}

//! FIXME: write doc.

void
SoEmissiveColorElement::setElt(const int32_t numColors,
                              const SbColor * const colors)
{
  this->colors = colors;
  this->numColors = numColors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoEmissiveColorElement::getNum() const
{
  return this->numColors;
}

//! FIXME: write doc.

const SbColor &
SoEmissiveColorElement::get(const int index) const
{
  assert(index >= 0 && index <= this->numColors);
  return this->colors[index];
}

/*!
  Returns a pointer to the colora array. This method is not part of the OIV API.
*/
const SbColor *
SoEmissiveColorElement::getArrayPtr() const
{
  return this->colors;
}

//! FIXME: write doc.

//$ EXPORT INLINE
const SoEmissiveColorElement *
SoEmissiveColorElement::getInstance(SoState *state)
{
  return (const SoEmissiveColorElement *)
    SoElement::getConstElement(state, classStackIndex);
}
