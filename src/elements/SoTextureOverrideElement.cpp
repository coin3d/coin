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
  \class SoTextureOverrideElement Inventor/elements/SoTextureOverrideElement.h
  \brief The SoTextureOverrideElement makes it possible to override texture elements.
  \ingroup elements
*/


#include <Inventor/elements/SoTextureOverrideElement.h>
#include <assert.h>

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
  return ((SoTextureOverrideElement*)element)->flags == this->flags;
}

//!

SoElement *
SoTextureOverrideElement::copyMatchInfo() const
{
  SoTextureOverrideElement *elem =
    (SoTextureOverrideElement*) this->getTypeId().createInstance();
  elem->flags = this->flags;
  return elem;
}

//!

void
SoTextureOverrideElement::init(SoState *state)
{
  this->flags = 0;
}

//!

void
SoTextureOverrideElement::push(SoState *state)
{
  inherited::push(state);
  SoTextureOverrideElement * prev = (SoTextureOverrideElement*) this->getNextInStack();
  this->flags = prev->flags;
}

//!

SbBool
SoTextureOverrideElement::getQualityOverride(SoState *state)
{
  const SoTextureOverrideElement * const element =
    (const SoTextureOverrideElement *) getConstElement(state, classStackIndex);
  return (element->flags & TEXTURE_QUALITY) != 0;
}

//!

SbBool
SoTextureOverrideElement::getImageOverride(SoState *state)
{
  const SoTextureOverrideElement * const element =
    (const SoTextureOverrideElement *) getConstElement(state, classStackIndex);
  return (element->flags & TEXTURE_IMAGE) != 0;
}

//!

void
SoTextureOverrideElement::setQualityOverride(SoState *state, const SbBool value)
{
  SoTextureOverrideElement * const element =
    (SoTextureOverrideElement *) getElement(state, classStackIndex);
  if (value)
    element->flags |= TEXTURE_QUALITY;
  else
    element->flags &= ~TEXTURE_QUALITY;
}

//!

void
SoTextureOverrideElement::setImageOverride(SoState *state, const SbBool value)
{
  SoTextureOverrideElement * const element =
    (SoTextureOverrideElement *) getElement(state, classStackIndex);
  if (value)
    element->flags |= TEXTURE_IMAGE;
  else
    element->flags &= ~TEXTURE_IMAGE;
}

//!
void
SoTextureOverrideElement::print(FILE *fp) const
{
  fprintf(fp, "SoTextureOverrideElement::flags: 0x%x\n", this->flags);
}
