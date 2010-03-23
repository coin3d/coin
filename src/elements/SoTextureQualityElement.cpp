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
  \class SoTextureQualityElement Inventor/elements/SoTextureQualityElement.h
  \brief The SoTextureQualityElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureQualityElement.h>
#include <cassert>

SO_ELEMENT_SOURCE(SoTextureQualityElement);

/*!
  This static method initializes static data for the
  SoTextureQualityElement class.
*/

void
SoTextureQualityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureQualityElement, inherited);
}

/*!
  The destructor.
*/

SoTextureQualityElement::~SoTextureQualityElement()
{
}

//! FIXME: write doc.

void
SoTextureQualityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoTextureQualityElement::getDefault();
}

/*!
  FIXME: write doc.
*/
void
SoTextureQualityElement::set(SoState * const state, SoNode * const node,
                             const float quality)
{
  SoFloatElement::set(classStackIndex, state, node, quality);
}

/*!
  FIXME: write doc.
*/
void
SoTextureQualityElement::set(SoState * const state, const float quality)
{
  set(state, NULL, quality);
}

/*!
  FIXME: write doc.
*/
float
SoTextureQualityElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

/*!
  FIXME: write doc.
*/
float
SoTextureQualityElement::getDefault(void)
{
  return 0.5f;
}
