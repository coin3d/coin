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
  \class SoDecimationPercentageElement Inventor/elements/SoDecimationPercentageElement.h
  \brief The SoDecimationPercentageElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoDecimationPercentageElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoDecimationPercentageElement);

/*!
  This static method initializes static data for the
  SoDecimationPercentageElement class.
*/

void
SoDecimationPercentageElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDecimationPercentageElement, inherited);
}

/*!
  The destructor.
*/

SoDecimationPercentageElement::~SoDecimationPercentageElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoDecimationPercentageElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoDecimationPercentageElement::set(SoState * const state, SoNode * const node,
                          const float complexity)
{
  SoFloatElement::set(classStackIndex,state,node, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoDecimationPercentageElement::set(SoState * const state, const float complexity)
{
  set(state, NULL, complexity);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoDecimationPercentageElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoDecimationPercentageElement::getDefault()
{
  return 1.0f;
}
