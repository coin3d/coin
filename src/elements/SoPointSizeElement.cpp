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
  \class SoPointSizeElement Inventor/elements/SoPointSizeElement.h
  \brief The SoPointSizeElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoPointSizeElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoPointSizeElement);

/*!
  This static method initializes static data for the SoPointSizeElement class.
*/

void
SoPointSizeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPointSizeElement, inherited);
}

/*!
  The destructor.
*/

SoPointSizeElement::~SoPointSizeElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoPointSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

void
SoPointSizeElement::set(SoState * const state,
                        SoNode * const node,
                        const float pointSize)
{
  SoFloatElement::set(classStackIndex,state, node, pointSize);
}

//! FIXME: write doc.

void
SoPointSizeElement::set(SoState * const state, const float pointSize)
{
  set(state, NULL, pointSize);
}

//! FIXME: write doc.

float
SoPointSizeElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

float
SoPointSizeElement::getDefault()
{
  return 0.0f;
}
