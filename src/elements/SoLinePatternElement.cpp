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
  \class SoLinePatternElement Inventor/elements/SoLinePatternElement.h
  \brief The SoLinePatternElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoLinePatternElement.h>


#include <cassert>

SO_ELEMENT_SOURCE(SoLinePatternElement);

/*!
  This static method initializes static data for the
  SoLinePatternElement class.
*/

void
SoLinePatternElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLinePatternElement, inherited);
}

/*!
  The destructor.
*/

SoLinePatternElement::~SoLinePatternElement(void)
{
}

//! FIXME: write doc.

void
SoLinePatternElement::set(SoState * const state,
                          SoNode * const node,
                          const int32_t pattern)
{
    SoInt32Element::set(classStackIndex, state, node, pattern);
}

//! FIXME: write doc.

void
SoLinePatternElement::init(SoState * state)
{
  inherited::init(state);

  this->data = SoLinePatternElement::getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoLinePatternElement::set(SoState * const state, const int32_t pattern)
{
  set(state, NULL, pattern);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoLinePatternElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int32_t
SoLinePatternElement::getDefault()
{
  return CONTINUOUS;
}
