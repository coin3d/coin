/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoSpecularColorElement Inventor/elements/SoSpecularColorElement.h
  \brief The SoSpecularColorElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoSpecularColorElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbColor.h>
#include <assert.h>
#include <stdlib.h>


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
SoSpecularColorElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state;
}

//! FIXME: write doc.

void
SoSpecularColorElement::set(SoState * const state, SoNode * const node,
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
  return (const SoSpecularColorElement *)
    state->getElementNoPush(classStackIndex);
}
