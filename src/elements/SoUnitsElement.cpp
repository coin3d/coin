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
  \class SoUnitsElement Inventor/elements/SoUnitsElement.h
  \brief The SoUnitsElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoUnitsElement.h>


#include <assert.h>

/*!
  \fn SoUnitsElement::Units

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoUnitsElement);

/*!
  This static method initializes static data for the SoUnitsElement class.
*/

void
SoUnitsElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoUnitsElement, inherited);
}

/*!
  The destructor.
*/

SoUnitsElement::~SoUnitsElement(void)
{
}

//! FIXME: write doc.

void
SoUnitsElement::set(SoState * const state,
                    SoNode * const node,
                    const Units unit)
{
  assert((int)unit >= (int)METERS && (int)unit <= (int)NAUTICAL_MILES);
  SoInt32Element::set(classStackIndex, state, node, unit);
}

//! FIXME: write doc.

void
SoUnitsElement::init(SoState * state)
{
    inherited::init(state);
    this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoUnitsElement::set(SoState * const state, const Units units)
{
  set(state, NULL, units);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoUnitsElement::Units
SoUnitsElement::get(SoState * const state)
{
  return (Units) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoUnitsElement::Units
SoUnitsElement::getDefault()
{
  return METERS;
}
