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
  A constructor.  Can't be used directly.

  \sa void * SoDecimationPercentageElement::createInstance(void)
*/

SoDecimationPercentageElement::SoDecimationPercentageElement(void)
{
    setTypeId(SoDecimationPercentageElement::classTypeId);
    setStackIndex(SoDecimationPercentageElement::classStackIndex);
    this->data = getDefault();
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
