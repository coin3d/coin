/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
