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
  \class SoTextureScalePolicyElement Inventor/elements/SoTextureScalePolicyElement.h
  \brief The SoTextureScalePolicyElement class is yet to be documented.
  \ingroup elements

  This is currently an internal Coin element. The header file is not
  installed, and the API for this element might change without notice.  
*/

#include <Inventor/elements/SoTextureScalePolicyElement.h>


#include <assert.h>

/*!
  \fn SoTextureScalePolicyElement::Policy

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoTextureScalePolicyElement);

/*!
  This static method initializes static data for the SoTextureScalePolicyElement class.
*/

void
SoTextureScalePolicyElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureScalePolicyElement, inherited);
}

/*!
  The destructor.
*/

SoTextureScalePolicyElement::~SoTextureScalePolicyElement(void)
{
}

//! FIXME: write doc.

void
SoTextureScalePolicyElement::set(SoState * const state,
                                 SoNode * const node,
                                 const Policy policy)
{
  inherited::set(classStackIndex, state, node, (int) policy);
}

//! FIXME: write doc.

void
SoTextureScalePolicyElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.
void
SoTextureScalePolicyElement::set(SoState * const state, const Policy policy)
{
  SoTextureScalePolicyElement::set(state, NULL, policy);
}

//! FIXME: write doc.
SoTextureScalePolicyElement::Policy
SoTextureScalePolicyElement::get(SoState * const state)
{
  return (Policy) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.
SoTextureScalePolicyElement::Policy
SoTextureScalePolicyElement::getDefault(void)
{
  return USE_TEXTURE_QUALITY;
}
