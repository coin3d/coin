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
  \class SoTextOutlineEnabledElement Inventor/elements/SoTextOutlineEnabledElement.h
  \brief The SoTextOutlineEnabledElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/


#include <Inventor/elements/SoTextOutlineEnabledElement.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoTextOutlineEnabledElement);

/*!
  This static method initializes static data for the
  SoTextOutlinedEnabledElement class.
*/

void
SoTextOutlineEnabledElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextOutlineEnabledElement, inherited);
}

/*!
  The destructor.
*/

SoTextOutlineEnabledElement::~SoTextOutlineEnabledElement(void)
{
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::set(SoState * const state,
                                 SoNode * const node,
                                 const SbBool enabled)
{
    SoInt32Element::set(classStackIndex, state, node, enabled);
}

/*!
  FIXME: write doc.
*/

void
SoTextOutlineEnabledElement::set(SoState * const state, const SbBool enabled)
{
  set(state, NULL, enabled);
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::get(SoState * const state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

/*!
  FIXME: write doc.
*/

SbBool
SoTextOutlineEnabledElement::getDefault(void)
{
  return FALSE;
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::init(SoState * state)
{
    inherited::init(state);
    this->data = getDefault();
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::push(SoState * state)
{
    inherited::push(state);
}

//! FIXME: write doc.

void
SoTextOutlineEnabledElement::pop(SoState * state,
                                 const SoElement * prevTopElement)
{
    inherited::pop(state, prevTopElement);
}
