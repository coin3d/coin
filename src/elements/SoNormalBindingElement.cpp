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
  \class SoNormalBindingElement Inventor/elements/SoNormalBindingElement.h
  \brief The SoNormalBindingElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoNormalBindingElement.h>

#include <Inventor/SbName.h>
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
  A constructor.  Can't be used directly.

  \sa void * SoNormalBindingElement::createInstance(void)
*/

SoNormalBindingElement::SoNormalBindingElement(void)
{
    setTypeId(SoNormalBindingElement::classTypeId);
    setStackIndex(SoNormalBindingElement::classStackIndex);
    this->data = getDefault();
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
