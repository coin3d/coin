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
  A constructor.  Can't be used directly.

  \sa void * SoTextureImageElement::createInstance(void)
*/

SoTextureImageElement::SoTextureImageElement()
{
  setTypeId(SoTextureImageElement::classTypeId);
  setStackIndex(SoTextureImageElement::classStackIndex);
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

  // FIXME: should also check image data if components are 2 or 4?
  return elem->numComponents == 2 || elem->numComponents == 4;
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
