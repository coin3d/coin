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
  \class SoTextureCoordinateBindingElement Inventor/elements/SoTextureCoordinateBindingElement.h
  \brief The SoTextureCoordinateBindingElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureCoordinateBindingElement.h>


#include <assert.h>

/*!
  \fn SoTextureCoordinateBindingElement::Binding

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoTextureCoordinateBindingElement);

/*!
  This static method initializes static data for the
  SoTextureCoordinateBindingElement class.
*/

void
SoTextureCoordinateBindingElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureCoordinateBindingElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTextureCoordinateBindingElement::createInstance(void)
*/

SoTextureCoordinateBindingElement::SoTextureCoordinateBindingElement(void)
{
    setTypeId(SoTextureCoordinateBindingElement::classTypeId);
    setStackIndex(SoTextureCoordinateBindingElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoTextureCoordinateBindingElement::~SoTextureCoordinateBindingElement(void)
{
}

//! FIXME: write doc.

void
SoTextureCoordinateBindingElement::set(SoState * const state,
                                       SoNode * const node,
                                       const Binding binding)
{
  assert(binding == PER_VERTEX || binding == PER_VERTEX_INDEXED);
  SoInt32Element::set(classStackIndex, state, node, binding);
}

//! FIXME: write doc.

void
SoTextureCoordinateBindingElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoTextureCoordinateBindingElement::getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoTextureCoordinateBindingElement::set(SoState * const state,
                                       const Binding binding)
{
  set(state, NULL, binding);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoTextureCoordinateBindingElement::Binding
SoTextureCoordinateBindingElement::get(SoState * const state)
{
  return (Binding)SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoTextureCoordinateBindingElement::Binding
SoTextureCoordinateBindingElement::getDefault()
{
  return PER_VERTEX_INDEXED;
}
