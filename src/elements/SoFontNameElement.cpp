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
  \class SoFontNameElement Inventor/elements/SoFontNameElement.h
  \brief The SoFontNameElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoFontNameElement.h>

#include <Inventor/SbName.h>

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
  A constructor.  Can't be used directly.

  \sa void * SoFontNameElement::createInstance(void)
*/

SoFontNameElement::SoFontNameElement(void)
  : fontName(*defaultfontname)
{
  setTypeId(SoFontNameElement::classTypeId);
  setStackIndex(SoFontNameElement::classStackIndex);
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

const SbName
SoFontNameElement::get(SoState * const state)
{
  SoFontNameElement * element = (SoFontNameElement *)
    SoElement::getConstElement(state, classStackIndex);
  return element->fontName;
}

//! FIXME: write doc.

SbBool
SoFontNameElement::matches(SoElement * element) const
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

const SbName
SoFontNameElement::getDefault()
{
  return *SoFontNameElement::defaultfontname;
}
