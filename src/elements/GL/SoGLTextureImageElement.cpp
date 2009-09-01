/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement is used to control the current GL texture.
  \ingroup elements

  This is a lazy element. It is evaluated in SoShape::shouldGLRender().
*/

// *************************************************************************

#include <Inventor/elements/SoGLTextureImageElement.h>

#include <cstdlib>

#include <Inventor/C/tidbits.h>
#include <Inventor/SbImage.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/misc/SoGLImage.h>

#include <Inventor/elements/SoGLShaderProgramElement.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H
#include "glue/glp.h"
#include "rendering/SoGL.h" // GL wrapper.
#include "shaders/SoGLShaderProgram.h"

// *************************************************************************

// Can be used as a workaround for buggy PROXY texture handling (bug
// in our code or in the OpenGL driver).
static int COIN_MAXIMUM_TEXTURE2_SIZE = 0;
static int COIN_MAXIMUM_TEXTURE3_SIZE = 0;

// *************************************************************************

SO_ELEMENT_SOURCE(SoGLTextureImageElement);

// *************************************************************************

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
SoGLTextureImageElement::init(SoState * stateptr)
{
  inherited::init(stateptr);
  this->glimage = NULL;
  this->state = stateptr;

  // check environment variables
  if (COIN_MAXIMUM_TEXTURE2_SIZE == 0) {
    const char * env = coin_getenv("COIN_MAXIMUM_TEXTURE2_SIZE");
    if (env) COIN_MAXIMUM_TEXTURE2_SIZE = atoi(env);
    else COIN_MAXIMUM_TEXTURE2_SIZE = -1;
  }
  if (COIN_MAXIMUM_TEXTURE3_SIZE == 0) {
    const char * env = coin_getenv("COIN_MAXIMUM_TEXTURE3_SIZE");
    if (env) COIN_MAXIMUM_TEXTURE3_SIZE = atoi(env);
    else COIN_MAXIMUM_TEXTURE3_SIZE = -1;
  }
}


// Documented in superclass. Overridden to pass GL state to the next
// element.
void
SoGLTextureImageElement::push(SoState * stateptr)
{
  inherited::push(stateptr);
  this->glimage = NULL;
  this->state = stateptr;
}


// Documented in superclass. Overridden to pass GL state to the
// previous element.
void
SoGLTextureImageElement::pop(SoState * stateptr,
                             const SoElement * prevTopElement)
{
  inherited::pop(stateptr, prevTopElement);
  SoGLTextureImageElement * prev = (SoGLTextureImageElement*)
    prevTopElement;

  if (prev->glimage && prev->glimage->getImage()) prev->glimage->getImage()->readUnlock();

  SoGLShaderProgram * prog = SoGLShaderProgramElement::get(stateptr);
  if (prog) prog->updateCoinParameter(stateptr, SbName("coin_texunit0_model"),
                                      this->glimage != NULL ? this->model : 0);
}

static SoTextureImageElement::Wrap
translateWrap(const SoGLImage::Wrap wrap)
{
  if (wrap == SoGLImage::REPEAT) return SoTextureImageElement::REPEAT;
  return SoTextureImageElement::CLAMP;
}

