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
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement is used to control the current GL texture.

  This is a lazy element. It is evaluated in SoShape::shouldGLRender().
*/

#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/misc/SoGLImage.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

SO_ELEMENT_SOURCE(SoGLTextureImageElement);

/*!
  This static method initializes static data in the
  SoGLTextureImageElement class.
*/

void
SoGLTextureImageElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLTextureImageElement, inherited);
}

/*!
  The destructor.
*/

SoGLTextureImageElement::~SoGLTextureImageElement(void)
{
}


// doc from parent
void
SoGLTextureImageElement::init(SoState * state)
{
  inherited::init(state);
  this->dlist = NULL;
  this->image = NULL;
  this->didapply = FALSE;

  // set these to illegal values to make sure things are initialized
  // the first time.
  this->quality = SoTextureQualityElement::getDefault();
  this->glmodel = -1;
  this->glblendcolor.setValue(-1.0f, -1.0f, -1.0f);
  this->glalphatest = FALSE;
  glDisable(GL_ALPHA_TEST);

  // store state to be able to apply dlists
  this->state = state;
}


/*!
  Overloaded to pass GL state to the next element.
*/
void
SoGLTextureImageElement::push(SoState * state)
{
  inherited::push(state);
  SoGLTextureImageElement * prev = (SoGLTextureImageElement*)
    this->getNextInStack();
  this->glmodel = prev->glmodel;
  this->glblendcolor = prev->glblendcolor;
  this->glalphatest = prev->glalphatest;
  this->state = state;
  this->image = NULL;
  this->dlist = NULL;
  this->didapply = FALSE;
}


/*!
  Overloaded to pass GL state to the previous element.
*/
void
SoGLTextureImageElement::pop(SoState * state,
                             const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLTextureImageElement * prev = (SoGLTextureImageElement*)
    prevTopElement;
  
  if (prev->dlist) prev->dlist->unref(state); // unref dlist (ref'ed in set())
  this->glmodel = prev->glmodel;
  this->glblendcolor = prev->glblendcolor;
  this->glalphatest = prev->glalphatest;
  this->didapply = FALSE; // force texture to be applied in the next evaluate()
}

static SoTextureImageElement::Wrap
translateWrap(const SoGLImage::Wrap wrap)
{
  // FIXME: add test when OpenGL 1.2 feature CLAMP_TO_EDGE is added
  if (wrap == SoGLImage::REPEAT) return SoTextureImageElement::REPEAT;
  return SoTextureImageElement::CLAMP;
}

/*!
  Sets the current texture. Id \a didapply is TRUE, it is assumed 
  that the texture image already is the current GL texture. Do not
  use this feature unless you know what you're doing.
*/
void
SoGLTextureImageElement::set(SoState * const state, SoNode * const node,
                             SoGLImage * image, const Model model,
                             const SbColor & blendColor,
                             const SbBool didapply)
{
  SoGLTextureImageElement * elem = (SoGLTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (elem->dlist) elem->dlist->unref();
  if (image) {
    // keep SoTextureImageElement "up-to-date"
    inherited::set(state, node,
                   image->getSize(),
                   image->getNumComponents(),
                   image->getDataPtr(),
                   translateWrap(image->getWrapS()),
                   translateWrap(image->getWrapS()),
                   model,
                   blendColor);
    elem->image = image;
    elem->didapply = didapply;
    elem->quality = SoTextureQualityElement::get(state);
    // FIXME: the next line causes a memory leak, according to
    // Purify. 20001102 mortene.
    elem->dlist = image->getGLDisplayList(state, elem->quality);
    if (elem->dlist) elem->dlist->ref(); // ref to make sure dlist is not deleted too soon
    elem->alphatest = image->needAlphaTest();
  }
  else {
    elem->didapply = FALSE;
    elem->image = NULL;
    elem->dlist = NULL;
    elem->quality = SoTextureQualityElement::get(state);
    elem->alphatest = FALSE;
    inherited::setDefault(state, node);
  }
}

// doc from parent
SbBool
SoGLTextureImageElement::hasTransparency(void) const
{
  if (this->image) {
    return this->image->hasTransparency();
  }
  return FALSE;
}

/*!
  Evaluates this lazy element. \a enabled should be TRUE if texturing is
  enabled. \a transparency should be TRUE if the current material is
  transparent and screen door transparency is not used.
*/
void
SoGLTextureImageElement::evaluate(const SbBool enabled, const SbBool transparency) const
{
  // cast away constness
  SoGLTextureImageElement *elem = (SoGLTextureImageElement*) this;

  if (enabled && elem->dlist) {
    if (!elem->didapply) {
      SoGLImage::apply(elem->state, elem->dlist, elem->quality);
      elem->didapply = TRUE;
    }
    if (int(elem->model) != elem->glmodel ||
        (elem->model == BLEND && elem->blendColor != elem->glblendcolor)) {
      elem->glmodel = (int) elem->model;
      elem->glblendcolor = elem->blendColor;

      if (model == DECAL) {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
      }
      else if (model == MODULATE) {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      }
      else {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blendColor.getValue());
      }
    }
  }

  // if transparent or !enabled, disable alpha test
  if (transparency || !enabled) {
    if (elem->glalphatest) {
      glDisable(GL_ALPHA_TEST);
      elem->glalphatest = FALSE;
    }
  }
  else if (elem->alphatest != elem->glalphatest) {
    // update GL alpha test
    elem->glalphatest = elem->alphatest;
    if (elem->alphatest) {
      // draw everything with alpha != 0.0
      glAlphaFunc(GL_NOTEQUAL, 0.0f);
      glEnable(GL_ALPHA_TEST);
    }
    else glDisable(GL_ALPHA_TEST);
  }
}

int32_t
SoGLTextureImageElement::getMaxGLTextureSize(void)
{
  static int32_t maxGLTextureSize = -1;
  if (maxGLTextureSize == -1) {
    GLint val;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
    maxGLTextureSize = (int32_t)val;
  }
  return maxGLTextureSize;
}
