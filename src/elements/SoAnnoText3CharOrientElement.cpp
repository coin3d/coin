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
  \class SoAnnoText3CharOrientElement Inventor/elements/SoAnnoText3CharOrientElement.h
  \brief The SoAnnoText3CharOrientElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoAnnoText3CharOrientElement.h>


#include <assert.h>

SO_ELEMENT_SOURCE(SoAnnoText3CharOrientElement);

/*!
  This static method initializes static data for the
  SoAnnoText3CharOrientElement class.
*/

void
SoAnnoText3CharOrientElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoAnnoText3CharOrientElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoAnnoText3CharOrientElement::createInstance()
*/

SoAnnoText3CharOrientElement::SoAnnoText3CharOrientElement(void)
{
    setTypeId(SoAnnoText3CharOrientElement::classTypeId);
    setStackIndex(SoAnnoText3CharOrientElement::classStackIndex);
}

/*!
  The destructor.
*/

SoAnnoText3CharOrientElement::~SoAnnoText3CharOrientElement(// virtual protected
    void)
{
}

//! FIXME: write doc.

void
SoAnnoText3CharOrientElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void
SoAnnoText3CharOrientElement::set(SoState * const state, SbBool isOriented)
{
  inherited::set(classStackIndex, state, isOriented);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::get(SoState * state)
{
  return (SbBool) SoInt32Element::get(classStackIndex, state);
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool
SoAnnoText3CharOrientElement::getDefault(void)
{
  return TRUE;
}
