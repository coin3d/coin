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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTextureImageElement Inventor/elements/SoTextureImageElement.h
  \brief The SoTextureImageElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureImageElement.h>


#include <assert.h>

/*!
  \fn SoTextureImageElement::Model

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::Wrap

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::size

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::numComponents

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::bytes

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::wrapS

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::wrapT

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::model

  FIXME: write doc.
*/

/*!
  \fn SoTextureImageElement::blendColor

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoTextureImageElement);

/*!
  This static method initializes static data for the
  SoTextureImageElement class.
*/

void
SoTextureImageElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoTextureImageElement, inherited);
}

/*!
  The destructor.
*/

SoTextureImageElement::~SoTextureImageElement()
{
}

//! FIXME: write doc.

void
SoTextureImageElement::init(SoState * state)
{
  inherited::init(state);
  this->setDefaultValues();
}

//
// private
//
void
SoTextureImageElement::setDefaultValues()
{
  this->bytes = getDefault(this->size, this->numComponents);
  this->wrapS = REPEAT;
  this->wrapT = REPEAT;
  this->model = MODULATE;
  this->blendColor.setValue(0.0f, 0.0f, 0.0f);
}

/*!
  Resets this element to its original values.
*/
void
SoTextureImageElement::setDefault(SoState * const state, SoNode * const node)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setDefaultValues();
}

//! FIXME: write doc.

void
SoTextureImageElement::set(SoState * const state, SoNode * const node,
                           const SbVec2s & size, const int numComponents,
                           const unsigned char * bytes,
                           const Wrap wrapS, const Wrap wrapT,
                           const Model model, const SbColor & blendColor)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);

  elem->setElt(size, numComponents, bytes, wrapS, wrapT,
               model, blendColor);
}

/*!
  FIXME: write doc.
*/

void SoTextureImageElement::set(SoState * const state, SoNode * const node,
                                const SbVec2s & size, const int numComponents,
                                const unsigned char * bytes, const int wrapS,
                                const int wrapT, const int model,
                                const SbColor & blendColor)
{
  SoTextureImageElement::set(state, node, size, numComponents, bytes,
                             (SoTextureImageElement::Wrap &)wrapS,
                             (SoTextureImageElement::Wrap &)wrapT,
                             (SoTextureImageElement::Model &)model,
                             blendColor);
}

//! FIXME: write doc.

const unsigned char *
SoTextureImageElement::get(SoState * const state,
                           SbVec2s & size,
                           int & numComponents,
                           Wrap & wrapS,
                           Wrap & wrapT,
                           Model & model,
                           SbColor &blendColor)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);

  size = elem->size;
  numComponents = elem->numComponents;
  wrapS = elem->wrapS;
  wrapT = elem->wrapT;
  model = elem->model;
  blendColor = elem->blendColor;
  return elem->bytes;
}

/*!
  FIXME: write doc.
*/

const unsigned char *
SoTextureImageElement::get(SoState * const state, SbVec2s & size,
                           int & numComponents, int & wrapS,
                           int & wrapT, int & model, SbColor & blendColor)
{
  return SoTextureImageElement::get(state, size, numComponents,
                                    (SoTextureImageElement::Wrap &)wrapS,
                                    (SoTextureImageElement::Wrap &)wrapT,
                                    (SoTextureImageElement::Model &)model,
                                    blendColor);
}

//! FIXME: write doc.

SbBool
SoTextureImageElement::containsTransparency(SoState * const state)
{
  const SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);

  return elem->hasTransparency();
}

/*!
  Called by containsTransparency(). Returns \e TRUE if image data
  has transparency. Default method does a very poor job of detecting
  this, since it returns \e TRUE when the number of components are 2
  or 4. Overload whenever it is important to know this
  (SoGLTextureImageElement overloads it to avoid transparency handling
  where possible).
*/
SbBool
SoTextureImageElement::hasTransparency(void) const
{
  return this->numComponents == 2 || this->numComponents == 4;
}

//! FIXME: write doc.

const unsigned char *
SoTextureImageElement::getDefault(SbVec2s &size, int &numComponents)
{
  // FIXME: what is the default texture???
  size.setValue(0,0);
  numComponents = 0;
  return NULL;
}

//! FIXME: write doc.

void
SoTextureImageElement::setElt(const SbVec2s &size, const int numComponents,
                              const unsigned char *bytes, const Wrap wrapS,
                              const Wrap wrapT, const Model model,
                              const SbColor &blendColor)
{
  this->size = size;
  this->numComponents = numComponents;
  this->bytes = bytes;
  this->wrapS = wrapS;
  this->wrapT = wrapT;
  this->model = model;
  this->blendColor = blendColor;
}

const SbColor &
SoTextureImageElement::getBlendColor(SoState * const state)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->blendColor;
}

const unsigned char *
SoTextureImageElement::getImage(SoState * const state,
                                SbVec2s &size,
                                int &numComponents)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);
  size = elem->size;
  numComponents = elem->numComponents;
  return elem->bytes;
}

SoTextureImageElement::Wrap
SoTextureImageElement::getWrapS(SoState * const state)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->wrapT;
}

SoTextureImageElement::Wrap
SoTextureImageElement::getWrapT(SoState * const state)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->wrapS;
}

SoTextureImageElement::Model
SoTextureImageElement::getModel(SoState * const state)
{
  SoTextureImageElement *elem = (SoTextureImageElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->model;
}
