/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoLightModelElement Inventor/elements/SoLightModelElement.h
  \brief The SoLightModelElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoLightModelElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


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
  data = PHONG;
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, const Model model)
{
  SoInt32Element::set(classStackIndex, state, model);
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
}

//! FIXME: write doc.

void
SoLightModelElement::set(SoState * const state, SoNode * const node,
                         const Model model)
{
  SoInt32Element::set(classStackIndex, state, node, model);
  SoShapeStyleElement::setLightModel(state, (int32_t)model);
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

//! FIXME: write doc

const SoLightModelElement *
SoLightModelElement::getInstance(SoState *state)
{
  return (const SoLightModelElement *)
    SoElement::getConstElement(state, classStackIndex);
}
