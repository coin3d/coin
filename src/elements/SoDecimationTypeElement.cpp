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
  \class SoDecimationTypeElement Inventor/elements/SoDecimationTypeElement.h
  \brief The SoDecimationTypeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationTypeElement.h>

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa void * SoDecimationTypeElement::createInstance(void)
*/

SoDecimationTypeElement::SoDecimationTypeElement(void)
{
    setTypeId(SoDecimationTypeElement::classTypeId);
    setStackIndex(SoDecimationTypeElement::classStackIndex);
    this->data = getDefault();
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
