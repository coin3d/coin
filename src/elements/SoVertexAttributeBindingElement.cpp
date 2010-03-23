/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVertexAttributeBindingElement Inventor/elements/SoVertexAttributeBindingElement.h
  \brief The SoVertexAttributeBindingElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoVertexAttributeBindingElement.h>


#include <cassert>

/*!
  \fn SoVertexAttributeBindingElement::Binding

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoVertexAttributeBindingElement);

/*!
  This static method initializes static data for the
  SoVertexAttributeBindingElement class.
*/

void
SoVertexAttributeBindingElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoVertexAttributeBindingElement, inherited);
}

/*!
  The destructor.
*/

SoVertexAttributeBindingElement::~SoVertexAttributeBindingElement(void)
{
}

//! FIXME: write doc.

void
SoVertexAttributeBindingElement::set(SoState * const state,
                              SoNode * const node,
                              const Binding binding)
{
  assert(static_cast<int>(binding) >= static_cast<int>(OVERALL) &&
         static_cast<int>(binding) <= static_cast<int>(PER_VERTEX_INDEXED)
        );
  SoInt32Element::set(classStackIndex, state, node, binding);
}

//! FIXME: write doc.

void
SoVertexAttributeBindingElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoVertexAttributeBindingElement::set(SoState * const state, const Binding binding)
{
  set(state, NULL, binding);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoVertexAttributeBindingElement::Binding
SoVertexAttributeBindingElement::get(SoState * const state)
{
  return static_cast<Binding>(SoInt32Element::get(classStackIndex, state));
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoVertexAttributeBindingElement::Binding
SoVertexAttributeBindingElement::getDefault()
{
  return DEFAULT;
}
