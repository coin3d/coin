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
  \class SoComplexityTypeElement Inventor/elements/SoComplexityTypeElement.h
  \brief The SoComplexityTypeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoComplexityTypeElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>


#include <assert.h>

/*!
  \fn SoComplexityTypeElement::Type

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoComplexityTypeElement);

/*!
  This static method initializes static data for the SoComplexityTypeElement
  class.
*/

void
SoComplexityTypeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoComplexityTypeElement, inherited);
}

/*!
  The destructor.
*/

SoComplexityTypeElement::~SoComplexityTypeElement(void)
{
}

//! FIXME: write doc.

void
SoComplexityTypeElement::set(SoState * const state,
                             SoNode * const node,
                             const Type type)
{
  assert(type == OBJECT_SPACE || type == SCREEN_SPACE || type == BOUNDING_BOX);

  SoShapeStyleElement::setComplexityType(state, type);

  SoInt32Element::set(classStackIndex, state, node, type);
}

//! FIXME: write doc.

void
SoComplexityTypeElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoComplexityTypeElement::set(SoState * const state, const Type type)
{
  set(state, NULL, type);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoComplexityTypeElement::Type
SoComplexityTypeElement::get(SoState * const state)
{
  return (Type)SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoComplexityTypeElement::Type
SoComplexityTypeElement::getDefault()
{
  return OBJECT_SPACE;
}
