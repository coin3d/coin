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
  \class SoDecimationTypeElement Inventor/elements/SoDecimationTypeElement.h
  \brief The SoDecimationTypeElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationTypeElement.h>
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
