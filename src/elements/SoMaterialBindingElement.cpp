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
  \class SoMaterialBindingElement Inventor/elements/SoMaterialBindingElement.h
  \brief The SoMaterialBindingElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoMaterialBindingElement.h>


#include <assert.h>

/*!
  \fn SoMaterialBindingElement::Binding

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoMaterialBindingElement);

/*!
  This static method initializes static data for the
  SoMaterialBindingElement class.
*/

void
SoMaterialBindingElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoMaterialBindingElement, inherited);
}

/*!
  The destructor.
*/

SoMaterialBindingElement::~SoMaterialBindingElement(void)
{
}

//! FIXME: write doc.

void
SoMaterialBindingElement::set(SoState * const state,
                              SoNode * const node,
                              const Binding binding)
{
  assert((int)binding >= (int)OVERALL &&
         (int)binding <= (int)PER_VERTEX_INDEXED);
  SoInt32Element::set(classStackIndex, state, node, binding);
}

//! FIXME: write doc.

void
SoMaterialBindingElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoMaterialBindingElement::set(SoState * const state, const Binding binding)
{
  set(state, NULL, binding);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoMaterialBindingElement::Binding
SoMaterialBindingElement::get(SoState * const state)
{
  return (Binding) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoMaterialBindingElement::Binding
SoMaterialBindingElement::getDefault()
{
  return DEFAULT;
}
