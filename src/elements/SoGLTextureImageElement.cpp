/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement is used to control the current GL texture.
  \ingroup elements

  This is a lazy element. It is evaluated in SoShape::shouldGLRender().
*/

#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include "../misc/GLWrapper.h"

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
  this->quality = -1.0f;

  // set these to illegal values to make sure things are initialized
  // the first time.
  this->glmodel = -1;
  this->glblendcolor.setValue(-1.0f, -1.0f, -1.0f);
  this->glalphatest = FALSE;
  glDisable(GL_ALPHA_TEST);

  // store state to be able to apply dlists
  this->state = state;
}


// Documented in superclass. Overridden to pass GL state to the next
// element.
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


// Documented in superclass. Overridden to pass GL state to the
// previous element.
void
SoGLTextureImageElement::pop(SoState * state,
                             const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLTextureImageElement * prev = (SoGLTextureImageElement*)
    prevTopElement;
  
  if (prev->dlist) prev->dlist->unref(state); // unref dlist (ref'ed in set())
  if (prev->image && prev->image->getImage()) prev->image->getImage()->readUnlock();

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
  if (!elem) return;
  
  if (elem->dlist) elem->dlist->unref();
  if (elem->image && elem->image->getImage()) elem->image->getImage()->readUnlock();
  if (image) {
    // keep SoTextureImageElement "up-to-date"
    inherited::set(state, node,
                   SbVec3s(0,0,0),
                   0,
                   NULL,
                   translateWrap(image->getWrapS()),
                   translateWrap(image->getWrapT()),
                   translateWrap(image->getWrapR()),
                   model,
                   blendColor);
    elem->quality = -1.0f;
    elem->image = image;
    // make sure image isn't changed while this is the active texture
    if (image->getImage()) image->getImage()->readLock(); 
    elem->didapply = didapply;
    // FIXME: the next line causes a memory leak, according to
    // Purify. 20001102 mortene.
    elem->dlist = image->getGLDisplayList(state);
    if (elem->dlist) elem->dlist->ref(); // ref to make sure dlist is not deleted too soon
    elem->alphatest = image->useAlphaTest();
  }
  else {
    elem->didapply = FALSE;
    elem->image = NULL;
    elem->dlist = NULL;
    elem->alphatest = FALSE;
    inherited::setDefault(state, node);
  }
}

SoGLImage * 
SoGLTextureImageElement::get(SoState * state, Model & model,
                             SbColor & blendcolor)
{
  const SoGLTextureImageElement * elem = (const SoGLTextureImageElement*)
    SoReplacedElement::getConstElement(state, classStackIndex);
  
  model = elem->model;
  blendcolor = elem->blendColor;
  return elem->image;
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
  SoGLTextureImageElement * elem = (SoGLTextureImageElement *)this;

#ifdef HAVE_THREADS
  // if threads is enabled, the image is loaded on demand, and we
  // should trigger a image load by just attempting to fetch the data
  // from the image.
  if (!enabled && elem->image && elem->image->getImage()) {
    SbVec3s size;
    int nc;
    (void) elem->image->getImage()->getValue(size, nc);
  }
#endif // HAVE_THREADS

  if (enabled && elem->image) SoGLImage::tagImage(elem->state, elem->image);
  if (enabled && elem->dlist) {
    // notify the texture resource handler that this image/dl has been
    // recently used
    if (!elem->didapply) {
      elem->dlist->call(elem->state);
      elem->didapply = TRUE;
      elem->quality = -1.0f;
    }
    float quality = SoTextureQualityElement::get(elem->state);
    if (elem->quality != quality) {
      elem->image->applyQuality(elem->dlist, quality);
      elem->quality = quality;
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
      // draw everything with alpha > 0.5. This will make the texture
      // look ok even when linear filtering is used.
      glAlphaFunc(GL_GREATER, 0.5f);
      glEnable(GL_ALPHA_TEST);
    }
    else glDisable(GL_ALPHA_TEST);
  }
}

/*!
  The size returned by this function can be a very coarse estimate as
  it uses glGetIntegerv(GL_MAX_TEXTURE_SIZE).
  For a better estimate, use isTextureSizeLegal(),
 */
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

/*!
  Returns true if the texture with the given dimensions is supported
  by the current OpenGL context.
  If zsize==0, 2D texturing is assumed, else 3D texturing is assumed.

  This function uses PROXY textures and will fall back to getMaxGLTextureSize()
  if PROXY textures are not supported (OpenGL < 1.1 and GL_EXT_texture not
  available). In the 3D case, 3D textures need to be supported (OpenGL >= 1.2 
  or GL_EXT_texture3D).

  \since 2001-12-04
*/
SbBool 
SoGLTextureImageElement::isTextureSizeLegal(int xsize, int ysize, int zsize, 
                                            int bytespertexel)
{
  // FIXME: the technique we are using doesn't really match what is recommended at
  //  http://www.opengl.org/developers/documentation/OGL_userguide/OpenGLonWin-13.html
  // (see "Testing Whether Textures Fit: The Texture Proxy Mechanism").
  // 20020701 mortene.

  // FIXME: mipmaps must be handled specifically, which we are not
  // doing. 20020701 mortene.

  const GLWrapper_t * glw = GLWRAPPER_FROM_STATE(this->state);
  if (zsize==0) { // 2D textures
    if (glw->has2DProxyTextures) {
      GLint w;
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, bytespertexel,
                   xsize, ysize, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE,
                   NULL);
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, 
                               GL_TEXTURE_WIDTH, &w);
      if (w==0) return FALSE;
      return TRUE;
    }
    else {
      // Falls back to using the old method
      int maxsize = SoGLTextureImageElement::getMaxGLTextureSize();
      if (xsize > maxsize || ysize > maxsize) return FALSE;
      return TRUE;
    }
  }
  else { // 3D textures
    if (glw->has3DProxyTextures && glw->glTexImage3D) {
      GLint w;
      glw->glTexImage3D(GL_PROXY_TEXTURE_3D, 0, bytespertexel,
                        xsize, ysize, zsize, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE,
                        NULL);
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0, 
                               GL_TEXTURE_WIDTH, &w);
      if (w==0) return FALSE;
      return TRUE;
    }
    return FALSE; // 3D textured not supported
  }
}
