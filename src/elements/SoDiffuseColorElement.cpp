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
  \class SoDiffuseColorElement Inventor/elements/SoDiffuseColorElement.h
  \brief The SoDiffuseColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/SbColor.h>
#include <Inventor/errors/SoDebugError.h>
#include <cassert>
#include <cstdlib>

#include "SbBasicP.h"

SO_ELEMENT_SOURCE(SoDiffuseColorElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoDiffuseColorElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoDiffuseColorElement, inherited);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::init(SoState * stateptr)
{
  inherited::init(stateptr);
  this->state = stateptr;
}

/*!
  The destructor.
*/

SoDiffuseColorElement::~SoDiffuseColorElement()
{
}

//! FIXME: write doc.

void
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numcolors,
                           const SbColor * const colors)
{
  SoDiffuseColorElement * elem = const_cast<SoDiffuseColorElement *>
    (
     SoDiffuseColorElement::getInstance(state)
     );

  SoLazyElement::setDiffuse(state, node, numcolors, colors, &elem->colorpacker);
}

//! FIXME: write doc.

void
SoDiffuseColorElement::set(SoState * const state, SoNode * const node,
                           const int32_t numcolors,
                           const uint32_t * const colors,
                           const SbBool packedtransparency)
{
  SoLazyElement::setPacked(state, node, numcolors, colors, packedtransparency);
}

SbBool
SoDiffuseColorElement::isPacked() const
{
  SoLazyElement * lazy = SoLazyElement::getInstance(this->state);
  return lazy->isPacked();
}

//! FIXME: write doc.

int32_t
SoDiffuseColorElement::getNum(void) const
{
  SoLazyElement * lazy = SoLazyElement::getInstance(this->state);
  return lazy->getNumDiffuse();
}

/*!
  Returns the color array. Don't use this unless
  SoDiffuseColorElement::isPacked() returns \c FALSE.

  This method is not part of the original SGI Open Inventor v2.1 API.

  \since Coin 1.0
*/
const SbColor *
SoDiffuseColorElement::getColorArrayPtr(void) const
{
  SoLazyElement * lazy = SoLazyElement::getInstance(this->state);

#if COIN_DEBUG
  if (lazy->isPacked()) {
    SoDebugError::postWarning("SoDiffuseColorElement::getColorArrayPtr",
                              "colors are packed -- use getPackedArrayPtr() "
                              "instead");
  }
#endif // COIN_DEBUG
  return lazy->getDiffusePointer();
}

/*!
  Returns the packed color array. Don't use this unless
  SoDiffuseColorElement::isPacked() returns \c TRUE.

  This method is not part of the original SGI Open Inventor v2.1 API.

  \since Coin 1.0
*/
const uint32_t *
SoDiffuseColorElement::getPackedArrayPtr(void) const
{
  SoLazyElement * lazy = SoLazyElement::getInstance(this->state);

#if COIN_DEBUG
  if (!lazy->isPacked()) {
    SoDebugError::postWarning("SoDiffuseColorElement::getPackedArrayPtr",
                              "colors are *not* packed -- use "
                              "getColorArrayPtr() instead");
  }
#endif // COIN_DEBUG
  return lazy->getPackedPointer();
}

//! FIXME: write doc.

const SbColor &
SoDiffuseColorElement::get(const int index) const
{
  return SoLazyElement::getDiffuse(this->state, index);
}

SbBool
SoDiffuseColorElement::hasPackedTransparency(void) const
{
  return SoLazyElement::getInstance(this->state)->isTransparent();
}

//! FIXME: write doc.

const SoDiffuseColorElement *
SoDiffuseColorElement::getInstance(SoState *state)
{
  return coin_assert_cast<const SoDiffuseColorElement *>
    (
    state->getElementNoPush(classStackIndex)
    );
}
