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
  \class SoTextOutlineEnabledElement Inventor/elements/SoTextOutlineEnabledElement.h
  \brief The SoTextOutlineEnabledElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.

  \since Coin 1.0
  \since TGS Inventor 2.4
*/


#include <Inventor/elements/SoTextOutlineEnabledElement.h>
#include <cassert>

SO_ELEMENT_SOURCE(SoTextOutlineEnabledElement);

/*!
  This static method initializes static data for the
  SoTextOutlinedEnabledElement class.
*/

void
SoTextOutlineEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextOutlineEnabledElement, inherited);
}

/*!
  The destructor.
*/

SoTextOutlineEnabledElement::~SoTextOutlineEnabledElement(void)
{
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::set(SoState * const state,
                                 SoNode * const node,
                                 const SbBool enabled)
{
  SoInt32Element::set(classStackIndex, state, node, enabled);
}

/*!
  FIXME: write doc.
*/

void
SoTextOutlineEnabledElement::set(SoState * const state, const SbBool enabled)
{
  set(state, NULL, enabled);
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::get(SoState * const state)
{
  return static_cast<SbBool>(SoInt32Element::get(classStackIndex, state));
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::getDefault(void)
{
  return FALSE;
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::push(SoState * state)
{
  inherited::push(state);
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::pop(SoState * state,
                                 const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}
