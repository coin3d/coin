/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/


/*!
  \class SoSceneTexture2 SoSceneTexture2.h Inventor/nodes/SoSceneTexture2.h
  \brief The SoSceneTexture2 class is used to create a 2D texture from a Coin scene graph.
  \ingroup nodes

  This node behaves exactly like SoTexture2 when it comes mapping the
  actual texture onto subsequent geometry. Please read the SoTexture2 documentation
  for more information about how textures are mapped onto shapes.

  \since Coin 2.2
*/

/*!
  \enum SoSceneTexture2::Model

  Texture mapping model, for deciding how to "merge" the texturemap
  with the object it is mapped unto.
*/
/*!
  \var SoSceneTexture2::Model SoSceneTexture2::MODULATE

  Texture color is multiplied by the polygon color. The result will
  be Phong shaded (if light model is PHONG).
*/
/*!
  \var SoSceneTexture2::Model SoSceneTexture2::DECAL

  Texture image overwrites polygon shading. Textured pixels will
  not be Phong shaded. Has undefined behaviour for grayscale and
  grayscale-alpha textures.
*/
/*!
  \var SoSceneTexture2::Model SoSceneTexture2::BLEND

  This model is normally used with monochrome textures (i.e. textures
  with one or two components). The first component, the intensity, is
  then used to blend between the shaded color of the polygon and the
  SoSceneTexture2::blendColor.
*/
/*!
  \var SoSceneTexture2::Model SoSceneTexture2::REPLACE

  Texture image overwrites polygon shading. Textured pixels will not
  be Phong shaded. Supports grayscale and grayscale alpha
  textures. This feature requires OpenGL 1.1. MODULATE will be used if
  OpenGL version < 1.1 is detected.

  Please note that using this texture model will make your Inventor
  files incompatible with older versions of Coin and Inventor. You
  need Coin >= 2.2 or TGS Inventor 4.0 to load Inventor files that
  uses the REPLACE texture model.

  \since Coin 2.2
  \since TGS Inventor 4.0
*/

/*!
  \enum SoSceneTexture2::Wrap

  Enumeration of wrapping strategies which can be used when the
  texturemap doesn't cover the full extent of the geometry.
*/
/*!
  \var SoSceneTexture2::Wrap SoSceneTexture2::REPEAT
  Repeat texture when coordinate is not between 0 and 1.
*/
/*!
  \var SoSceneTexture2::Wrap SoSceneTexture2::CLAMP
  Clamp coordinate between 0 and 1.
*/

/*!
  \enum SoSceneTexture2::TransparencyFunction

  For deciding how the texture's alpha channel is handled. It's not
  possible to automatically detect this, since the texture is stored
  only on the graphics card's memory, and it'd be too slow to fetch
  the image to test the alpha channel like Coin does for regular
  textures.
*/

/*!
  \var SoSceneTexture2::Transparency SoSceneTexture2::NONE
  The alpha channel is ignored.
*/

/*!
  \var SoSceneTexture2::Transparency SoSceneTexture2::ALPHA_TEST
  An alpha test function is used.
*/

/*!
  \var SoSceneTexture2::Transparency SoSceneTexture2::ALPHA_BLEND
  Alpha blending is used.
*/

/*!
  \var SoSFEnum SoSceneTexture2::wrapS

  Wrapping strategy for the S coordinate when the texturemap is
  narrower than the object to map unto.

  Default value is SoSceneTexture2::REPEAT.
*/
/*!
  \var SoSFEnum SoSceneTexture2::wrapT

  Wrapping strategy for the T coordinate when the texturemap is
  shorter than the object to map unto.

  Default value is SoSceneTexture2::REPEAT.
*/
/*!
  \var SoSFEnum SoSceneTexture2::model

  Texturemapping model for how the texturemap is "merged" with the
  polygon primitives it is applied to. Default value is
  SoSceneTexture2::MODULATE.
*/
/*!
  \var SoSFColor SoSceneTexture2::blendColor

  Blend color. Used when SoSceneTexture2::model is SoSceneTexture2::BLEND.

  Default color value is [0, 0, 0], black, which means no contribution
  to the blending is made.
*/

