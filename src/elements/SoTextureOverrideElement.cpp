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
  \class SoTextureOverrideElement Inventor/elements/SoTextureOverrideElement.h
  \brief The SoTextureOverrideElement makes it possible to override texture elements.
  \ingroup elements
*/


#include <Inventor/elements/SoTextureOverrideElement.h>

#include "coindefs.h"
#include "SbBasicP.h"

#include <cassert>

SO_ELEMENT_SOURCE(SoTextureOverrideElement);

/*!
This static method initializes static data for the
SoTextureOverrideElement class.
*/

void
SoTextureOverrideElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureOverrideElement, inherited);
}

/*!
  The destructor.
*/

SoTextureOverrideElement::~SoTextureOverrideElement(void)
{
}

//!

SbBool
SoTextureOverrideElement::matches(const SoElement *element) const
{
  return coin_assert_cast<const SoTextureOverrideElement *>(element)->flags == this->flags;
}

//!

SoElement *
SoTextureOverrideElement::copyMatchInfo() const
{
  SoTextureOverrideElement * elem =
    static_cast<SoTextureOverrideElement *>(this->getTypeId().createInstance());
  elem->flags = this->flags;
  return elem;
}

//!

void
SoTextureOverrideElement::init(SoState * COIN_UNUSED_ARG(state))
{
  this->flags = 0;
}

//!

void
SoTextureOverrideElement::push(SoState *state)
{
  inherited::push(state);
  const SoTextureOverrideElement * prev =
    coin_assert_cast<SoTextureOverrideElement *>
    (
     this->getNextInStack()
     );
  this->flags = prev->flags;
}

//!

SbBool
SoTextureOverrideElement::getQualityOverride(SoState *state)
{
  const SoTextureOverrideElement * const element =
    coin_assert_cast<const SoTextureOverrideElement *>
    (
     getConstElement(state, classStackIndex)
     );
  return (element->flags & TEXTURE_QUALITY) != 0;
}

//!

SbBool
SoTextureOverrideElement::getImageOverride(SoState *state)
{
  const SoTextureOverrideElement * const element =
    coin_assert_cast<const SoTextureOverrideElement *>
    (
     getConstElement(state, classStackIndex)
     );
  return (element->flags & TEXTURE_IMAGE) != 0;
}

SbBool
SoTextureOverrideElement::getBumpMapOverride(SoState *state)
{
  const SoTextureOverrideElement * const element =
    coin_assert_cast<const SoTextureOverrideElement *>
    (
     getConstElement(state, classStackIndex)
     );
  return (element->flags & BUMP_MAP) != 0;
}

//!

void
SoTextureOverrideElement::setQualityOverride(SoState *state, const SbBool value)
{
  SoTextureOverrideElement * const element =
    coin_safe_cast<SoTextureOverrideElement *>
    (
     getElement(state, classStackIndex)
     );
  if (element) {
    if (value)
      element->flags |= TEXTURE_QUALITY;
    else
      element->flags &= ~TEXTURE_QUALITY;
  }
}

//!

void
SoTextureOverrideElement::setImageOverride(SoState *state, const SbBool value)
{
  SoTextureOverrideElement * const element =
    coin_safe_cast<SoTextureOverrideElement *>(getElement(state, classStackIndex));
  if (element) {
    if (value)
      element->flags |= TEXTURE_IMAGE;
    else
      element->flags &= ~TEXTURE_IMAGE;
  }
}

//!

void
SoTextureOverrideElement::setBumpMapOverride(SoState *state, const SbBool value)
{
  SoTextureOverrideElement * const element =
    coin_safe_cast<SoTextureOverrideElement *>(getElement(state, classStackIndex));
  if (element) {
    if (value)
      element->flags |= BUMP_MAP;
    else
      element->flags &= ~BUMP_MAP;
  }
}

//!
void
SoTextureOverrideElement::print(FILE *fp) const
{
  fprintf(fp, "SoTextureOverrideElement::flags: 0x%x\n", this->flags);
}
