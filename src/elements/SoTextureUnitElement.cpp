/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTextureUnitElement Inventor/elements/SoTextureUnitElement.h
  \brief The SoTextureUnitElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \since 2003-10-27
*/

#include <Inventor/elements/SoTextureUnitElement.h>
#include <assert.h>

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
SoTextureUnitElement::get(SoState * const state)
{
  return (int) SoInt32Element::get(classStackIndex, state);
}


