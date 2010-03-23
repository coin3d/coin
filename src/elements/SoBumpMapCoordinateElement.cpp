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
  \class SoBumpMapCoordinateElement Inventor/elements/SoBumpMapCoordinateElement.h
  \brief The SoBumpMapCoordinateElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoBumpMapCoordinateElement.h>

#include <cassert>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoNode.h>

#include "tidbitsp.h"
#include "SbBasicP.h"

/*!
  \fn SoBumpMapCoordinateElement::numcoords

  FIXME: write doc.
*/

/*!
  \fn SoBumpMapCoordinateElement::coords

  FIXME: write doc.
*/


SO_ELEMENT_SOURCE(SoBumpMapCoordinateElement);

/*!
  This static method initializes static data for the
  SoBumpMapCoordinateElement class.
*/

void
SoBumpMapCoordinateElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoBumpMapCoordinateElement, inherited);
}

/*!
  The destructor.
*/

SoBumpMapCoordinateElement::~SoBumpMapCoordinateElement(void)
{
}

//! FIXME: write doc.

void
SoBumpMapCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->numcoords = 0;
  this->coords = NULL;
}

//! FIXME: write doc.

void
SoBumpMapCoordinateElement::set(SoState * state,
                                SoNode * node,
                                const int32_t numcoords,
                                const SbVec2f * coords)
{
  SoBumpMapCoordinateElement * elem = coin_assert_cast<SoBumpMapCoordinateElement *>
    (SoReplacedElement::getElement(state, classStackIndex, node));
  elem->coords = coords;
  elem->numcoords = numcoords;
}

//! FIXME: write doc.

const SbVec2f &
SoBumpMapCoordinateElement::get(const int index) const
{
  assert(index >= 0 && index < this->numcoords);
  return this->coords[index];
}


//! FIXME: write doc.

const SoBumpMapCoordinateElement *
SoBumpMapCoordinateElement::getInstance(SoState * state)
{
  return coin_assert_cast<const SoBumpMapCoordinateElement *>
    (getConstElement(state, classStackIndex));
}

//! FIXME: write doc.

int32_t
SoBumpMapCoordinateElement::getNum(void) const
{
  return this->numcoords;
}

//! FIXME: write doc.

const SbVec2f *
SoBumpMapCoordinateElement::getArrayPtr(void) const
{
  return this->coords;
}
