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
  \class SoAmbientColorElement Inventor/elements/SoAmbientColorElement.h
  \brief The SoAmbientColorElement class is yet to be documented.
  \ingroup elements
*/


#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbColor.h>
#include <assert.h>
#include <stdlib.h>

SO_ELEMENT_SOURCE(SoAmbientColorElement);

/*!
  This static method initializes static data for the
  SoAmbientColorElement class.
*/

void
SoAmbientColorElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAmbientColorElement, inherited);
}

//! FIXME: write doc.

void
SoAmbientColorElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
}

/*!
  The destructor.
*/

SoAmbientColorElement::~SoAmbientColorElement()
{
}

//! FIXME: write doc.

void
SoAmbientColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numcolors,
                           const SbColor * const colors)
{
  SoLazyElement::setAmbient(state, colors);
#if COIN_DEBUG
  if (numcolors > 1) {
    SoDebugError::postWarning("SoAmbientColorElement::set",
                              "Multiple ambient colors not supported. "
                              "All color except the first will be ignored.");
  }
#endif // COIN_DEBIG
}

//! FIXME: write doc.

int32_t
SoAmbientColorElement::getNum(void) const
{
  return 1;
}

//! FIXME: write doc.

const SbColor &
SoAmbientColorElement::get(const int index) const
{
  assert(index == 0);
  return SoLazyElement::getAmbient(this->state);
}

/*!
  Return a pointer to the color array. This method is not part of the OIV API.
*/
const SbColor *
SoAmbientColorElement::getArrayPtr(void) const
{
  return &SoLazyElement::getAmbient(this->state);
}

//! FIXME: write doc.

const SoAmbientColorElement *
SoAmbientColorElement::getInstance(SoState *state)
{
  return (const SoAmbientColorElement *)
    state->getElementNoPush(classStackIndex);
}
