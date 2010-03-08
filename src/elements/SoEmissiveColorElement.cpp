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
  \class SoEmissiveColorElement Inventor/elements/SoEmissiveColorElement.h
  \brief The SoEmissiveColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoEmissiveColorElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbColor.h>
#include <cassert>
#include <cstdlib>

#include "coindefs.h"
#include "SbBasicP.h"

SO_ELEMENT_SOURCE(SoEmissiveColorElement);

/*!
  This static method initializes static data for the SoEmissiveColorElement
  class.
*/

void
SoEmissiveColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoEmissiveColorElement, inherited);
}

/*!
  The destructor.
*/

SoEmissiveColorElement::~SoEmissiveColorElement()
{
}

//! FIXME: write doc.

void
SoEmissiveColorElement::init(SoState * stateptr)
{
  inherited::init(stateptr);
  this->state = stateptr;
}

//! FIXME: write doc.

void
SoEmissiveColorElement::set(SoState * const state, SoNode * const COIN_UNUSED_ARG(node),
                            const int32_t numcolors,
                            const SbColor * const colors)
{
  SoLazyElement::setEmissive(state, colors);
#if COIN_DEBUG
  if (numcolors > 1) {
    SoDebugError::postWarning("SoEmissiveColorElement::set",
                              "Multiple emissive colors not supported. "
                              "All color except the first will be ignored.");
  }
#endif // COIN_DEBIG
}


//! FIXME: write doc.

int32_t
SoEmissiveColorElement::getNum(void) const
{
  return 1;
}

//! FIXME: write doc.

const SbColor &
SoEmissiveColorElement::get(const int index) const
{
  assert(index == 0);
  return SoLazyElement::getEmissive(this->state);
}

/*!
  Returns a pointer to the color array. This method is not part of the OIV API.
*/
const SbColor *
SoEmissiveColorElement::getArrayPtr(void) const
{
  return & SoLazyElement::getEmissive(this->state);
}

//! FIXME: write doc.

const SoEmissiveColorElement *
SoEmissiveColorElement::getInstance(SoState *state)
{
  return coin_assert_cast<const SoEmissiveColorElement *>
    (
    state->getElementNoPush(classStackIndex)
    );
}
