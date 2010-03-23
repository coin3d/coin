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
  \class SoShadowStyleElement Inventor/elements/SoShadowStyleElement.h
  \brief The SoShadowStyleElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \since Coin 2.5
*/

#include <Inventor/annex/FXViz/elements/SoShadowStyleElement.h>


#include <assert.h>

/*!
  \fn SoShadowStyleElement::Style

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoShadowStyleElement);

// doc in parent
void
SoShadowStyleElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoShadowStyleElement, inherited);
}

/*!
  The destructor.
*/

SoShadowStyleElement::~SoShadowStyleElement(void)
{
}

//! FIXME: write doc.

void
SoShadowStyleElement::set(SoState * const state,
                          SoNode * const node,
                          const int style)
{
  SoInt32Element::set(classStackIndex, state, node, style);
}

//! FIXME: write doc.

void
SoShadowStyleElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoShadowStyleElement::set(SoState * const state, const int style)
{
  SoShadowStyleElement::set(state, NULL, style);
}

//! FIXME: write doc.

int
SoShadowStyleElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

int
SoShadowStyleElement::getDefault(void)
{
  return CASTS_SHADOW_AND_SHADOWED;
}


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  BOOST_CHECK_MESSAGE(SoShadowStyleElement::getClassStackIndex() != -1,
                      "missing class initialization");
}

#endif // COIN_TEST_SUITE
