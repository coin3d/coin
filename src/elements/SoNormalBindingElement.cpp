/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoNormalBindingElement Inventor/elements/SoNormalBindingElement.h
  \brief The SoNormalBindingElement is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoNormalBindingElement.h>

#include <assert.h>

/*!
  \fn SoNormalBindingElement::Binding

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoNormalBindingElement);

/*!
  This static method initializes static methods for the
  SoNormalBindingElement class.
*/

void
SoNormalBindingElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoNormalBindingElement, inherited);
}

/*!
  The destructor.
*/

SoNormalBindingElement::~SoNormalBindingElement(void)
{
}

//! FIXME: write doc.

void
SoNormalBindingElement::set(SoState * const state,
                            SoNode * const node,
                            const Binding binding)
{
  assert((int)binding >= OVERALL && (int)binding <= PER_VERTEX_INDEXED);
  SoInt32Element::set(classStackIndex, state, node, binding);
}

//! FIXME: write doc.

void
SoNormalBindingElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoNormalBindingElement::set(SoState * const state, const Binding binding)
{
  set(state, NULL, binding);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoNormalBindingElement::Binding
SoNormalBindingElement::get(SoState * const state)
{
  return (Binding) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoNormalBindingElement::Binding
SoNormalBindingElement::getDefault()
{
  return DEFAULT;
}
