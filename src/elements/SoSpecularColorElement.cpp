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
  \class SoSpecularColorElement Inventor/elements/SoSpecularColorElement.h
  \brief The SoSpecularColorElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoSpecularColorElement.h>
#include <Inventor/SbColor.h>
#include <assert.h>
#include <stdlib.h>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbColor * defaultspecularcolor = NULL;

static void
SoSpecularColorElement_cleanup_func(void)
{
  delete defaultspecularcolor;
}

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
  defaultspecularcolor = new SbColor;
  defaultspecularcolor->setValue(0.0f, 0.0f, 0.0f);
  (void)atexit(SoSpecularColorElement_cleanup_func);
}

/*!
  The destructor.
*/

SoSpecularColorElement::~SoSpecularColorElement()
{
}

//! FIXME: write doc.

void
SoSpecularColorElement::init(SoState * state)
{
  inherited::init(state);
  this->colors = defaultspecularcolor;
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
  if (elem) {
    if (numColors > 0)
      elem->setElt(numColors, colors);
    else
      elem->setElt(1, defaultspecularcolor);
  }
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
