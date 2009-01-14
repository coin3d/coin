/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGTexture3EnabledElement class is a an element which stores
  whether 3D texturing is enabled or not.
  \ingroup elements

  \COIN_CLASS_EXTENSION

  \sa SoTextureEnabledElement
  \since Coin 2.0
  \since TGS Inventor 2.6
*/

#include <Inventor/elements/SoTexture3EnabledElement.h>

#include "coindefs.h"

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
  inherited::set(classStackIndex, state, node, static_cast<int32_t>(enabled));
}


// doc from parent
void
SoTexture3EnabledElement::init(SoState * COIN_UNUSED(state))
{
  this->data = SoTexture3EnabledElement::getDefault();
}

/*!
  Return current state of this element.
*/
SbBool
SoTexture3EnabledElement::get(SoState * const state)
{
  return static_cast<SbBool>(inherited::get(classStackIndex, state));
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
