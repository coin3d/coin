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
  \class SoListenerDopplerElement Inventor/elements/SoListenerDopplerElement.h
  \brief The SoListenerDopplerElement holds the doppler velocity and factor of the current listener.
  \ingroup elements

  The dopplerVelocity and dopplerFactor is set by SoListener nodes
  during audio rendering. The SoListenerDopplerElement is used when
  the SoVRMLSound nodes render themselves.

  \COIN_CLASS_EXTENSION

  \since Coin 2.0
*/

#include <Inventor/elements/SoListenerDopplerElement.h>

#include "coindefs.h"
#include "SbBasicP.h"

#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoListenerDopplerElement::dopplerVelocity

  The doppler velocity of the listener.
  It is the application programmer's responsibility to
  set this value. Coin does not update this value
  automatically based on changes in a listener's position.
*/

/*!
  \fn SoListenerDopplerElement::dopplerFactor

  The amount of doppler effect applied to the listener. The normal
  range would be [0.0f to 1.0f], where 0.0f is default and disable all
  doppler effects.
*/

SO_ELEMENT_SOURCE(SoListenerDopplerElement);

/*!
  This static method initializes static data for the
  SoListenerDopplerElement class.
*/

void
SoListenerDopplerElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerDopplerElement, inherited);
}

/*!
  The destructor.
*/

SoListenerDopplerElement::~SoListenerDopplerElement(void)
{
}

/*!  Initializes the element to it's default value. The default value
  for the velocity is (0.0f, 0.0f, 0.0f), in other words, the listener
  is not moving. The default value for the dopplerFactor is 0.0f, in
  other words, doppler effect is disabled.  */

void
SoListenerDopplerElement::init(SoState * state)
{
  inherited::init(state);
  this->dopplerVelocity = SbVec3f(0.0f, 0.0f, 0.0f);
  this->dopplerFactor = 0.0f;
}

/*!
  Sets the current listener's doppler velocity.
*/

void
SoListenerDopplerElement::setDopplerVelocity(SoState * const state,
                                             SoNode * const COIN_UNUSED_ARG(node),
                                             const SbVec3f & velocity)
{
  SoListenerDopplerElement * elem =
    coin_safe_cast<SoListenerDopplerElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );

  if (elem) {
    elem->dopplerVelocity = velocity;
  }
}

/*!
  Sets the current listener's doppler factor.
*/

void
SoListenerDopplerElement::setDopplerFactor(SoState * const state,
                                             SoNode * const COIN_UNUSED_ARG(node),
                                             float factor)
{
  SoListenerDopplerElement * elem =
    coin_safe_cast<SoListenerDopplerElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->dopplerFactor = factor;
  }
}

//! Returns the current listener's doppler velocity

const SbVec3f &
SoListenerDopplerElement::getDopplerVelocity(SoState * const state)
{
  const SoListenerDopplerElement * elem =
    coin_assert_cast<const SoListenerDopplerElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->dopplerVelocity;
}

//! Returns the current listener's doppler factor

float
SoListenerDopplerElement::getDopplerFactor(SoState * const state)
{
  const SoListenerDopplerElement * elem =
    coin_assert_cast<const SoListenerDopplerElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->dopplerFactor;
}

//! Prints the contents of the element (unimplemented)

void
SoListenerDopplerElement::print(FILE * /* file */) const
{
}
