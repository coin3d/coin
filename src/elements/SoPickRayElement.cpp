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
  \class SoPickRayElement Inventor/elements/SoPickRayElement.h
  \brief The SoPickRayElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoPickRayElement.h>

#include <coindefs.h> // COIN_STUB()

#include <assert.h>

/*!
  \fn SoPickRayElement::volume

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoPickRayElement);

/*!
  This static method initializes static data for the SoPickRayElement class.
*/

void
SoPickRayElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPickRayElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoPickRayElement::createInstance(void)
*/

SoPickRayElement::SoPickRayElement(void)
  : volume()
{
    setTypeId(SoPickRayElement::classTypeId);
    setStackIndex(SoPickRayElement::classStackIndex);
}

/*!
  The destructor.
*/

SoPickRayElement::~SoPickRayElement(void)
{
}

//! FIXME: write doc.

void
SoPickRayElement::init(SoState * state)
{
    inherited::init(state);
}

//! FIXME: write doc.

SbBool
SoPickRayElement::matches(const SoElement * /* element */) const
{
  // should always return false; not part of cache consideration
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoPickRayElement::copyMatchInfo() const
{
  assert(getTypeId().canCreateInstance());
  SoPickRayElement *element =
    (SoPickRayElement *)(getTypeId().createInstance());
  // no use copying any data. matches() will always return false.
  return (SoElement*)element;
}

//! FIXME: write doc.

void
SoPickRayElement::set(SoState * const state,
                      const SbViewVolume & volume)
{
  SoPickRayElement *elem = (SoPickRayElement*)
    SoElement::getElement(state, classStackIndex);
  elem->volume = volume;
}

//! FIXME: write doc.

const SbViewVolume &
SoPickRayElement::get(SoState * const state)
{
  SoPickRayElement *elem = (SoPickRayElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->volume;
}

//! FIXME: write doc.

void
SoPickRayElement::print(FILE * /* file */) const
{
  COIN_STUB();
}
