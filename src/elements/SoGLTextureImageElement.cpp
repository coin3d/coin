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
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement is used to control the current GL texture.

  This is a lazy element. It is evaluated in SoShape::shouldGLRender().
*/

#include <Inventor/elements/SoGLTextureImageElement.h>

#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/misc/SoGLImage.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
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
  this->quality = SoTextureQualityElement::getDefault();
  this->image = NULL;   // the image stored in this element
  this->glimage = NULL; // the image currently sent to GL

  // set these to illegal values to make sure things are initialized
  // the first time.
  this->glmodel = -1;
  this->glquality = -1.0f;
  this->glblendcolor.setValue(-1.0f, -1.0f, -1.0f);
  this->glalphatest = FALSE;
  glDisable(GL_ALPHA_TEST);
}


/*!
  Overloaded to pass GL state to the next element.
*/
void
SoGLTextureImageElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLTextureImageElement*)this->next)->glimage = this->glimage;
  ((SoGLTextureImageElement*)this->next)->glquality = this->glquality;
  ((SoGLTextureImageElement*)this->next)->glmodel = this->glmodel;
  ((SoGLTextureImageElement*)this->next)->glblendcolor = this->glblendcolor;
  ((SoGLTextureImageElement*)this->next)->glalphatest = this->glalphatest;
}


/*!
  Overloaded to pass GL state to the previous element.
*/
void
SoGLTextureImageElement::pop(SoState * state,
                             const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLTextureImageElement *prev = (SoGLTextureImageElement*)
    prevTopElement;

  prev->glimage = this->glimage;
  prev->glmodel = this->glmodel;
  prev->glquality = this->glquality;
  prev->glblendcolor = this->glblendcolor;
  prev->glalphatest = this->glalphatest;
}

/*!
  Sets the current texture.
*/
void
SoGLTextureImageElement::set(SoState * const state, SoNode * const node,
                             SoGLImage *image, const Model model,
                             const SbColor &blendColor)
{
  SoGLTextureImageElement * elem = (SoGLTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (image) {
    // keep SoTextureImageElement "up-to-date"
    inherited::set(state, node,
                   image->getSize(),
                   image->getNumComponents(),
                   image->getDataPtr(),
                   image->shouldClampS() ?
                   SoTextureImageElement::CLAMP :
                   SoTextureImageElement::REPEAT,
                   image->shouldClampT() ?
                   SoTextureImageElement::CLAMP :
                   SoTextureImageElement::REPEAT,
                   model,
                   blendColor);
    elem->image = image;
    elem->quality = SoTextureQualityElement::get(state);
    elem->alphatest = image->needAlphaTest();
  }
  else {
    elem->image = NULL;
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

  if (enabled && elem->image) {
    if (elem->image != elem->glimage || !elem->image->isValid()) {
      elem->glimage = elem->image;
      elem->glquality = elem->quality;
      elem->image->apply(elem->quality);
    }
    else if (elem->quality != elem->glquality) {
      elem->glquality = elem->quality;
      elem->image->apply(elem->quality);
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

