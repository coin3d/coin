/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoPickStyleElement Inventor/elements/SoPickStyleElement.h
  \brief The SoPickStyleElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoPickStyleElement.h>


#include <assert.h>

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
  assert(style >= SHAPE && style <= UNPICKABLE);
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
  return (Style) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoPickStyleElement::Style
SoPickStyleElement::getDefault()
{
  return SHAPE;
}
