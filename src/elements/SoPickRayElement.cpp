/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
