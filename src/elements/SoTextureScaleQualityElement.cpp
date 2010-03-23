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
  \class SoTextureScaleQualityElement Inventor/elements/SoTextureScaleQualityElement.h
  \brief The SoTextureScaleQualityElement class is yet to be documented.
  \ingroup elements

  This is currently an internal Coin element. The header file is not
  installed, and the API for this element might change without notice.

*/

#include "elements/SoTextureScaleQualityElement.h"

#include <cassert>

SO_ELEMENT_SOURCE(SoTextureScaleQualityElement);

/*!
  This static method initializes static data for the SoTextureScaleQualityElement class.
*/

void
SoTextureScaleQualityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureScaleQualityElement, inherited);
}

/*!
  The destructor.
*/

SoTextureScaleQualityElement::~SoTextureScaleQualityElement(void)
{
}

//! FIXME: write doc.

void
SoTextureScaleQualityElement::set(SoState * state,
                                  SoNode * node,
                                  const float quality)
{
  inherited::set(classStackIndex, state, node, quality);
}

//! FIXME: write doc.

void
SoTextureScaleQualityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.
float
SoTextureScaleQualityElement::get(SoState * state)
{
  return inherited::get(classStackIndex, state);
}

//! FIXME: write doc.
float
SoTextureScaleQualityElement::getDefault(void)
{
  return 0.5f;
}
