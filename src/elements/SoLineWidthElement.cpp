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
  \class SoLineWidthElement Inventor/elements/SoLineWidthElement.h
  \brief The SoLineWidthElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoLineWidthElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoLineWidthElement);

/*!
  This static method initializes static data for the
  SoLineWidthElement class.
*/

void
SoLineWidthElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoLineWidthElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoLineWidthElement::createInstance(void)
*/

SoLineWidthElement::SoLineWidthElement(void)
{
    setTypeId(SoLineWidthElement::classTypeId);
    setStackIndex(SoLineWidthElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoLineWidthElement::~SoLineWidthElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoLineWidthElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoLineWidthElement::getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoLineWidthElement::set(SoState * const state, SoNode * const node,
                        const float lineWidth)
{
  SoFloatElement::set(classStackIndex,state, node, lineWidth);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoLineWidthElement::set(SoState * const state, const float lineWidth)
{
  set(state, NULL, lineWidth);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoLineWidthElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoLineWidthElement::getDefault()
{
  return 0.0f;
}
