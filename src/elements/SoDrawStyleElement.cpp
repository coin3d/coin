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
  \class SoDrawStyleElement Inventor/elements/SoDrawStyleElement.h
  \brief The SoDrawStyleElement class is yet to be documented.

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
  A constructor.  Can't be used directly.

  \sa void * SoDrawStyleElement::createInstance(void)
*/

SoDrawStyleElement::SoDrawStyleElement(void)
{
  setTypeId(SoDrawStyleElement::classTypeId);
  setStackIndex(SoDrawStyleElement::classStackIndex);
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
