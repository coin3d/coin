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
  \class SoFontSizeElement Inventor/elements/SoFontSizeElement.h
  \brief The SoFontSizeElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFontSizeElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoFontSizeElement);

/*!
  This static method initializes static data for the SoFontSizeElement
  class.
*/

void
SoFontSizeElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoFontSizeElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoFontSizeElement::createInstance(void)
*/

SoFontSizeElement::SoFontSizeElement(void)
{
    setTypeId(SoFontSizeElement::classTypeId);
    setStackIndex(SoFontSizeElement::classStackIndex);
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoFontSizeElement::~SoFontSizeElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoFontSizeElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoFontSizeElement::set(SoState * const state, SoNode * const node,
                        const float fontSize)
{
  SoFloatElement::set(classStackIndex, state, node, fontSize);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoFontSizeElement::set(SoState * const state, const float fontSize)
{
  set(state, NULL, fontSize);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoFontSizeElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
float
SoFontSizeElement::getDefault()
{
  return 10.0f;
}
