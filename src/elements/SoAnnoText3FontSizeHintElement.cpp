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
