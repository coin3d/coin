/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoPointSizeElement Inventor/elements/SoPointSizeElement.h
  \brief The SoPointSizeElement class is yet to be documented.

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
  A constructor.  Can't be used directly.

  \sa SoPointSizeElement::createInstance(void)
*/

SoPointSizeElement::SoPointSizeElement(void)
{
    setTypeId(SoPointSizeElement::classTypeId);
    setStackIndex(SoPointSizeElement::classStackIndex);
    this->data = getDefault();
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
