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
  \class SoAnnoText3FontSizeHintElement Inventor/elements/SoAnnoText3FontSizeHintElement.h
  \brief The SoAnnoText3FontSizeHintElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3FontSizeHintElement.h>


#include <assert.h>

/*!
  \fn SoAnnoText3FontSizeHintElement::FontSizeHint

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoAnnoText3FontSizeHintElement);

/*!
  This static method initializes static data for the
  SoAnnoText3FontSizeHintElement class.
*/

void
SoAnnoText3FontSizeHintElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAnnoText3FontSizeHintElement, inherited);
}

/*!
  The destructor.
*/

SoAnnoText3FontSizeHintElement::~SoAnnoText3FontSizeHintElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoAnnoText3FontSizeHintElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3FontSizeHintElement::set(SoState * const state, SoNode * const node,
                                const FontSizeHint hint)
{
  SoInt32Element::set(classStackIndex,state,node,hint);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3FontSizeHintElement::set(SoState * const state, const FontSizeHint hint)
{
  set(state, NULL, hint);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3FontSizeHintElement::FontSizeHint
SoAnnoText3FontSizeHintElement::get(SoState * const state)
{
  return (SoAnnoText3FontSizeHintElement::FontSizeHint)
    SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SoAnnoText3FontSizeHintElement::FontSizeHint
SoAnnoText3FontSizeHintElement::getDefault()
{
  return FIT_TEXT_VECTOR;
}
