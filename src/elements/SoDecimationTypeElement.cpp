/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoDecimationTypeElement Inventor/elements/SoDecimationTypeElement.h
  \brief The SoDecimationTypeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationTypeElement.h>

#include <coindefs.h> // COIN_STUB()

#include <assert.h>

/*!
  \fn SoDecimationTypeElement::Type

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoDecimationTypeElement);

/*!
  This static method initializes static data for the
  SoDecimationTypeElement class.
*/

void
SoDecimationTypeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDecimationTypeElement, inherited);
}

/*!
  The destructor.
*/

SoDecimationTypeElement::~SoDecimationTypeElement(void)
{
}

//! FIXME: write doc.

void
SoDecimationTypeElement::set(SoState * const state,
                             SoNode * const node,
                             const Type type)
{
  assert((int)type >= (int)AUTOMATIC && (int)type <= (int)PERCENTAGE);
  SoInt32Element::set(classStackIndex, state, node, type);
}

//! FIXME: write doc.

void
SoDecimationTypeElement::init(SoState * state)
{
    inherited::init(state);
    this->data = getDefault();
}

//! FIXME: write doc.

void
SoDecimationTypeElement::print(FILE * /* fp */) const
{
  COIN_STUB();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoDecimationTypeElement::set(SoState * const state, const Type type)
{
  set(state, NULL, type);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDecimationTypeElement::Type
SoDecimationTypeElement::get(SoState * const state)
{
  return (Type) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoDecimationTypeElement::Type
SoDecimationTypeElement::getDefault()
{
  return AUTOMATIC;
}
