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
  \class SoMaterialBindingElement Inventor/elements/SoMaterialBindingElement.h
  \brief The SoMaterialBindingElement class is yet to be documented.
  \ingroup elements

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
