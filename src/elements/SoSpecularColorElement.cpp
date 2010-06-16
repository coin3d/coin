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
  \class SoSpecularColorElement Inventor/elements/SoSpecularColorElement.h
  \brief The SoSpecularColorElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoSpecularColorElement.h>

#include "coindefs.h"
#include "SbBasicP.h"

#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbColor.h>
#include <cassert>
#include <cstdlib>


SO_ELEMENT_SOURCE(SoSpecularColorElement);

/*!
  This static method initializes static data for the SoSpecularColorElement
  class.
*/

void
SoSpecularColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoSpecularColorElement, inherited);
}

/*!
  The destructor.
*/

SoSpecularColorElement::~SoSpecularColorElement()
{
}

//! FIXME: write doc.

void
SoSpecularColorElement::init(SoState * stateptr)
{
  inherited::init(stateptr);
  this->state = stateptr;
}

//! FIXME: write doc.

void
SoSpecularColorElement::set(SoState * const state, SoNode * const COIN_UNUSED_ARG(node),
                            const int32_t numcolors,
                            const SbColor * const colors)
{
  SoLazyElement::setSpecular(state, colors);
#if COIN_DEBUG
  if (numcolors > 1) {
    SoDebugError::postWarning("SoSpecularColorElement::set",
                              "Multiple specular colors not supported. "
                              "All color except the first will be ignored.");
  }
#endif // COIN_DEBIG
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoSpecularColorElement::getNum() const
{
  return 1;
}

//! FIXME: write doc.

const SbColor &
SoSpecularColorElement::get(const int index) const
{
  assert(index == 0);
  return SoLazyElement::getSpecular(this->state);
}

/*!
  Returns a pointer to the color array. This method is not part of the OIV API.
*/
const SbColor *
SoSpecularColorElement::getArrayPtr(void) const
{
  return & SoLazyElement::getSpecular(this->state);
}

//! FIXME: write doc.

const SoSpecularColorElement *
SoSpecularColorElement::getInstance(SoState *state)
{
  return coin_assert_cast<const SoSpecularColorElement *>
    (
     state->getElementNoPush(classStackIndex)
    );
}
