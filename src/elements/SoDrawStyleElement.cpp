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
  \class SoDrawStyleElement Inventor/elements/SoDrawStyleElement.h
  \brief The SoDrawStyleElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoDrawStyleElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


#include <assert.h>

/*!
  \fn SoDrawStyleElement::Style

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoDrawStyleElement);

/*!
  This static method initializes static data for the
  SoDrawStyleElement class.
*/

void
SoDrawStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDrawStyleElement, inherited);
}

/*!
  The destructor.
*/

SoDrawStyleElement::~SoDrawStyleElement(void)
{
}

//! FIXME: write doc.

void
SoDrawStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoDrawStyleElement::set(SoState * const state,
                        SoNode * const node,
                        const Style style)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t)style);
  SoShapeStyleElement::setDrawStyle(state, (int32_t)style);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoDrawStyleElement::set(SoState * const state, const Style style)
{
  set(state, NULL, style);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDrawStyleElement::Style
SoDrawStyleElement::get(SoState * const state)
{
  return (Style) inherited::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDrawStyleElement::Style
SoDrawStyleElement::getDefault()
{
  return FILLED;
}
