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
  \class SoFocalDistanceElement Inventor/elements/SoFocalDistanceElement.h
  \brief The SoFocalDistanceElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFocalDistanceElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoFocalDistanceElement);

/*!
  This static method initializes static data for the
  SoFocalDistanceElement class.
*/

void
SoFocalDistanceElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoFocalDistanceElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoFocalDistanceElement::createInstance(void)
*/

SoFocalDistanceElement::SoFocalDistanceElement(void)
{
    setTypeId(SoFocalDistanceElement::classTypeId);
    setStackIndex(SoFocalDistanceElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoFocalDistanceElement::~SoFocalDistanceElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoFocalDistanceElement::init(SoState * state)
{
    inherited::init(state);
    this->data = getDefault();
}

//! FIXME: write doc.

void
SoFocalDistanceElement::set(SoState * const state, SoNode * const node,
                            const float distance)
{
  SoFloatElement::set(classStackIndex, state, node, distance);
}

//! FIXME: write doc.

void
SoFocalDistanceElement::set(SoState * const state, const float distance)
{
  set(state, NULL, distance);
}

//! FIXME: write doc.

float
SoFocalDistanceElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

float
SoFocalDistanceElement::getDefault()
{
  return 5.0f;
}
