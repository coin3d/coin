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
#include <Inventor/nodes/SoSceneTexture2.h>

#include <assert.h>
#include <string.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/misc/SoNotification.h>


// SoTexture2 includes
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
#include <Inventor/misc/SoGLImage.h>

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
  void updatePBuffer(SoState * state, const float quality);
  SoGLRenderAction * glaction;
  static void prerendercb(void * userdata, SoGLRenderAction * action);

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
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

void
SoSceneTexture2::initClass(void)
{
  SO_NODE_INIT_CLASS(SoSceneTexture2, SoNode, "Node");

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

  SO_NODE_CONSTRUCTOR(SoSceneTexture2);
  SO_NODE_ADD_FIELD(size, (256, 256));
  SO_NODE_ADD_FIELD(scene, (NULL));

  SO_NODE_ADD_FIELD(wrapS, (SoTexture2::REPEAT));
  SO_NODE_ADD_FIELD(wrapT, (SoTexture2::REPEAT));
  SO_NODE_ADD_FIELD(model, (MODULATE));
  SO_NODE_ADD_FIELD(blendColor, (0.0f, 0.0f, 0.0f));

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, SoTexture2::Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, SoTexture2::Wrap);

  SO_NODE_DEFINE_ENUM_VALUE(Model, MODULATE);
  SO_NODE_DEFINE_ENUM_VALUE(Model, DECAL);
  SO_NODE_DEFINE_ENUM_VALUE(Model, BLEND);
  SO_NODE_DEFINE_ENUM_VALUE(Model, REPLACE);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);
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
        SoDebugError::postWarning("SoTexture2::GLRender",
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
    SoGLTextureEnabledElement::set(state,
                                   PRIVATE(this)->glimage != NULL &&
                                   quality > 0.0f);
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

// doc from parent
void
SoSceneTexture2::callback(SoCallbackAction * action)
{
  SoSceneTexture2::doAction(action);
}

// doc from parent
void
SoSceneTexture2::rayPick(SoRayPickAction * action)
{
  SoSceneTexture2::doAction(action);
}

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
           f == &this->model) {
    // no need to render scene again, but update the texture object
    PRIVATE(this)->glimagevalid = FALSE;
  }
  inherited::notify(list);
}

// doc from parent
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
}

SoSceneTexture2P::~SoSceneTexture2P()
{
  if (this->glimage) this->glimage->unref(NULL);
  if (this->glcontext != NULL) {
    cc_glglue_context_destruct(this->glcontext);
  }
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

  if (this->glcontext == NULL) {
    // FIXME: make it possible to specify what kind of context you want
    // (RGB or RGBA, I guess). pederb, 2003-11-27
    this->glcontext = cc_glglue_context_create_offscreen(size[0], size[1]);
    // FIXME: test if we actually got a pbuffer. pederb, 2003-11-27
    this->glcontextsize = size;
    
    if (this->glaction) delete this->glaction;
    this->glaction = new SoGLRenderAction(SbViewportRegion(size));
    this->glaction->addPreRenderCallback(SoSceneTexture2P::prerendercb, (void*) this);
    this->glimagevalid = FALSE;
  }
  
  if (!this->pbuffervalid) {
    assert(this->glaction != NULL);
    assert(this->glcontext != NULL);
    this->glaction->setTransparencyType((SoGLRenderAction::TransparencyType)
                                        SoShapeStyleElement::getTransparencyType(state));
    
    cc_glglue_context_make_current(this->glcontext);  
    this->glaction->apply(scene);
    cc_glglue_context_reinstate_previous(this->glcontext);
  }
  if (!this->glimagevalid || (this->glimage == NULL)) {
    // just delete old glimage
    if (this->glimage) {
      this->glimage->unref(state);
      this->glimage = NULL;
    }
    this->glimage = new SoGLImage;
    // bind texture to pbuffer
    this->glimage->setPBuffer(state, this->glcontext,
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
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

#undef PUBLIC

#undef LOCK_GLIMAGE
#undef UNLOCK_GLIMAGE

// **************************************************************
