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
  \class SoFocalDistanceElement Inventor/elements/SoFocalDistanceElement.h
  \brief The SoFocalDistanceElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoFocalDistanceElement.h>


#include <cassert>

SO_ELEMENT_SOURCE(SoFocalDistanceElement);

/*!
  This static method initializes static data for the
  SoFocalDistanceElement class.
*/

void
SoFocalDistanceElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoFocalDistanceElement, inherited);
}

/*!
  The destructor.
*/

SoFocalDistanceElement::~SoFocalDistanceElement(void)
{
}

//! FIXME: write doc.

void
SoFocalDistanceElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoFocalDistanceElement::set(SoState * const state, SoNode * const node,
                            const float distance)
{
  SoFloatElement::set(classStackIndex, state, node, distance);
}

//! FIXME: write doc.

void
SoFocalDistanceElement::set(SoState * const state, const float distance)
{
  SoFocalDistanceElement::set(state, NULL, distance);
}

//! FIXME: write doc.

float
SoFocalDistanceElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

float
SoFocalDistanceElement::getDefault()
{
  return 5.0f;
}
