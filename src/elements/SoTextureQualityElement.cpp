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
  \class SoTextureQualityElement Inventor/elements/SoTextureQualityElement.h
  \brief The SoTextureQualityElement is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/SbName.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoTextureQualityElement);

/*!
  This static method initializes static data for the
  SoTextureQualityElement class.
*/

void
SoTextureQualityElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureQualityElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoTextureQualityElement::createInstance(void)
*/

SoTextureQualityElement::SoTextureQualityElement(void)
{
  setTypeId(SoTextureQualityElement::classTypeId);
  setStackIndex(SoTextureQualityElement::classStackIndex);
  this->data = getDefault();
}

/*!
  The destructor.
*/

SoTextureQualityElement::~SoTextureQualityElement()
{
}

//! FIXME: write doc.

void
SoTextureQualityElement::init(SoState * state)
{
  inherited::init(state);
  this->data = SoTextureQualityElement::getDefault();
}

/*!
  FIXME: write doc.
*/
void
SoTextureQualityElement::set(SoState * const state, SoNode * const node,
                             const float quality)
{
  SoFloatElement::set(classStackIndex, state, node, quality);
}

/*!
  FIXME: write doc.
*/
void
SoTextureQualityElement::set(SoState * const state, const float quality)
{
  set(state, NULL, quality);
}

/*!
  FIXME: write doc.
*/
float
SoTextureQualityElement::get(SoState * const state)
{
  return SoFloatElement::get(classStackIndex, state);
}

/*!
  FIXME: write doc.
*/
float
SoTextureQualityElement::getDefault(void)
{
  return 0.5f;
}
