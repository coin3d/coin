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
  \class SoListenerPositionElement Inventor/elements/SoListenerPositionElement.h
  \brief The SoListenerPositionElement holds the position of the current listener.
  \ingroup elements

This position is set by SoListener nodes and SoCamera Nodes during audio 
rendering. When a SoListener is visited by the SoAudioRenderAction, 
it will add a new SoListenerPositionElement to the state, holding it's position
and with the setbylistener flag set. When a SoCamera is visited by 
SoAudioRenderAction, it will add a new SoListenerPositionElement only if there 
are no previous elements with the setbylistener flag set. 

The SoListenerPositionElement is used when the SoVRMLSound nodes render 
themselves. 
*/

#include <Inventor/elements/SoListenerPositionElement.h>
#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoListenerPositionElement::position

  The position of the listener. Can be set by the SoListener class or the
  SoCamera class.
*/

SO_ELEMENT_SOURCE(SoListenerPositionElement);

/*!
  This static method initializes static data for the
  SoListenerPositionElement class.
*/

void
SoListenerPositionElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerPositionElement, inherited);
}

/*!
  The destructor.
*/

SoListenerPositionElement::~SoListenerPositionElement(void)
{
}

/*! 
  Initializes the element to it's default value. The default 
  value for the position is (0.0f, 0.0f, 0.0f) and the 
  default value for the setByListener flag is FALSE.
*/

void
SoListenerPositionElement::init(SoState * state)
{
  inherited::init(state);
  this->position = SbVec3f(0.0f, 0.0f, 0.0f);
  this->setbylistener = FALSE;
}

/*! 
  Sets the current listener position, and indicates if it was set
  by a SoListener node or a SoCamera node.
*/

void
SoListenerPositionElement::set(SoState * const state,
                               SoNode * const node,
                               const SbVec3f & position, 
                               SbBool setbylistener)
{
  SoListenerPositionElement *elem =
    (SoListenerPositionElement*) SoElement::getElement(state, classStackIndex);
  if (elem) {
    elem->position = position;
    elem->setbylistener = setbylistener;
  }
}

//! Returns the current listener position

const SbVec3f &
SoListenerPositionElement::get(SoState * const state)
{
  const SoListenerPositionElement *elem = (SoListenerPositionElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->position;
}

/*! 
  Returns TRUE if the position was set by a SoListener node, 
  and FALSE if it was set by a SoCamera node
*/

SbBool
SoListenerPositionElement::isSetByListener(SoState * const state)
{
  const SoListenerPositionElement *elem = (SoListenerPositionElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->setbylistener;
}

//! Prints contents of element (not implemented)

void
SoListenerPositionElement::print(FILE * /* file */) const
{
}
