/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoPickRayElement Inventor/elements/SoPickRayElement.h
  \brief The SoPickRayElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoPickRayElement.h>

#include "SbBasicP.h"

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
  SoPickRayElement * element =
    static_cast<SoPickRayElement *>(getTypeId().createInstance());
  // no use copying any data. matches() will always return false.
  return element;
}

//! FIXME: write doc.

void
SoPickRayElement::set(SoState * const state,
                      const SbViewVolume & volume)
{
  SoPickRayElement * elem = coin_safe_cast<SoPickRayElement *>
    (
     SoElement::getElement(state, classStackIndex)
     );
  if (elem) {
    elem->volume = volume;
  }
}

//! FIXME: write doc.

const SbViewVolume &
SoPickRayElement::get(SoState * const state)
{
  const SoPickRayElement * elem = coin_assert_cast<const SoPickRayElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->volume;
}

//! FIXME: write doc.
