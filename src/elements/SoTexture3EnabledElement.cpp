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
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGTexture3EnabledElement class is a an element which stores
  whether 3D texturing is enabled or not.
  \ingroup elements

  \sa SoTextureEnabledElement
  \since 2002-10-29
*/

#include <Inventor/elements/SoTexture3EnabledElement.h>

SO_ELEMENT_SOURCE(SoTexture3EnabledElement);


// doc from parent
void
SoTexture3EnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTexture3EnabledElement, inherited);
}

/*!
  The destructor.
*/
SoTexture3EnabledElement::~SoTexture3EnabledElement(void)
{
}

/*!
  Sets the state of this element.
*/
void
SoTexture3EnabledElement::set(SoState * const state,
                               SoNode * const node,
                               const SbBool enabled)
{
  inherited::set(classStackIndex, state, node, (int32_t) enabled);
}


// doc from parent
void
SoTexture3EnabledElement::init(SoState * state)
{
  this->data = SoTexture3EnabledElement::getDefault();
}

/*!
  Return current state of this element. 
*/
SbBool
SoTexture3EnabledElement::get(SoState * const state)
{
  return (SbBool) inherited::get(classStackIndex, state);
}

/*!
  Returns default state of this element (FALSE).
*/
SbBool
SoTexture3EnabledElement::getDefault(void)
{
  return FALSE;
}

void
SoTexture3EnabledElement::setElt(int32_t value)
{
  this->data = value;
}


