/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoFontSizeElement Inventor/elements/SoFontSizeElement.h
  \brief The SoFontSizeElement class is yet to be documented.
  \ingroup elements

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
