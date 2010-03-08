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
  \class SoListenerGainElement Inventor/elements/SoListenerGainElement.h
  \brief The SoListenerGainElement class stores the SoListener gain during a scene graph traversal.
  \ingroup elements

  This gain is set by SoListener nodes during audio rendering.
  The SoListenerGainElement is used when the SoVRMLSound nodes render
  themselves.

  \COIN_CLASS_EXTENSION

  \since Coin 2.0
*/

#include <Inventor/elements/SoListenerGainElement.h>


#include <cassert>

SO_ELEMENT_SOURCE(SoListenerGainElement);

// doc from parent
void
SoListenerGainElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerGainElement, inherited);
}

/*!
  The destructor.
*/
SoListenerGainElement::~SoListenerGainElement(void)
{
}

// doc from parent
void
SoListenerGainElement::init(SoState * state)
{
  inherited::init(state);

  this->data = 1.0f;
}

/*!
  Sets the value of this element.
*/
void
SoListenerGainElement::set(SoState * const state, SoNode * const node,
                          const float gain)
{
  SoFloatElement::set(classStackIndex, state, node, gain);
}


/*!
  \overload
*/
void
SoListenerGainElement::set(SoState * const state, const float gain)
{
  SoListenerGainElement::set(state, NULL, gain);
}

/*!
  Returns the element value.
*/
float
SoListenerGainElement::get(SoState * const state)
{
  float val = SoFloatElement::get(classStackIndex, state);
  return val;
}
