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
  \class SoListenerGainElement Inventor/elements/SoListenerGainElement.h
  \brief The SoListenerGainElement class stores the SoListener gain during a scene graph traversal.
  \ingroup elements

  This gain is set by SoListener nodes during audio rendering. 
  The SoListenerGainElement is used when the SoVRMLSound nodes render 
  themselves. 
  
  \since 2002-11-12
*/

#include <Inventor/elements/SoListenerGainElement.h>


#include <assert.h>

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
