/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTextureImageElement Inventor/elements/SoTextureImageElement.h
  \brief The SoTextureImageElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>

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
  \fn SoTextureImageElement::wrapR

  FIXME: write doc.

  \since 2001-12-04
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
  this->wrapR = REPEAT;
  this->model = MODULATE;
  this->blendColor.setValue(0.0f, 0.0f, 0.0f);
}

/*!
  Resets this element to its original values.
*/
void
SoTextureImageElement::setDefault(SoState * const state, SoNode * const node)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getElement(classStackIndex);
  if (elem) {
    elem->setDefaultValues();
  }
}

//! FIXME: write doc.
void
SoTextureImageElement::set(SoState * const state, SoNode * const node,
                           const SbVec2s & size, const int numComponents,
                           const unsigned char * bytes,
                           const Wrap wrapS, const Wrap wrapT,
                           const Model model, const SbColor & blendColor)
{
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getElement(classStackIndex);
  if (elem) {
    elem->setElt(size, numComponents, bytes, wrapS, wrapT,
                 model, blendColor);
  }
}

/*!
  FIXME: write doc.

  \since 2001-12-04
*/
void
SoTextureImageElement::set(SoState * const state, SoNode * const node,
                           const SbVec3s & size, const int numComponents,
                           const unsigned char * bytes,
                           const Wrap wrapS, 
                           const Wrap wrapT, 
                           const Wrap wrapR,
                           const Model model, const SbColor & blendColor)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getElement(classStackIndex);

  if (elem) {
    elem->setElt(size, numComponents, bytes, wrapS, wrapT, wrapR,
                 model, blendColor);
  }
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

/*!
  FIXME: write doc.

  \since 2001-12-04
*/
void SoTextureImageElement::set(SoState * const state, SoNode * const node,
                                const SbVec3s & size, const int numComponents,
                                const unsigned char * bytes, const int wrapS,
                                const int wrapT, const int wrapR, 
                                const int model,
                                const SbColor & blendColor)
{
  SoTextureImageElement::set(state, node, size, numComponents, bytes,
                             (SoTextureImageElement::Wrap &)wrapS,
                             (SoTextureImageElement::Wrap &)wrapT,
                             (SoTextureImageElement::Wrap &)wrapR,
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
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getConstElement(classStackIndex);

  wrapS = elem->wrapS;
  wrapT = elem->wrapT;
  model = elem->model;
  blendColor = elem->blendColor;
  
  return getImage(state, size, numComponents);
}

/*! FIXME: write doc.

  \since 2001-12-04
*/
const unsigned char *
SoTextureImageElement::get(SoState * const state,
                           SbVec3s & size,
                           int & numComponents,
                           Wrap & wrapS,
                           Wrap & wrapT,
                           Wrap & wrapR,
                           Model & model,
                           SbColor &blendColor)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getConstElement(classStackIndex);

  wrapS = elem->wrapS;
  wrapT = elem->wrapT;
  wrapR = elem->wrapR;
  model = elem->model;
  blendColor = elem->blendColor;
  
  return getImage(state, size, numComponents);
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
/*!
  FIXME: write doc.

  \since 2001-12-04
*/
const unsigned char *
SoTextureImageElement::get(SoState * const state, SbVec3s & size,
                           int & numComponents, int & wrapS,
                           int & wrapT, int & wrapR, 
                           int & model, SbColor & blendColor)
{
  return SoTextureImageElement::get(state, size, numComponents,
                                    (SoTextureImageElement::Wrap &)wrapS,
                                    (SoTextureImageElement::Wrap &)wrapT,
                                    (SoTextureImageElement::Wrap &)wrapR,
                                    (SoTextureImageElement::Model &)model,
                                    blendColor);
}

//! FIXME: write doc.

SbBool
SoTextureImageElement::containsTransparency(SoState * const state)
{
  const SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getConstElement(classStackIndex);

  return elem->hasTransparency();
}

/*!
  Called by containsTransparency(). Returns \e TRUE if image data has
  transparency. Default method does a very poor job of detecting this,
  since it returns \e TRUE when the number of components are 2 or
  4. Override whenever it is important to know this
  (SoGLTextureImageElement overrides it to avoid transparency handling
  where possible).
*/
SbBool
SoTextureImageElement::hasTransparency(void) const
{
  return (this->numComponents==2 || this->numComponents==4);
}

//! FIXME: write doc.

const unsigned char *
SoTextureImageElement::getDefault(SbVec2s & size, int & numComponents)
{
  size.setValue(0,0);
  numComponents = 0;
  return NULL;
}

/*! FIXME: write doc.
 
  \since 2001-12-04
*/
const unsigned char *
SoTextureImageElement::getDefault(SbVec3s & size, int & numComponents)
{
  size.setValue(0,0,0);
  numComponents = 0;
  return NULL;
}

//! FIXME: write doc.

void
SoTextureImageElement::setElt(const SbVec2s &size, const int numComponents,
                              const unsigned char * bytes, const Wrap wrapS,
                              const Wrap wrapT, const Model model,
                              const SbColor &blendColor)
{
  this->size.setValue(size[0],size[1],1);
  this->numComponents = numComponents;
  this->bytes = bytes;
  this->wrapS = wrapS;
  this->wrapT = wrapT;
  this->wrapR = REPEAT;
  this->model = model;
  this->blendColor = blendColor;
}

/*!
  FIXME: write doc.

  \since 2001-12-04 
*/
void
SoTextureImageElement::setElt(const SbVec3s &size, const int numComponents,
                              const unsigned char * bytes, const Wrap wrapS,
                              const Wrap wrapT, const Wrap wrapR, 
                              const Model model,
                              const SbColor &blendColor)
{
  this->size = size;
  this->numComponents = numComponents;
  this->bytes = bytes;
  this->wrapS = wrapS;
  this->wrapT = wrapT;
  this->wrapR = wrapR;
  this->model = model;
  this->blendColor = blendColor;
}

const SbColor &
SoTextureImageElement::getBlendColor(SoState * const state)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getConstElement(classStackIndex);
  return elem->blendColor;
}

