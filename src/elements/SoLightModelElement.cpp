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
  \class SoLightModelElement Inventor/elements/SoLightModelElement.h
  \brief The SoLightModelElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#ifdef COIN_INTERNAL
#define COIN_STOREDINTERNAL COIN_INTERNAL
#undef COIN_INTERNAL
#endif // COIN_INTERNAL
#include <Inventor/elements/SoLightModelElement.h>
#ifdef COIN_STOREDINTERNAL
#define COIN_INTERNAL COIN_STOREDINTERNAL
#undef COIN_STOREDINTERNAL
#endif // COIN_STOREDINTERNAL

#include <Inventor/elements/SoLazyElement.h>
#include <assert.h>

/*!
  \fn SoLightModelElement::Model

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoLightModelElement);

/*!
  This static method initializes static data for the
  SoLightModelElement class.
*/

void
SoLightModelElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoLightModelElement, inherited);
}

/*!
  The destructor.
*/

SoLightModelElement::~SoLightModelElement()
{
}

//! FIXME: write doc.

void
SoLightModelElement::init(SoState * /* state */)
{
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, const Model model)
{
  SoLazyElement::setLightModel(state, (int32_t) model);
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, SoNode * const node,
                         const Model model)
{
  SoLazyElement::setLightModel(state, (int32_t) model);
}

//! FIXME: write doc.

SoLightModelElement::Model
SoLightModelElement::get(SoState * const state)
{
  return (SoLightModelElement::Model) SoLazyElement::getLightModel(state);
}

//! FIXME: write doc.

SoLightModelElement::Model
SoLightModelElement::getDefault()
{
  return (SoLightModelElement::Model) SoLazyElement::getDefaultLightModel();
}

//! FIXME: write doc

const SoLightModelElement *
SoLightModelElement::getInstance(SoState *state)
{
  return (const SoLightModelElement *)
    state->getElementNoPush(classStackIndex);
}
