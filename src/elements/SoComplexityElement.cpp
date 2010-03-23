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
  \class SoComplexityElement Inventor/elements/SoComplexityElement.h
  \brief The SoComplexityElement class contains the current shape complexity for a graph traverser.
  \ingroup elements
*/

#include <Inventor/elements/SoComplexityElement.h>


#include <cassert>


SO_ELEMENT_SOURCE(SoComplexityElement);


/*!
  This static method initializes static data for the SoComplexityElement
  class.
*/

void
SoComplexityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoComplexityElement, inherited);
}

/*!
  The destructor.
*/

SoComplexityElement::~SoComplexityElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoComplexityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoComplexityElement::set(SoState * const state, SoNode * const node,
                         const float complexity)
{
  SoFloatElement::set(classStackIndex, state, node, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoComplexityElement::set(SoState * const state, const float complexity)
{
  set(state, NULL, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoComplexityElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoComplexityElement::getDefault()
{
  return 0.5f;
}