const unsigned char *
SoTextureImageElement::getImage(SoState * const state,
                                SbVec2s &size,
                                int &numComponents)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getConstElement(classStackIndex);

  if (elem->getTypeId().isDerivedFrom(SoGLTextureImageElement::getClassTypeId())) {
    Model dummy1;
    SbColor dummy2;
    SoGLImage * image = SoGLTextureImageElement::get(state, dummy1, dummy2);
    unsigned char * bytes = NULL;
    size = SbVec2s(0,0);
    numComponents = 0;
    if (image && image->getImage()) {
      bytes = image->getImage()->getValue(size, numComponents);
    }
    return bytes;
  }
  size.setValue(elem->size[0], elem->size[1]);
  numComponents = elem->numComponents;
  return elem->bytes;
}

/*!
  \since 2001-12-04 
*/
const unsigned char *
SoTextureImageElement::getImage(SoState * const state,
                                SbVec3s &size,
                                int &numComponents)
{
  SoTextureImageElement * elem = (SoTextureImageElement*)
    state->getConstElement(classStackIndex);

  if (elem->getTypeId().isDerivedFrom(SoGLTextureImageElement::getClassTypeId())) {
    Model dummy1;
    SbColor dummy2;
    SoGLImage * image = SoGLTextureImageElement::get(state, dummy1, dummy2);
    unsigned char * bytes = NULL;
    size = SbVec3s(0,0,0);
    numComponents = 0;
    if (image && image->getImage()) {
      bytes = image->getImage()->getValue(size, numComponents);
    }
    return bytes;
  }
  size = elem->size;
  numComponents = elem->numComponents;
  return elem->bytes;
}

SoTextureImageElement::Wrap
SoTextureImageElement::getWrapS(SoState * const state)
{
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getConstElement(classStackIndex);
  return elem->wrapT;
}

SoTextureImageElement::Wrap
SoTextureImageElement::getWrapT(SoState * const state)
{
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getConstElement(classStackIndex);
  return elem->wrapS;
}

/*! 
  \since 2001-12-04 
*/
SoTextureImageElement::Wrap
SoTextureImageElement::getWrapR(SoState * const state)
{
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getConstElement(classStackIndex);
  return elem->wrapR;
}

SoTextureImageElement::Model
SoTextureImageElement::getModel(SoState * const state)
{
  SoTextureImageElement * elem = (SoTextureImageElement *)
    state->getConstElement(classStackIndex);
  return elem->model;
}