/*!
  \var SoSFVec2s SoSceneTexture2::size

  The size of the texture. This node currently only supports power of
  two textures.  If the size is not a power of two, the value will be
  rounded upwards to the next power of two.
*/

/*!
  \var SoSFNode SoSceneTexture2::scene

  The scene graph that is rendered into the texture.
*/

/*!
  \var SoSFVec4f SoSceneTexture2::clearColor
  
  The color the color buffer is cleared to before rendering the scene.
  Default value is (0.0f, 0.0f, 0.0f, 0.0f).
*/

/*!
  \var SoSFEnum SoSceneTexture2::transparencyFunction 
  
  The transparency function used. Default value is NONE.
*/

#include <Inventor/nodes/SoSceneTexture2.h>

#include <assert.h>
#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/misc/SoNotification.h>

#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureScalePolicyElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/C/tidbits.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE


// *************************************************************************

class SoSceneTexture2P {
public:
  SoSceneTexture2P(SoSceneTexture2 * api);
  ~SoSceneTexture2P();

  SoSceneTexture2 * api;
  void * glcontext;
  SbVec2s glcontextsize;

  SoGLImage * glimage;
  SbBool pbuffervalid;
  SbBool glimagevalid;
  SbBool glrectangle;
  void updatePBuffer(SoState * state, const float quality);
  SoGLRenderAction * glaction;
  static void prerendercb(void * userdata, SoGLRenderAction * action);

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
  SbBool canrendertotexture;
  unsigned char * offscreenbuffer;
  int offscreenbuffersize;
};

// *************************************************************************

#undef PRIVATE
#define PRIVATE(obj) obj->pimpl


#ifdef COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_) (PRIVATE(_thisp_)->mutex.lock())
#define UNLOCK_GLIMAGE(_thisp_) (PRIVATE(_thisp_)->mutex.unlock())
#else // COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_)
#define UNLOCK_GLIMAGE(_thisp_)
#endif // COIN_THREADSAFE

SO_NODE_SOURCE(SoSceneTexture2);

// Documented in superclass.
void
SoSceneTexture2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSceneTexture2, SO_FROM_COIN_2_2);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);
  SO_ENABLE(SoGLRenderAction, SoGLTexture3EnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
  SO_ENABLE(SoCallbackAction, SoTextureEnabledElement);
  SO_ENABLE(SoCallbackAction, SoTexture3EnabledElement);

  SO_ENABLE(SoRayPickAction, SoTextureImageElement);
  SO_ENABLE(SoRayPickAction, SoTextureEnabledElement);
  SO_ENABLE(SoRayPickAction, SoTexture3EnabledElement);
}

static SoGLImage::Wrap
translateWrap(const SoSceneTexture2::Wrap wrap)
{
  if (wrap == SoSceneTexture2::REPEAT) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP;
}

