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
  \class SoTextureOverrideElement Inventor/elements/SoTextureOverrideElement.h
  \brief The SoTextureOverrideElement makes it possible to override texture elements.

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
  SoTextureOverrideElement *next = (SoTextureOverrideElement*) this->next;
  next->flags = this->flags;
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
