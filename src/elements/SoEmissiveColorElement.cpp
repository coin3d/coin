/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoEmissiveColorElement Inventor/elements/SoEmissiveColorElement.h
  \brief The SoEmissiveColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoEmissiveColorElement.h>

#include <Inventor/SbColor.h>
#include <assert.h>

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbColor * defaultemissivecolor = NULL;


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
  defaultemissivecolor = new SbColor; // FIXME: deallocate on exit. 20000406 mortene.
  defaultemissivecolor->setValue(0.0f, 0.0f, 0.0f);
}

/*!
  The destructor.
*/

SoEmissiveColorElement::~SoEmissiveColorElement()
{
}

//! FIXME: write doc.

void
SoEmissiveColorElement::init(SoState * /* state */)
{
  this->colors = defaultemissivecolor;
  this->numColors = 1;
}

//! FIXME: write doc.

void
SoEmissiveColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numColors,
                           const SbColor * const colors)
{
  SoEmissiveColorElement *elem = (SoEmissiveColorElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (numColors > 0)
    elem->setElt(numColors, colors);
  else
    elem->setElt(1, defaultemissivecolor);
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
