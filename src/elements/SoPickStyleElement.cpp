/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoPickStyleElement Inventor/elements/SoPickStyleElement.h
  \brief The SoPickStyleElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoPickStyleElement.h>


#include <cassert>

/*!
  \fn SoPickStyleElement::Style

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoPickStyleElement);

/*!
  This static method initializes static data for the SoPickStyleElement class.
*/

void
SoPickStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPickStyleElement, inherited);
}

/*!
  The destructor.
*/

SoPickStyleElement::~SoPickStyleElement(void)
{
}

//! FIXME: write doc.

void
SoPickStyleElement::set(SoState * const state,
                        SoNode * const node,
                        const int32_t style)
{
  assert(style >= SHAPE && style <= SHAPE_FRONTFACES);
  SoInt32Element::set(classStackIndex, state, node, style);
}

//! FIXME: write doc.

void
SoPickStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoPickStyleElement::set(SoState * const state, const Style style)
{
  set(state, NULL, style);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoPickStyleElement::Style
SoPickStyleElement::get(SoState * const state)
{
  return static_cast<Style>(SoInt32Element::get(classStackIndex, state));
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoPickStyleElement::Style
SoPickStyleElement::getDefault()
{
  return SHAPE;
}