/*!
  Sets the current texture. Id \a didapply is TRUE, it is assumed
  that the texture image already is the current GL texture. Do not
  use this feature unless you know what you're doing.
*/
void
SoGLTextureImageElement::set(SoState * const stateptr, SoNode * const node,
                             SoGLImage * image, const Model model,
                             const SbColor & blendColor)
{
  SoGLTextureImageElement * elem = (SoGLTextureImageElement*)
    stateptr->getElement(classStackIndex);

  if (elem->glimage && elem->glimage->getImage()) elem->glimage->getImage()->readUnlock();
  if (image) {
    // keep SoTextureImageElement "up-to-date"
    inherited::set(stateptr, node,
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
    inherited::setDefault(stateptr, node);
  }
  SoShapeStyleElement::setBigImageEnabled(stateptr,
                                          image && image->isOfType(SoGLBigImage::getClassTypeId()));
  sogl_update_shapehints_transparency(stateptr);
  
  elem->updateLazyElement();

  SoGLShaderProgram * prog = SoGLShaderProgramElement::get(stateptr);
  if (prog) prog->updateCoinParameter(stateptr, SbName("coin_texunit0_model"), elem->glimage ? elem->model : 0);
}

SoGLImage *
SoGLTextureImageElement::get(SoState * state, Model & model,
                             SbColor & blendcolor)
{
  const SoGLTextureImageElement * elem = (const SoGLTextureImageElement*)
    getConstElement(state, classStackIndex);

  model = elem->model;
  blendcolor = elem->blendColor;
  return elem->glimage;
}

/*!

  Return TRUE if at least one pixel in the current texture image is
  transparent.

  \since Coin 3.1
 */
SbBool 
SoGLTextureImageElement::hasTransparency(SoState * state)
{
  const SoGLTextureImageElement * elem = (const SoGLTextureImageElement*)
    getConstElement(state, classStackIndex);
  return elem->hasTransparency();
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
  The size returned by this function will just be a very coarse
  estimate as it only uses the more or less obsoleted technique of
  calling glGetIntegerv(GL_MAX_TEXTURE_SIZE).

  For a better estimate, use
  SoGLTextureImageElement::isTextureSizeLegal().

  Note that this function needs an OpenGL context to be made current
  for it to work. Without that, you will most likely get a faulty
  return value or even a crash.
*/
int32_t
SoGLTextureImageElement::getMaxGLTextureSize(void)
{
  SoDebugError::postWarning("SoGLTextureImageElement::getMaxGLTextureSize",
                            "This function is obsoleted. It should not "
                            "be used because its interface is fubar: "
                            "the maximum texture size handled by "
                            "the OpenGL driver depends on the context, and "
                            "this function does not know which context this "
                            "information is requested for.");

  GLint val;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);
  return (int32_t)val;
}

/*!
  Returns true if the texture with the given dimensions is supported
  by the current OpenGL context.

  If zsize==0, 2D texturing is assumed, else 3D texturing is assumed.

  This function uses OpenGL proxy textures and will fall back to what
  you get from SoGLTextureImageElement::getMaxGLTextureSize() if proxy
  textures are not supported (i.e. OpenGL version < 1.1 and
  GL_EXT_texture not available). In the 3D case, 3D textures need to
  be supported (OpenGL >= 1.2 or GL_EXT_texture3D).

  Note that this function needs an OpenGL context to be made current
  for it to work. Without that, you will most likely get a faulty
  return value or even a crash.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SbBool
SoGLTextureImageElement::isTextureSizeLegal(int xsize, int ysize, int zsize,
                                            int bytespertexel)
{
  const cc_glglue * glw = sogl_glue_instance(this->state);
  SbBool compress =
    this->glimage ? this->glimage->getFlags() & SoGLImage::COMPRESSED : FALSE;


  GLenum internalformat = coin_glglue_get_internal_texture_format(glw, bytespertexel, compress);
  GLenum format = coin_glglue_get_texture_format(glw, bytespertexel);

  return coin_glglue_is_texture_size_legal(glw, xsize, ysize, zsize,
                                           internalformat, format, GL_UNSIGNED_BYTE, TRUE);
}

void
SoGLTextureImageElement::updateLazyElement(void) const
{
  if (state->isElementEnabled(SoLazyElement::getClassStackIndex())) {
    uint32_t glimageid = this->glimage ? this->glimage->getGLImageId() : 0;
    SbBool alphatest = FALSE;
    uint32_t flags = this->glimage ? this->glimage->getFlags() : 0;
    if (flags & SoGLImage::FORCE_ALPHA_TEST_TRUE) {
      alphatest = TRUE;
    }
    else if (flags & SoGLImage::FORCE_ALPHA_TEST_FALSE) {
      alphatest = FALSE;
    }
    else {
      alphatest = this->glimage &&
        this->glimage->getImage() &&
        this->glimage->getImage()->hasData() ?
        this->glimage->useAlphaTest() : FALSE;
    }
    SoLazyElement::setGLImageId(state, glimageid, alphatest);
  }
}
