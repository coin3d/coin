/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoListenerVelocityElement Inventor/elements/SoListenerVelocityElement.h
  \brief The SoListenerVelocityElement holds the velocity of the current listener.
  \ingroup elements

This velocity is set by SoListener nodes during audio rendering. 
The SoListenerVelocityElement is used when the SoVRMLSound nodes render 
themselves. 
*/

#include <Inventor/elements/SoListenerVelocityElement.h>
#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoListenerVelocityElement::velocity

  The velocity of the listener.
  It is the application programmer's responsibility to 
  set this value. Coin does   not update this value 
  automatically based on changes in a listener's position.
*/

SO_ELEMENT_SOURCE(SoListenerVelocityElement);

/*!
  This static method initializes static data for the
  SoListenerVelocityElement class.
*/

void
SoListenerVelocityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerVelocityElement, inherited);
}

/*!
  The destructor.
*/

SoListenerVelocityElement::~SoListenerVelocityElement(void)
{
}

/*! 
  Initializes the element to it's default value. The default 
  value for the velocity is (0.0f, 0.0f, 0.0f), in other
  words, the listener is not moving. 
*/

void
SoListenerVelocityElement::init(SoState * state)
{
  inherited::init(state);
  this->velocity = SbVec3f(0.0f, 0.0f, 0.0f);
}

/*! 
  Sets the current listener velocity.
*/

void
SoListenerVelocityElement::set(SoState * const state,
                          SoNode * const node,
                          const SbVec3f & velocity)
{
  SoListenerVelocityElement *elem =
    (SoListenerVelocityElement*) SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->velocity = velocity;
  }
}

//! Returns the current listener velocity

const SbVec3f &
SoListenerVelocityElement::get(SoState * const state)
{
  const SoListenerVelocityElement *elem = (SoListenerVelocityElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->velocity;
}

//! Prints the contents of the element (unimplemented)

void
SoListenerVelocityElement::print(FILE * /* file */) const
{
}