SoSceneTexture2::SoSceneTexture2(void)
{
  this->pimpl = new SoSceneTexture2P(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoSceneTexture2);
  SO_NODE_ADD_FIELD(size, (256, 256));
  SO_NODE_ADD_FIELD(scene, (NULL));
  SO_NODE_ADD_FIELD(clearColor, (0.0f, 0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(transparencyFunction, (NONE));

  SO_NODE_ADD_FIELD(wrapS, (REPEAT));
  SO_NODE_ADD_FIELD(wrapT, (REPEAT));
  SO_NODE_ADD_FIELD(model, (MODULATE));
  SO_NODE_ADD_FIELD(blendColor, (0.0f, 0.0f, 0.0f));

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);
  SO_NODE_SET_SF_ENUM_TYPE(transparencyFunction, TransparencyFunction);

  SO_NODE_DEFINE_ENUM_VALUE(Model, MODULATE);
  SO_NODE_DEFINE_ENUM_VALUE(Model, DECAL);
  SO_NODE_DEFINE_ENUM_VALUE(Model, BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Model, REPLACE);
  
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);
  
  SO_NODE_DEFINE_ENUM_VALUE(TransparencyFunction, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(TransparencyFunction, ALPHA_BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(TransparencyFunction, ALPHA_TEST);
}

SoSceneTexture2::~SoSceneTexture2(void)
{
  delete PRIVATE(this);
}


// Documented in superclass.
void
SoSceneTexture2::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  float quality = SoTextureQualityElement::get(state);

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
  SoNode * root = this->scene.getValue();

  LOCK_GLIMAGE(this);

  if (root && (!PRIVATE(this)->pbuffervalid || !PRIVATE(this)->glimagevalid)) {
    PRIVATE(this)->updatePBuffer(state, quality);

    // don't cache when we change the glimage
    SoCacheElement::setInvalid(TRUE);
    if (state->isCacheOpen()) {
      SoCacheElement::invalidate(state);
    }
  }
  UNLOCK_GLIMAGE(this);

  SoTextureImageElement::Model glmodel = (SoTextureImageElement::Model)
    this->model.getValue();

  if (glmodel == SoTextureImageElement::REPLACE) {
    if (!cc_glglue_glversion_matches_at_least(glue, 1, 1, 0)) {
      static int didwarn = 0;
      if (!didwarn) {
        SoDebugError::postWarning("SoSceneTexture2::GLRender",
                                  "Unable to use the GL_REPLACE texture model. "
                                  "Your OpenGL version is < 1.1. "
                                  "Using GL_MODULATE instead.");
        didwarn = 1;
      }
      // use MODULATE and not DECAL, since DECAL only works for RGB
      // and RGBA textures
      glmodel = SoTextureImageElement::MODULATE;
    }
  }

  int unit = SoTextureUnitElement::get(state);
  int maxunits = cc_glglue_max_texture_units(glue);
  if (unit == 0) {
    SoGLTextureImageElement::set(state, this,
                                 PRIVATE(this)->glimage,
                                 glmodel,
                                 this->blendColor.getValue());

    SoGLTexture3EnabledElement::set(state, this, FALSE);
    if (PRIVATE(this)->glimage && PRIVATE(this)->glrectangle) {
      SoGLTextureEnabledElement::enableRectangle(state, this);
    }
    else {
      SoGLTextureEnabledElement::set(state,
                                     PRIVATE(this)->glimage != NULL &&
                                     quality > 0.0f);
    }
    if (this->isOverride()) {
      SoTextureOverrideElement::setImageOverride(state, TRUE);
    }
  }
  else if (unit < maxunits) {
    SoGLMultiTextureImageElement::set(state, this, unit,
                                      PRIVATE(this)->glimage,
                                      glmodel,
                                      this->blendColor.getValue());

    SoGLMultiTextureEnabledElement::set(state, this, unit,
                                        PRIVATE(this)->glimage != NULL &&
                                        quality > 0.0f);
  }
  else {
    // we already warned in SoTextureUnit. I think it's best to just
    // ignore the texture here so that all texture for non-supported
    // units will be ignored. pederb, 2003-11-04
  }
}


// Documented in superclass.
void
SoSceneTexture2::doAction(SoAction * action)
{
#if 0 // disabled until we figure out what to do here, pederb 2003-11-27
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  SoTexture3EnabledElement::set(state, this, FALSE);

  if (size != SbVec2s(0,0)) {
    SoTextureImageElement::set(state, this,
                               size, nc, bytes,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
    SoTextureEnabledElement::set(state, this, TRUE);
  }
  else {
    SoTextureImageElement::setDefault(state, this);
    SoTextureEnabledElement::set(state, this, FALSE);
  }
  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
#endif // disabled
}

// Documented in superclass.
void
SoSceneTexture2::callback(SoCallbackAction * action)
{
  SoSceneTexture2::doAction(action);
}

// Documented in superclass.
void
SoSceneTexture2::rayPick(SoRayPickAction * action)
{
  SoSceneTexture2::doAction(action);
}

// Documented in superclass.
void
SoSceneTexture2::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->scene ||
      f == &this->size) {
    // rerender scene
    PRIVATE(this)->pbuffervalid = FALSE;
  }
  else if (f == &this->wrapS ||
           f == &this->wrapT ||
           f == &this->model ||
           f == &this->transparencyFunction) {
    // no need to render scene again, but update the texture object
    PRIVATE(this)->glimagevalid = FALSE;
  }
  inherited::notify(list);
}

