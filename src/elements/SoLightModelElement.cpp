/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoLightModelElement Inventor/elements/SoLightModelElement.h
  \brief The SoLightModelElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightModelElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa void * SoLightModelElement::createInstance(void)
*/

SoLightModelElement::SoLightModelElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
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
  data = PHONG;
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, const Model model)
{
  SoInt32Element::set(classStackIndex, state, model);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, SoNode * const node,
                         const Model model)
{
  SoInt32Element::set(classStackIndex, state, node, model);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

SoLightModelElement::Model
SoLightModelElement::get(SoState * const state)
{
  return (Model) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

SoLightModelElement::Model
SoLightModelElement::getDefault()
{
  return PHONG;
}
