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
  \class SoComplexityTypeElement Inventor/elements/SoComplexityTypeElement.h
  \brief The SoComplexityTypeElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoComplexityTypeElement.h>

#include <Inventor/elements/SoShapeStyleElement.h>

#include <cassert>

#include "SbBasicP.h"

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
  return static_cast<Type>(
                        SoInt32Element::get(classStackIndex, state)
                        );
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoComplexityTypeElement::Type
SoComplexityTypeElement::getDefault()
{
  return OBJECT_SPACE;
}
