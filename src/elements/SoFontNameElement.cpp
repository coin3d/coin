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
  \class SoFontNameElement Inventor/elements/SoFontNameElement.h
  \brief The SoFontNameElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFontNameElement.h>


#include <assert.h>


SbName * SoFontNameElement::defaultfontname = NULL;

/*!
  \fn SoFontNameElement::fontName

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoFontNameElement);

/*!
  This static method initializes static data for the SoFontNameElement
  class.
*/

void
SoFontNameElement::initClass(void)
{
#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage.
  (void)atexit(SoFontNameElement::clean);
#endif // COIN_DEBUG

  SO_ELEMENT_INIT_CLASS(SoFontNameElement, inherited);

  SoFontNameElement::defaultfontname = new SbName("defaultFont");
}

void
SoFontNameElement::clean(void)
{
#if COIN_DEBUG
  delete SoFontNameElement::defaultfontname;
#endif // COIN_DEBUG
}

/*!
  The destructor.
*/

SoFontNameElement::~SoFontNameElement()
{
}

//! FIXME: write doc.

void
SoFontNameElement::set(SoState * const state,
                       SoNode * const node,
                       const SbName fontName)
{
  SoFontNameElement * element = (SoFontNameElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->fontName = fontName;
}

//! FIXME: write doc.

const SbName &
SoFontNameElement::get(SoState * const state)
{
  SoFontNameElement * element = (SoFontNameElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->fontName;
}

//! FIXME: write doc.

SbBool
SoFontNameElement::matches(const SoElement * element) const
{
  if ((SoElement *)this == element)
    return TRUE;
  if (element->getTypeId() != SoFontNameElement::getClassTypeId())
    return FALSE;
  if (this->fontName != ((SoFontNameElement *)element)->fontName)
    return FALSE;
  return TRUE;
}

//! FIXME: write doc.

SoElement *
SoFontNameElement::copyMatchInfo(void) const
{
  SoFontNameElement * element = (SoFontNameElement *)
    (SoFontNameElement::getClassTypeId().createInstance());
  element->fontName = this->fontName;
  element->nodeId = this->nodeId;
  return element;
}

//! FIXME: write doc.

void
SoFontNameElement::print(FILE * file) const
{
  fprintf(file, "SoFontNameElement[%p]: font = %s\n", this,
           this->fontName.getString());
}

//! FIXME: write doc.

void
SoFontNameElement::init(SoState * state)
{
  inherited::init(state);
  this->fontName = *SoFontNameElement::defaultfontname;
}

//! FIXME: write doc.

SbName
SoFontNameElement::getDefault(void)
{
  return *SoFontNameElement::defaultfontname;
}