// Documented in superclass.
void
SoSceneTexture2::write(SoWriteAction * action)
{
  inherited::write(action);
}


#undef PRIVATE

// *************************************************************************

#undef PUBLIC
#define PUBLIC(obj) obj->api

SoSceneTexture2P::SoSceneTexture2P(SoSceneTexture2 * api)
{
  this->api = api;
  this->glcontext = NULL;
  this->pbuffervalid = FALSE;
  this->glimagevalid = FALSE;
  this->glimage = NULL;
  this->glaction = NULL;
  this->glcontextsize.setValue(-1,-1);
  this->glrectangle = FALSE;
  this->offscreenbuffer = NULL;
  this->offscreenbuffersize = 0;

#if defined(HAVE_AGL) || defined(HAVE_WGL)
  this->canrendertotexture = TRUE;
#else // AGL || WGL
  this->canrendertotexture = FALSE;
#endif // ! AGL && WGL
}

SoSceneTexture2P::~SoSceneTexture2P()
{
  if (this->glimage) this->glimage->unref(NULL);
  if (this->glcontext != NULL) {
    cc_glglue_context_destruct(this->glcontext);
  }
  delete[] this->offscreenbuffer;
  delete this->glaction;
}

void
SoSceneTexture2P::updatePBuffer(SoState * state, const float quality)
{
  SbVec2s size = PUBLIC(this)->size.getValue();

  SoNode * scene = PUBLIC(this)->scene.getValue();
  assert(scene);

  if ((this->glcontext && this->glcontextsize != size) || (size == SbVec2s(0,0))) {
    if (this->glimage) {
      this->glimage->unref(state);
      this->glimage = NULL;
    }
    if (this->glcontext) {
      cc_glglue_context_destruct(this->glcontext);
      this->glcontextsize.setValue(-1,-1);
      this->glcontext = NULL;
    }
    delete this->glaction;
    this->glaction = NULL;
    this->glimagevalid = FALSE;
  }
  if (size == SbVec2s(0,0)) return;

  // FIXME: temporary until non power of two textures are supported,
  // pederb 2003-12-05
  size[0] = (short) coin_geq_power_of_two(size[0]);
  size[1] = (short) coin_geq_power_of_two(size[1]);

  if (this->glcontext == NULL) {
    this->glcontextsize = size;
    const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
    // disabled until an pbuffer extension is available to create a
    // render-to-texture pbuffer that has a non power of two size.
    // pederb, 2003-12-05
#if 0
    if (!glue->has_ext_texture_rectangle) {
#else
    if (1) {
#endif
      this->glcontextsize[0] = (short) coin_geq_power_of_two(size[0]);
      this->glcontextsize[1] = (short) coin_geq_power_of_two(size[1]);

      if (this->glcontextsize != size) {
        static int didwarn = 0;
        if (!didwarn) {
          SoDebugError::postWarning("SoSceneTexture2P::updatePBuffer",
                                    "Requested non power of two size, but your OpenGL "
                                    "driver lacks support for such pbuffer textures.");
          didwarn = 1;
        }
      }
    }
    this->glrectangle = FALSE;
    if (!coin_is_power_of_two(this->glcontextsize[0]) ||
        !coin_is_power_of_two(this->glcontextsize[1])) {
      // we only get here if the OpenGL driver can handle non power of
      // two textures/pbuffers.
      this->glrectangle = TRUE;
    }

    // FIXME: make it possible to specify what kind of context you want
    // (RGB or RGBA, I guess). pederb, 2003-11-27
    this->glcontext = cc_glglue_context_create_offscreen(this->glcontextsize[0],
                                                         this->glcontextsize[1]);
    // FIXME: test if we actually got a pbuffer. pederb, 2003-11-27

    if (this->glaction) delete this->glaction;
    this->glaction = new SoGLRenderAction(SbViewportRegion(this->glcontextsize));
    this->glaction->addPreRenderCallback(SoSceneTexture2P::prerendercb, (void*) this);
    this->glimagevalid = FALSE;
  }

  if (!this->pbuffervalid) {
    assert(this->glaction != NULL);
    assert(this->glcontext != NULL);
    this->glaction->setTransparencyType((SoGLRenderAction::TransparencyType)
                                        SoShapeStyleElement::getTransparencyType(state));

    cc_glglue_context_make_current(this->glcontext);
    glEnable(GL_DEPTH_TEST);
    this->glaction->apply(scene);
    // Make sure that rendering to pBuffer is completed to avoid
    // flickering. DON'T REMOVE THIS. You have been warned.
    glFlush();

    if (!this->canrendertotexture) {
      SbVec2s size = this->glcontextsize;
      int reqbytes = size[0]*size[1]*4;
      if (reqbytes > this->offscreenbuffersize) {
        delete[] this->offscreenbuffer;
        this->offscreenbuffer = new unsigned char[reqbytes];
        this->offscreenbuffersize = reqbytes;
      }
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glReadPixels(0, 0, size[0], size[1], GL_RGBA, GL_UNSIGNED_BYTE,
                   this->offscreenbuffer);
      glPixelStorei(GL_PACK_ALIGNMENT, 4);
    }
    cc_glglue_context_reinstate_previous(this->glcontext);
  }
  if (!this->glimagevalid || (this->glimage == NULL)) {
    // just delete old glimage
    if (this->glimage) {
      this->glimage->unref(state);
      this->glimage = NULL;
    }
    this->glimage = new SoGLImage;
    uint32_t flags = this->glimage->getFlags();
    if (this->glrectangle) {
      flags |= SoGLImage::RECTANGLE;
    }
    switch ((SoSceneTexture2::TransparencyFunction) (PUBLIC(this)->transparencyFunction.getValue())) {
    case SoSceneTexture2::NONE:
      flags |= SoGLImage::FORCE_TRANSPARENCY_FALSE;
      break;
    case SoSceneTexture2::ALPHA_BLEND:
      flags |= SoGLImage::FORCE_TRANSPARENCY_TRUE|SoGLImage::FORCE_ALPHA_TEST_TRUE;
      break;
    case SoSceneTexture2::ALPHA_TEST:
      flags |= SoGLImage::FORCE_TRANSPARENCY_TRUE;
      break;
    default:
      assert(0 && "should not get here");
      break;
    }
    this->glimage->setFlags(flags);

    if (this->canrendertotexture) {
      // bind texture to pbuffer
      this->glimage->setPBuffer(state, this->glcontext,
                                translateWrap((SoSceneTexture2::Wrap)PUBLIC(this)->wrapS.getValue()),
                                translateWrap((SoSceneTexture2::Wrap)PUBLIC(this)->wrapT.getValue()),
                                quality);
    }
  }
  if (!this->canrendertotexture) {
    assert(this->glimage);
    assert(this->offscreenbuffer);
    this->glimage->setData(this->offscreenbuffer,
                           this->glcontextsize,
                           4,
                           translateWrap((SoSceneTexture2::Wrap)PUBLIC(this)->wrapS.getValue()),
                           translateWrap((SoSceneTexture2::Wrap)PUBLIC(this)->wrapT.getValue()),
                           quality);
  }
  this->glimagevalid = TRUE;
  this->pbuffervalid = TRUE;
}

void
SoSceneTexture2P::prerendercb(void * userdata, SoGLRenderAction * action)
{
  SoSceneTexture2 * thisp = (SoSceneTexture2*) userdata;
  SbVec4f col = thisp->clearColor.getValue();
  glClearColor(col[0], col[1], col[2], col[3]);
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

#undef PUBLIC

#undef LOCK_GLIMAGE
#undef UNLOCK_GLIMAGE

// **************************************************************
