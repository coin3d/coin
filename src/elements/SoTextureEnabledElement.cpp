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
  \class SoTextureEnabledElement Inventor/elements/SoTextureEnabledElement.h
  \brief The SoTextureEnabledElement class is an element which stores whether texturing is enabled or not.
  \ingroup elements

  \sa SoTexture3EnabledElement
  \since 2002-10-29
*/

#include <Inventor/elements/SoTextureEnabledElement.h>

SO_ELEMENT_SOURCE(SoTextureEnabledElement);

// doc from parent
void
SoTextureEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureEnabledElement, inherited);
}

/*!
  The destructor.
*/
SoTextureEnabledElement::~SoTextureEnabledElement(void)
{
}

/*!
  Sets the state of this element.
*/
void
SoTextureEnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  inherited::set(classStackIndex, state, node, (int32_t) enabled);
}


// doc from parent
void
SoTextureEnabledElement::init(SoState * state)
{
  this->data = SoTextureEnabledElement::getDefault();
}

/*!
  Return current state of this element. 
*/
SbBool
SoTextureEnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoTextureEnabledElement::getDefault(void)
{
  return FALSE;
}

void 
SoTextureEnabledElement::setElt(int32_t value)
{
  this->data = value;
}


