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
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement is used to control the current GL texture.
  \ingroup elements

  This is a lazy element. It is evaluated in SoShape::shouldGLRender().
*/

#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h> // GL wrapper.
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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
  this->glimage = NULL;
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
  this->glimage = NULL;
  this->state = state;
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
  
  if (prev->glimage && prev->glimage->getImage()) prev->glimage->getImage()->readUnlock();
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
                             const SbColor & blendColor)
{
  SoGLTextureImageElement * elem = (SoGLTextureImageElement*)
    state->getElement(classStackIndex);
  if (!elem) return;

  if (elem->glimage && elem->glimage->getImage()) elem->glimage->getImage()->readUnlock();
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
    elem->glimage = image;
    // make sure image isn't changed while this is the active texture
    if (image->getImage()) image->getImage()->readLock();
  }
  else {
    elem->glimage = NULL;
    inherited::setDefault(state, node);
  }
  elem->updateLazyElement();
}

SoGLImage *
SoGLTextureImageElement::get(SoState * state, Model & model,
                             SbColor & blendcolor)
{
  const SoGLTextureImageElement * elem = (const SoGLTextureImageElement*)
    state->getConstElement(classStackIndex);
  
  model = elem->model;
  blendcolor = elem->blendColor;
  return elem->glimage;
}

// doc from parent
SbBool
SoGLTextureImageElement::hasTransparency(void) const
{
  if (this->glimage) {
    // only return TRUE if the image has transparency, and if it can't
    // be rendered using glAlphaTest()
    return this->glimage->hasTransparency() && !this->glimage->useAlphaTest();
  }
  return FALSE;
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

  const cc_glglue * glw = sogl_glue_instance(this->state);
  if (zsize==0) { // 2D textures
    if (cc_glglue_has_2d_proxy_textures(glw)) {
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
    if (cc_glglue_has_3d_textures(glw) &&
        cc_glglue_has_3d_proxy_textures(glw)) {
      cc_glglue_glTexImage3D(glw,
                             GL_PROXY_TEXTURE_3D, 0, bytespertexel,
                             xsize, ysize, zsize, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE,
                             NULL);
      GLint w;
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_3D, 0,
                               GL_TEXTURE_WIDTH, &w);
      if (w==0) return FALSE;
      return TRUE;
    }
    else {
#if COIN_DEBUG
      static SbBool first = TRUE;
      if (first) {
        SbString err("3D textures");
        if (cc_glglue_has_3d_textures(glw)) err = "3D proxy textures";
        SoDebugError::post("SoGLTextureImageElement::isTextureSizeLegal",
                           "%s not supported with this OpenGL driver",
                           err.getString());
        first = FALSE;
      }
#endif // COIN_DEBUG
      return FALSE;
    }
  }
}

void 
SoGLTextureImageElement::updateLazyElement(void) const
{
  if (state->isElementEnabled(SoLazyElement::getClassStackIndex())) {
    uint32_t glimageid = this->glimage ? this->glimage->getGLImageId() : 0;
    SbBool alphatest = this->glimage && this->glimage->getImage() && 
      this->glimage->getImage()->hasData() ? 
      this->glimage->useAlphaTest() : FALSE;
    
    SoLazyElement::setGLImageId(state, glimageid, alphatest);
  }
}


