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
  \class SoTextureUnitElement Inventor/elements/SoTextureUnitElement.h
  \brief The SoTextureUnitElement class is yet to be documented.

  This element is used for keeping control of multitexturing usage.

  FIXME: write doc.

  \ingroup elements
  \COIN_CLASS_EXTENSION
  \since Coin 2.2
*/

#include <Inventor/elements/SoTextureUnitElement.h>
#include <cassert>

SO_ELEMENT_SOURCE(SoTextureUnitElement);

/*!
  This static method initializes static data for the SoTextureUnitElement class.
*/

void
SoTextureUnitElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureUnitElement, inherited);
}

/*!
  The destructor.
*/

SoTextureUnitElement::~SoTextureUnitElement(void)
{
}

//! FIXME: write doc.

void
SoTextureUnitElement::set(SoState * const state,
                          SoNode * const node,
                          const int unit)
{
  SoInt32Element::set(classStackIndex, state, node, unit);
}

//! FIXME: write doc.

void
SoTextureUnitElement::init(SoState * state)
{
  inherited::init(state);
  this->data = 0;
}

//! FIXME: write doc.

int
SoTextureUnitElement::get(SoState * state)
{
  return static_cast<int>(SoInt32Element::get(classStackIndex, state));
}
