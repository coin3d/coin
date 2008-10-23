/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/SoRenderManager.h>

#include <algorithm>

#include <Inventor/system/gl.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/SoDB.h>

#include "tidbitsp.h"
#include "misc/AudioTools.h"
#include "misc/SoRenderManagerP.h"

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

SoRenderManager::SoRenderManager(void)
{
  assert(SoDB::isInitialized() && "SoDB::init() has not been invoked");

  PRIVATE(this) = new SoRenderManagerP(this);

  PRIVATE(this)->dummynode = new SoInfo;
  PRIVATE(this)->dummynode->ref();  

  PRIVATE(this)->rootsensor = NULL;
  PRIVATE(this)->scene = NULL;
  PRIVATE(this)->camera = NULL;
  PRIVATE(this)->rendercb = NULL;
  PRIVATE(this)->rendercbdata = NULL;
  PRIVATE(this)->redrawshot = NULL;
  PRIVATE(this)->stereostencilmask = NULL;
  PRIVATE(this)->superimpositions = NULL;

  PRIVATE(this)->doublebuffer = TRUE;
  PRIVATE(this)->deleteaudiorenderaction = TRUE;
  PRIVATE(this)->deleteglaction = TRUE;
  PRIVATE(this)->isactive = TRUE;
  PRIVATE(this)->texturesenabled = TRUE;

  PRIVATE(this)->nearplanevalue = 0.6f;
  PRIVATE(this)->stereooffset = 1.0f;
  PRIVATE(this)->isrgbmode = TRUE;
  PRIVATE(this)->backgroundcolor.setValue(0.0f, 0.0f, 0.0f, 0.0f);
  PRIVATE(this)->backgroundindex = 0;
  PRIVATE(this)->overlaycolor = SbColor(1.0f, 0.0f, 0.0f);
  PRIVATE(this)->stereostencilmaskvp = SbViewportRegion(0, 0);

  PRIVATE(this)->stereostenciltype = SoRenderManager::MONO;
  PRIVATE(this)->rendermode = SoRenderManager::AS_IS;
  PRIVATE(this)->stereomode = SoRenderManager::MONO;
  PRIVATE(this)->autoclipping = SoRenderManager::NO_AUTO_CLIPPING;
  PRIVATE(this)->redrawpri = SoRenderManager::getDefaultRedrawPriority();

  PRIVATE(this)->glaction = new SoGLRenderAction(SbViewportRegion(400, 400));
  PRIVATE(this)->audiorenderaction = new SoAudioRenderAction;

  PRIVATE(this)->clipsensor = 
    new SoNodeSensor(SoRenderManagerP::updateClippingPlanesCB, PRIVATE(this));
  PRIVATE(this)->clipsensor->setPriority(this->getRedrawPriority() - 1);

}

SoRenderManager::~SoRenderManager()
{
  PRIVATE(this)->dummynode->unref();

  if (PRIVATE(this)->deleteglaction) delete PRIVATE(this)->glaction;
  if (PRIVATE(this)->deleteaudiorenderaction) delete PRIVATE(this)->audiorenderaction;
  if (PRIVATE(this)->rootsensor) delete PRIVATE(this)->rootsensor;
  if (PRIVATE(this)->redrawshot) delete PRIVATE(this)->redrawshot;

  if (PRIVATE(this)->superimpositions != NULL) {
    while (PRIVATE(this)->superimpositions->getLength() > 0) {
      this->removeSuperimposition((Superimposition *)(*PRIVATE(this)->superimpositions)[0]);
    }
    delete PRIVATE(this)->superimpositions;
  }

  delete PRIVATE(this)->clipsensor;

  if (PRIVATE(this)->scene) 
    PRIVATE(this)->scene->unref();
  this->setCamera(NULL);

  delete PRIVATE(this);
}

/*!
  Set the node which is top of the scene graph we're managing.  The \a
  sceneroot node reference count will be increased by 1, and any
  previously set scene graph top node will have it's reference count
  decreased by 1.

  \sa getSceneGraph()
*/
void
SoRenderManager::setSceneGraph(SoNode * const sceneroot)
{
  this->detachClipSensor();
  this->detachRootSensor();
  // Don't unref() until after we've set up the new root, in case the
  // old root == the new sceneroot. (Just to be that bit more robust.)
  SoNode * oldroot = PRIVATE(this)->scene;
  
  PRIVATE(this)->scene = sceneroot;
  
  if (PRIVATE(this)->scene) {
    PRIVATE(this)->scene->ref();
    // FIXME: WTF? Did someone forget to read the coding guidelines? kintel 20080729
    //PRIVATE(this)->camera = PRIVATE(this)->searchForCamera(PRIVATE(this)->scene);
    
    this->attachRootSensor(PRIVATE(this)->scene);
    this->attachClipSensor(PRIVATE(this)->scene);
  }
  
  if (oldroot) oldroot->unref();
}

/*!
  Returns pointer to root of scene graph.
 */
SoNode *
SoRenderManager::getSceneGraph(void) const
{
  return PRIVATE(this)->scene;
}

/*!  
  Sets the camera to be used.
*/
void 
SoRenderManager::setCamera(SoCamera * camera)
{
  if (PRIVATE(this)->camera) {
    PRIVATE(this)->camera->unref();
  }
  PRIVATE(this)->camera = camera;
  if (camera) camera->ref();
}

/*!
  Returns the current camera.
*/
SoCamera * 
SoRenderManager::getCamera(void) const
{
  return PRIVATE(this)->camera;
}

// Internal callback.
void
SoRenderManager::nodesensorCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoRenderManager::nodesensorCB",
                         "detected change in scene graph");
#endif // debug
  ((SoRenderManager *)data)->scheduleRedraw();
}

void 
SoRenderManager::attachRootSensor(SoNode * const sceneroot)
{
  if (!PRIVATE(this)->rootsensor) {
    (SoRenderManagerRootSensor::debug()) ?
      PRIVATE(this)->rootsensor = new SoRenderManagerRootSensor(SoRenderManager::nodesensorCB, this):
      PRIVATE(this)->rootsensor = new SoNodeSensor(SoRenderManager::nodesensorCB, this);
  }
  PRIVATE(this)->rootsensor->attach(sceneroot);
}

void 
SoRenderManager::detachRootSensor(void)
{
  if (PRIVATE(this)->rootsensor) {
    PRIVATE(this)->rootsensor->detach();
  }
}

void 
SoRenderManager::attachClipSensor(SoNode * const sceneroot)
{
  PRIVATE(this)->clipsensor->attach(sceneroot);
  if (PRIVATE(this)->autoclipping != SoRenderManager::NO_AUTO_CLIPPING) {
    PRIVATE(this)->clipsensor->schedule();
  }
}

void 
SoRenderManager::detachClipSensor(void)
{
  if (PRIVATE(this)->clipsensor->isScheduled()) {
    PRIVATE(this)->clipsensor->unschedule();
  }
  if (PRIVATE(this)->clipsensor->getAttachedNode()) {
    PRIVATE(this)->clipsensor->detach();
  }
}

void 
SoRenderManager::clearBuffers(SbBool color, SbBool depth)
{
  GLbitfield mask = 0;
  if (color) mask |= GL_COLOR_BUFFER_BIT;
  if (depth) mask |= GL_DEPTH_BUFFER_BIT;
  const SbColor4f bgcol = PRIVATE(this)->backgroundcolor;
  glClearColor(bgcol[0], bgcol[1], bgcol[2], bgcol[3]);
  glClear(mask);
}

void
SoRenderManager::prerendercb(void * userdata, SoGLRenderAction * action)
{
  // remove callback again
  action->removePreRenderCallback(prerendercb, userdata);
  // MSVC7 on 64-bit Window wants it to go through this cast.
  const uintptr_t bitfield = (uintptr_t)userdata;
  GLbitfield mask = (GLbitfield)bitfield;

#if COIN_DEBUG && 0 // debug
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  SoDebugError::postInfo("SoRenderManager::prerendercb",
                         "GL_VIEWPORT=<%d, %d, %d, %d>",
                         view[0], view[1], view[2], view[3]);
#endif // debug
  
  // clear the viewport
  glClear(mask);
}

/*!

 */
SoRenderManager::Superimposition *
SoRenderManager::addSuperimposition(SoNode * scene,
                                    uint32_t flags)
{
  if (!PRIVATE(this)->superimpositions) {
    PRIVATE(this)->superimpositions = new SbPList;
  }
  Superimposition * s = new Superimposition(scene, TRUE, this, flags);
  PRIVATE(this)->superimpositions->append(s);
  return s;
}

/*!

 */
void
SoRenderManager::removeSuperimposition(Superimposition * s)
{
  int idx = -1;
  if (!PRIVATE(this)->superimpositions) goto error;
  if ((idx = PRIVATE(this)->superimpositions->find(s)) == -1) goto error;

  PRIVATE(this)->superimpositions->remove(idx);
  delete s;
  return;

 error:
#if COIN_DEBUG
  SoDebugError::post("SoSceneManager::removeSuperimposition",
                     "no such superimposition");
#endif // COIN_DEBUG
  return;
}


/*!
  Render the scene graph.

  If \a clearwindow is \c TRUE, clear the rendering buffer before
  drawing. If \a clearzbuffer is \c TRUE, clear the depth buffer
  values before rendering. Both of these arguments should normally be
  \c TRUE, but they can be set to \c FALSE to optimize for special
  cases (e.g. when doing wireframe rendering one doesn't need a depth
  buffer).
 */
void
SoRenderManager::render(const SbBool clearwindow, const SbBool clearzbuffer)
{
  // FIXME: according to a user, TGS Inventor seems to disable the
  // redraw SoOneShotSensor while the scene graph is being rendered,
  // which Coin does not do. SGI Inventor probably has the same
  // behavior as TGS Inventor. (Should investigate this.)
  //
  // pederb suggests keeping the current behavior in Coin, even though
  // this may cause trouble for code being ported from SGI / TGS
  // Inventor, as it is convenient to "touch()" a node for triggering
  // continuous animation. Besides, making Coin compatible with SGI
  // (?) / TGS Inventor now may cause problems for existing Coin
  // client code.
  //
  // I'm however not too happy with this fairly large incompatibility.
  // Any usable suggestions for a resolution of this problem would be
  // welcome.
  //
  // 20050809 mortene.

  if (PRIVATE(this)->scene && 
      // Order is important below, because we don't want to call
      // SoAudioDevice::instance() unless we need to -- as it triggers
      // loading the OpenAL library, which should only be loaded on
      // demand.
      coin_sound_should_traverse() &&
      SoAudioDevice::instance()->haveSound() &&
      SoAudioDevice::instance()->isEnabled())
    PRIVATE(this)->audiorenderaction->apply(PRIVATE(this)->scene);

  this->render(PRIVATE(this)->glaction, TRUE, clearwindow, clearzbuffer);
}


void
SoRenderManager::render(SoGLRenderAction * action,
                        const SbBool initmatrices,
                        const SbBool clearwindow_in,
                        const SbBool clearzbuffer)
{
  SbBool clearwindow = clearwindow_in; // make sure we only clear the color buffer once
  PRIVATE(this)->invokePreRenderCallbacks();

  if (PRIVATE(this)->superimpositions) {
    for (int i = 0; i < PRIVATE(this)->superimpositions->getLength(); i++) {
      Superimposition * s = (Superimposition *) (*PRIVATE(this)->superimpositions)[i];
      if (s->getStateFlags() & Superimposition::BACKGROUND) {
        s->render(action, clearwindow);
        clearwindow = FALSE;
      }
    }
  }

  (this->getStereoMode() == SoRenderManager::MONO) ?
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer):
    this->renderStereo(action, initmatrices, clearwindow, clearzbuffer);
  
  if (PRIVATE(this)->superimpositions) {
    for (int i = 0; i < PRIVATE(this)->superimpositions->getLength(); i++) {
      Superimposition * s = (Superimposition *) (*PRIVATE(this)->superimpositions)[i];
      if (!(s->getStateFlags() & Superimposition::BACKGROUND)) {
        s->render(action);
      }
    }
  }

  PRIVATE(this)->invokePostRenderCallbacks();
}

void
SoRenderManager::actuallyRender(SoGLRenderAction * action,
                                const SbBool initmatrices,
                                const SbBool clearwindow,
                                const SbBool clearzbuffer)
{
  GLbitfield mask = 0;
  if (clearwindow) mask |= GL_COLOR_BUFFER_BIT;
  if (clearzbuffer) mask |= GL_DEPTH_BUFFER_BIT;
  
  if (initmatrices) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  // If there has been changes in the scene graph leading to a node
  // sensor detect and schedule before we've gotten around to serving
  // the current redraw -- remove it. This will prevent infinite loops
  // in the case of scenegraph modifications between a nodesensor
  // trigger and SoSceneManager::render() actually being called. It
  // will also help us avoid "double redraws" at expose events.
  PRIVATE(this)->lock();
  if (PRIVATE(this)->rootsensor && PRIVATE(this)->rootsensor->isScheduled()) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "rootsensor unschedule");
#endif // debug
    PRIVATE(this)->rootsensor->unschedule();
  }
  PRIVATE(this)->unlock();
  // Apply the SoGLRenderAction to the scenegraph root.
  if (PRIVATE(this)->scene) {
    this->renderScene(action, PRIVATE(this)->scene, (uint32_t) mask);
  }

  // Automatically re-triggers rendering if any animation stuff is
  // connected to the realTime field.
  if (SoRenderManager::isRealTimeUpdateEnabled()) {
    // FIXME: it would be more elegant to use a private field class
    // inheriting SoSFTime ("SFRealTime") which could just be
    // touch()'ed, and which would do lazy reading of time-of-day on
    // demand. 20000316 mortene.
    SoField * realtime = SoDB::getGlobalField("realTime");
    if (realtime && (realtime->getTypeId() == SoSFTime::getClassTypeId())) {
      // Note that this should not get in the way of a
      // app-programmer controlled realTime field, as
      // enableRealTimeUpdate(FALSE) should then have been called.
      ((SoSFTime *)realtime)->setValue(SbTime::getTimeOfDay());
    }
  }
}

void 
SoRenderManager::renderScene(SoGLRenderAction * action, SoNode* scene, uint32_t clearmask)
{
  if (clearmask) {
    if (clearmask & GL_COLOR_BUFFER_BIT) {
      if (PRIVATE(this)->isrgbmode) {
        const SbColor4f bgcol = PRIVATE(this)->backgroundcolor;
        glClearColor(bgcol[0], bgcol[1], bgcol[2], bgcol[3]);
      }
      else {
        glClearIndex((GLfloat) PRIVATE(this)->backgroundindex);
      }
    }
    // Registering a callback is needed since the correct GL viewport
    // is set by SoGLRenderAction before rendering. It might not be
    // correct when we get here.
    // This callback is removed again in the prerendercb function
    action->addPreRenderCallback(this->prerendercb, (void*) (uintptr_t) clearmask);
  }
  action->apply(scene);
}

// render once in correct draw style
void 
SoRenderManager::renderSingle(SoGLRenderAction * action,
                              SbBool initmatrices,
                              SbBool clearwindow,
                              SbBool clearzbuffer)
{
  SoState * state = action->getState();
  state->push();

  SoNode * node = PRIVATE(this)->dummynode;

  if (!this->isTexturesEnabled()) {
    SoTextureQualityElement::set(state, node, 0.0f);
    SoTextureOverrideElement::setQualityOverride(state, TRUE);
  }
  switch (this->getRenderMode()) {
  case SoRenderManager::AS_IS:
    this->actuallyRender(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoRenderManager::WIREFRAME:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    this->actuallyRender(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoRenderManager::POINTS:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::POINTS);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    this->actuallyRender(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoRenderManager::HIDDEN_LINE:
    {
      // must clear before setting draw mask
      this->clearBuffers(TRUE, TRUE);

      // only draw into depth buffer
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      SoMaterialBindingElement::set(state, node, SoMaterialBindingElement::OVERALL);
      SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
      SoPolygonOffsetElement::set(state, node, 1.0f, 1.0f,
                                  SoPolygonOffsetElement::FILLED, TRUE);
      SoOverrideElement::setPolygonOffsetOverride(state, node, TRUE);
      SoOverrideElement::setLightModelOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, TRUE);
      this->actuallyRender(action, initmatrices, FALSE, FALSE);

      // reenable draw masks
      glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, FALSE);
      this->actuallyRender(action, initmatrices, FALSE, FALSE);
    }
    break;
  case SoRenderManager::WIREFRAME_OVERLAY:
      SoPolygonOffsetElement::set(state, node, 1.0f, 1.0f,
                                  SoPolygonOffsetElement::FILLED, TRUE);
      SoOverrideElement::setPolygonOffsetOverride(state, node, TRUE);
      this->actuallyRender(action, initmatrices, clearwindow, clearzbuffer);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      
      SoLazyElement::setDiffuse(state, node, 1, &PRIVATE(this)->overlaycolor, 
                                &PRIVATE(this)->colorpacker);
      SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
      SoMaterialBindingElement::set(state, node, SoMaterialBindingElement::OVERALL);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setLightModelOverride(state, node, TRUE);
      SoOverrideElement::setDiffuseColorOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, TRUE);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      this->actuallyRender(action, initmatrices, FALSE, FALSE);    
    break;

  case SoRenderManager::BOUNDING_BOX:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoComplexityTypeElement::set(state, node, SoComplexityTypeElement::BOUNDING_BOX);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    SoOverrideElement::setComplexityTypeOverride(state, node, TRUE);
    this->actuallyRender(action, initmatrices, clearwindow, clearzbuffer);
    break;
  default:
    assert(0 && "unknown rendering mode");
    break;
  }
  state->pop();
}

// render scene according to current stereo mode
void 
SoRenderManager::renderStereo(SoGLRenderAction * action,
                              SbBool initmatrices,
                              SbBool clearwindow,
                              SbBool clearzbuffer)
{
  if (!PRIVATE(this)->camera) return;

  this->clearBuffers(TRUE, TRUE);
  PRIVATE(this)->camera->setStereoAdjustment(PRIVATE(this)->stereooffset);
  
  SbBool stenciltestenabled = glIsEnabled(GL_STENCIL_TEST);

  // left eye
  PRIVATE(this)->camera->setStereoMode(SoCamera::LEFT_VIEW);

  switch (PRIVATE(this)->stereomode) {
  case SoRenderManager::ANAGLYPH:
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    this->renderSingle(action, initmatrices, FALSE, FALSE);
    break;
  case SoRenderManager::QUAD_BUFFER:
    glDrawBuffer(PRIVATE(this)->doublebuffer ? GL_BACK_LEFT : GL_FRONT_LEFT);
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoRenderManager::INTERLEAVED_ROWS:
  case SoRenderManager::INTERLEAVED_COLUMNS:
    this->initStencilBufferForInterleavedStereo();
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0x1, 0x1);
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
    break;
  default:
    assert(0 && "unknown stereo mode");
    break;
  }

  // right eye
  PRIVATE(this)->camera->setStereoMode(SoCamera::RIGHT_VIEW);

  switch (PRIVATE(this)->stereomode) {
  case SoRenderManager::ANAGLYPH:
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    this->renderSingle(action, initmatrices, FALSE, TRUE);
    break;
  case SoRenderManager::QUAD_BUFFER:
    glDrawBuffer(PRIVATE(this)->doublebuffer ? GL_BACK_RIGHT : GL_FRONT_RIGHT);
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoRenderManager::INTERLEAVED_ROWS:
  case SoRenderManager::INTERLEAVED_COLUMNS:
    glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
    this->renderSingle(action, initmatrices, FALSE, FALSE);
    break;
  default:
    assert(0 && "unknown stereo mode");
    break;
  }

  // restore / post render operations
  PRIVATE(this)->camera->setStereoMode(SoCamera::MONOSCOPIC);

  switch (PRIVATE(this)->stereomode) {
  case SoRenderManager::ANAGLYPH:
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    break;
  case SoRenderManager::QUAD_BUFFER:
    glDrawBuffer(PRIVATE(this)->doublebuffer ? GL_BACK : GL_FRONT);
    break;
  case SoRenderManager::INTERLEAVED_ROWS:
  case SoRenderManager::INTERLEAVED_COLUMNS:
    stenciltestenabled ?
      glEnable(GL_STENCIL_TEST) :
      glDisable(GL_STENCIL_TEST);
    break;
  default: 
    assert(0 && "unknown stereo mode"); 
    break;
  }
}

void
SoRenderManager::setAutoClipping(AutoClippingStrategy autoclipping)
{
  PRIVATE(this)->autoclipping = autoclipping;

  if (PRIVATE(this)->scene) {
    switch (autoclipping) {
    case SoRenderManager::NO_AUTO_CLIPPING:
      this->detachClipSensor();
      break;
    case SoRenderManager::FIXED_NEAR_PLANE:
    case SoRenderManager::VARIABLE_NEAR_PLANE:
      if (!PRIVATE(this)->clipsensor->getAttachedNode()) {
        PRIVATE(this)->clipsensor->attach(PRIVATE(this)->scene);
      }
      PRIVATE(this)->clipsensor->schedule();
      break;
    }
  }
}

void
SoRenderManager::initStencilBufferForInterleavedStereo(void)
{
  const SbViewportRegion & currentvp = PRIVATE(this)->glaction->getViewportRegion();
  if (PRIVATE(this)->stereostencilmaskvp == currentvp) { return; } // the common case

  SoRenderManager::StereoMode s = PRIVATE(this)->stereomode;
  assert((s == SoRenderManager::INTERLEAVED_ROWS) ||
         (s == SoRenderManager::INTERLEAVED_COLUMNS));

  // Find out whether or not we need to regenerate the mask data.
  SbBool allocnewmask = (PRIVATE(this)->stereostencilmask == NULL);

  const SbVec2s neworigin = currentvp.getViewportOriginPixels();
  const SbVec2s newsize = currentvp.getViewportSizePixels();

  const SbVec2s oldorigin = PRIVATE(this)->stereostencilmaskvp.getViewportOriginPixels();
  const SbVec2s oldsize = PRIVATE(this)->stereostencilmaskvp.getViewportSizePixels();

  allocnewmask = allocnewmask ||
    ((oldsize[0] + 7) / 8 * oldsize[1]) < ((newsize[0] + 7) / 8 * newsize[1]);

  const SbBool fillmask = allocnewmask || (PRIVATE(this)->stereostenciltype != s) ||
    ((s == SoRenderManager::INTERLEAVED_ROWS) && (oldsize[0] != newsize[0]));

  const SbBool layoutchange = !(PRIVATE(this)->stereostencilmaskvp == currentvp);

  const short bytewidth = (newsize[0] + 7) / 8;

  if (allocnewmask) {
    delete[] PRIVATE(this)->stereostencilmask;
    PRIVATE(this)->stereostencilmask = new GLubyte[bytewidth * newsize[1]];
  }

  PRIVATE(this)->stereostencilmaskvp = currentvp;

  if (fillmask) {
    GLubyte * mask = PRIVATE(this)->stereostencilmask;

    if (s == SoRenderManager::INTERLEAVED_COLUMNS) {
      // alternating columns of 0's and 1's
      (void)memset(mask, 0x55, bytewidth * newsize[1]);
    }
    else {
      // alternating rows of 0's and 1's
      for (short h=0; h < newsize[1]; h++) {
        const GLubyte fill = (h % 2) ? 0xff : 0x00;
        (void)memset(mask + (h * bytewidth), fill, bytewidth);
      }
    }

    PRIVATE(this)->stereostenciltype = s;
  }

  if (layoutchange) {
    glClearStencil(0x0);

    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, GL_REPLACE, GL_REPLACE);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glViewport(neworigin[0], neworigin[1], newsize[0], newsize[1]);

    glOrtho(0, newsize[0], 0, newsize[1], -1.0f, 1.0f);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // FIXME: I've noticed a problem with this approach. If there is
    // something in the window system obscuring part of the canvas
    // while this is called (as could e.g. happen with a size
    // indicator, as with the Sawfish window manager), the stencil
    // mask will not be set up for that part. 20041019 mortene.
    //
    // UPDATE 20041019 mortene: discussed this with pederb, and we
    // believe this may be due to a bug in either the OpenGL driver
    // (Nvidia 61.11, Linux) or window system or manager (Sawfish,
    // XFree86 v4.1.0.1). Should test on other systems to see if they
    // show the same artifact.

    glRasterPos2f(0, 0);
    glDrawPixels(newsize[0], newsize[1], GL_STENCIL_INDEX, GL_BITMAP,
                 PRIVATE(this)->stereostencilmask);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

/*!
  Reinitialize after parameters affecting the OpenGL context has
  changed.
*/
void
SoRenderManager::reinitialize(void)
{
  PRIVATE(this)->glaction->invalidateState();
}

/*!
  Redraw at first opportunity as system becomes idle.

  Multiple calls to this method before an actual redraw has taken
  place will only result in a single redraw of the scene.
*/
void
SoRenderManager::scheduleRedraw(void)
{
  PRIVATE(this)->lock();
  if (this->isActive() && PRIVATE(this)->rendercb) {
    if (!PRIVATE(this)->redrawshot) {
      PRIVATE(this)->redrawshot = 
        new SoOneShotSensor(SoRenderManagerP::redrawshotTriggeredCB, this);
      PRIVATE(this)->redrawshot->setPriority(this->getRedrawPriority());
    }

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoRenderManager::scheduleRedraw",
                           "scheduling redrawshot (oneshotsensor) %p",
                           PRIVATE(this)->redrawshot);
#endif // debug
    PRIVATE(this)->redrawshot->schedule();
  }
  PRIVATE(this)->unlock();
}

/*!
  Update window size of our SoGLRenderAction's viewport settings.

  Note that this will \e only change the information about window
  dimensions, the actual viewport size and origin (ie the rectangle
  which redraws are confined to) will stay the same.

  \sa setViewportRegion()
*/
void
SoRenderManager::setWindowSize(const SbVec2s & newsize)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoRenderManager::setWindowSize",
                         "(%d, %d)", newsize[0], newsize[1]);
#endif // debug

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  PRIVATE(this)->glaction->setViewportRegion(region);

    // FIXME: WTF? Did someone forget to read the coding guidelines? kintel 20080729
//   region = PRIVATE(this)->handleeventaction->getViewportRegion();
//   region.setWindowSize(newsize[0], newsize[1]);
//   PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Returns the current render action window size.

  \sa setWindowSize()
*/
const SbVec2s &
SoRenderManager::getWindowSize(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getWindowSize();
}

/*!
  Set size of rendering area for the viewport within the current
  window.
*/
void
SoRenderManager::setSize(const SbVec2s & newsize)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoRenderManager::setSize",
                         "(%d, %d)", newsize[0], newsize[1]);
#endif // debug

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  PRIVATE(this)->glaction->setViewportRegion(region);
}

/*!
  Returns size of render area.
 */
const SbVec2s &
SoRenderManager::getSize(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getViewportSizePixels();
}

/*!
  Set \e only the origin of the viewport region within the rendering
  window.

  \sa setViewportRegion(), setWindowSize()
*/
void
SoRenderManager::setOrigin(const SbVec2s & newOrigin)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoRenderManager::setOrigin",
                         "(%d, %d)", newOrigin[0], newOrigin[1]);
#endif // debug

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  PRIVATE(this)->glaction->setViewportRegion(region);
}

/*!
  Returns origin of rendering area viewport.

  \sa setOrigin()
*/
const SbVec2s &
SoRenderManager::getOrigin(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getViewportOriginPixels();
}

/*!
  Update our SoGLRenderAction's viewport settings.

  This will change \e both the information about window dimensions and
  the actual viewport size and origin.

  \sa setWindowSize()
*/
void
SoRenderManager::setViewportRegion(const SbViewportRegion & newregion)
{
  PRIVATE(this)->glaction->setViewportRegion(newregion);
}

/*!
  Returns current viewport region used by the renderaction and the
  event handling.

  \sa setViewportRegion()
*/
const SbViewportRegion &
SoRenderManager::getViewportRegion(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion();
}

/*!
  Sets color of rendering canvas.
 */
void
SoRenderManager::setBackgroundColor(const SbColor4f & color)
{
  PRIVATE(this)->backgroundcolor = color;
}

/*!
  Returns color used for clearing the rendering area before rendering
  the scene.
 */
const SbColor4f &
SoRenderManager::getBackgroundColor(void) const
{
  return PRIVATE(this)->backgroundcolor;
}

/*!
  Set index of background color in the color lookup table if rendering
  in colorindex mode.

  Note: colorindex mode is not supported yet in Coin.
 */
void
SoRenderManager::setBackgroundIndex(const int index)
{
  PRIVATE(this)->backgroundindex = index;
}

/*!
  Returns index of colormap for background filling.

  \sa setBackgroundIndex()
 */
int
SoRenderManager::getBackgroundIndex(void) const
{
  return PRIVATE(this)->backgroundindex;
}

/*!
  Turn RGB truecolor mode on or off. If you turn truecolor mode off,
  colorindex mode will be used instead.
*/
void
SoRenderManager::setRGBMode(const SbBool yes)
{
  PRIVATE(this)->isrgbmode = yes;
}

/*!
  Returns the "truecolor or colorindex" mode flag.
 */
SbBool
SoRenderManager::isRGBMode(void) const
{
  return PRIVATE(this)->isrgbmode;
}

/*!
  Tell the scenemanager that double buffering is used
 */
void 
SoRenderManager::setDoubleBuffer(const SbBool enable)
{
  PRIVATE(this)->doublebuffer = enable;
}

/*!
  returns if the scenemanager is double buffered
 */
SbBool 
SoRenderManager::isDoubleBuffer(void) const
{
  return PRIVATE(this)->doublebuffer;
}

/*!
  Set the callback function \a f to invoke when rendering the
  scene. \a userdata will be passed as the first argument of the
  function.
 */
void
SoRenderManager::setRenderCallback(SoRenderManagerRenderCB * f,
                                  void * const userdata)
{
  PRIVATE(this)->rendercb = f;
  PRIVATE(this)->rendercbdata = userdata;
}

/*!
  Activate rendering and event handling. Default is \c off.
 */
void
SoRenderManager::activate(void)
{
  PRIVATE(this)->isactive = TRUE;
}

/*!
  Deactive rendering and event handling.
 */
void
SoRenderManager::deactivate(void)
{
  PRIVATE(this)->isactive = FALSE;
}

/*!
  Returns the \e active flag.
 */
int
SoRenderManager::isActive(void) const
{
  return PRIVATE(this)->isactive;
}

/*!
  Do an immediate redraw by calling the redraw callback function.
 */
void
SoRenderManager::redraw(void)
{
  if (PRIVATE(this)->rendercb) {
    PRIVATE(this)->rendercb(PRIVATE(this)->rendercbdata, this);
  }
}

/*!
  Returns \c TRUE if the SoSceneManager automatically redraws the
  scene upon detecting changes in the scene graph.

  The automatic redraw is turned on and off by setting either a valid
  callback function with setRenderCallback(), or by passing \c NULL.
 */
SbBool
SoRenderManager::isAutoRedraw(void) const
{
  return PRIVATE(this)->rendercb != NULL;
}


/*!
  Sets the render mode.
*/
void 
SoRenderManager::setRenderMode(const RenderMode mode)
{
  PRIVATE(this)->rendermode = mode;
  PRIVATE(this)->dummynode->touch();
}

/*!
  Returns the current render mode.
*/
SoRenderManager::RenderMode 
SoRenderManager::getRenderMode(void) const
{
  return PRIVATE(this)->rendermode;
}

/*!
  Sets the stereo mode.
*/
void 
SoRenderManager::setStereoMode(const StereoMode mode)
{
  PRIVATE(this)->stereomode = mode;
  PRIVATE(this)->dummynode->touch();
}

/*!
  Returns the current stereo mode.
*/
SoRenderManager::StereoMode 
SoRenderManager::getStereoMode(void) const
{
  return PRIVATE(this)->stereomode;
}

/*!
  Sets the stereo offset used when doing stereo rendering.
*/
void 
SoRenderManager::setStereoOffset(const float offset)
{
  PRIVATE(this)->stereooffset = offset;
  PRIVATE(this)->dummynode->touch();
}

/*!
  Returns the current stereo offset.
*/
float 
SoRenderManager::getStereoOffset(void) const
{
  return PRIVATE(this)->stereooffset;
}

/*!
  Turn antialiased rendering on or off.

  See documentation for SoGLRenderAction::setSmoothing() and
  SoGLRenderAction::setNumPasses().
 */
void
SoRenderManager::setAntialiasing(const SbBool smoothing, const int numpasses)
{
  PRIVATE(this)->glaction->setSmoothing(smoothing);
  PRIVATE(this)->glaction->setNumPasses(numpasses);
}

/*!
  Returns rendering pass information.

  \sa setAntialiasing()
 */
void
SoRenderManager::getAntialiasing(SbBool & smoothing, int & numpasses) const
{
  smoothing = PRIVATE(this)->glaction->isSmoothing();
  numpasses = PRIVATE(this)->glaction->getNumPasses();
}

/*!
  Set the \a action to use for rendering. Overrides the default action
  made in the constructor.
 */
void
SoRenderManager::setGLRenderAction(SoGLRenderAction * const action)
{
  SbBool haveregion = FALSE;
  SbViewportRegion region;
  if (PRIVATE(this)->glaction) { // remember existing viewport region
    region = PRIVATE(this)->glaction->getViewportRegion();
    haveregion = TRUE;
  }
  if (PRIVATE(this)->deleteglaction) {
    delete PRIVATE(this)->glaction;
    PRIVATE(this)->glaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  // Note that the SGI and TGS Inventor implementations doesn't do
  // this -- which smells of a bug.
  if (action && action != PRIVATE(this)->glaction) action->invalidateState();
  PRIVATE(this)->glaction = action;
  PRIVATE(this)->deleteglaction = FALSE;
  if (PRIVATE(this)->glaction && haveregion)
    PRIVATE(this)->glaction->setViewportRegion(region);
}

/*!
  Returns pointer to render action.
 */
SoGLRenderAction *
SoRenderManager::getGLRenderAction(void) const
{
  return PRIVATE(this)->glaction;
}

/*!
  This method returns the current autoclipping strategy.

  \sa setAutoClipping
*/

SoRenderManager::AutoClippingStrategy
SoRenderManager::getAutoClipping(void) const
{
  return PRIVATE(this)->autoclipping;
}

/*!
  When the NbSceneManager::FIXED_NEAR_PLANE autoclipping strategy is
  used, you set the value of the near plane distance with this method.

  \sa setAutoClipping, getNearPlaneValue, NbSceneManager::AutoClippingStrategy
*/

void
SoRenderManager::setNearPlaneValue(float value)
{
  PRIVATE(this)->nearplanevalue = value;
}

/*!
  This method returns the near plane distance value that will be used
  when the NbSceneManager::FIXED_NEAR_PLANE autoclipping strategy is used.

  Default value is 0.6.

  \sa setAutoClipping, setNearPlaneValue,  NbSceneManager::AutoClippingStrategy
*/

float
SoRenderManager::getNearPlaneValue(void) const
{
  return PRIVATE(this)->nearplanevalue;
}

/*!
  Enable/disable textures when rendering.
  Defaults to TRUE.

  \sa isTexturesEnabled
*/

void 
SoRenderManager::setTexturesEnabled(const SbBool onoff)
{
  PRIVATE(this)->texturesenabled = onoff;
}

/*!
  Returns whether textures are enabled or not.

  \sa setTexturesEnabled
*/

SbBool 
SoRenderManager::isTexturesEnabled(void) const
{
  return PRIVATE(this)->texturesenabled;
}

/*!
  Set up the redraw \a priority for the SoOneShotSensor used to
  trigger redraws. By setting this lower than for your own sensors,
  you can make sure some code is always run before redraw happens.

  \sa SoDelayQueueSensor
 */
void
SoRenderManager::setRedrawPriority(const uint32_t priority)
{
  PRIVATE(this)->redrawpri = priority;

  if (PRIVATE(this)->redrawshot) PRIVATE(this)->redrawshot->setPriority(priority);
}

/*!
  Returns value of priority on the redraw sensor.
 */
uint32_t
SoRenderManager::getRedrawPriority(void) const
{
  return PRIVATE(this)->redrawpri;
}

/*!
  Set the \a action to use for rendering audio. Overrides the default action
  made in the constructor.
 */
void
SoRenderManager::setAudioRenderAction(SoAudioRenderAction * const action)
{
  if (PRIVATE(this)->deleteaudiorenderaction) {
    delete PRIVATE(this)->audiorenderaction;
    PRIVATE(this)->audiorenderaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  if (action && action != PRIVATE(this)->audiorenderaction) action->invalidateState();
  PRIVATE(this)->audiorenderaction = action;
  PRIVATE(this)->deleteaudiorenderaction = FALSE;
}

/*!
  Returns pointer to audio render action.
 */
SoAudioRenderAction *
SoRenderManager::getAudioRenderAction(void) const
{
  return PRIVATE(this)->audiorenderaction;
}

/*!
  Returns the default priority of the redraw sensor.

  \sa SoDelayQueueSensor, setRedrawPriority()
 */
uint32_t
SoRenderManager::getDefaultRedrawPriority(void)
{
  return 10000;
}

/*!
  Set whether or not for SoSceneManager instances to "touch" the
  global \c realTime field after a redraw. If this is not done,
  redrawing when animating the scene will only happen as fast as the
  \c realTime interval goes (which defaults to 12 times a second).

  \sa SoDB::setRealTimeInterval()
 */
void
SoRenderManager::enableRealTimeUpdate(const SbBool flag)
{
  SoRenderManagerP::touchtimer = flag;
  if (!SoRenderManagerP::cleanupfunctionset) {
    coin_atexit((coin_atexit_f*) SoRenderManagerP::cleanup, CC_ATEXIT_NORMAL);
    SoRenderManagerP::cleanupfunctionset = TRUE;
  }
}

/*!
  Returns whether or not we automatically notifies everything
  connected to the \c realTime field after a redraw.
 */
SbBool
SoRenderManager::isRealTimeUpdateEnabled(void)
{
  return SoRenderManagerP::touchtimer;
}


void
SoRenderManager::addPreRenderCallback(SoRenderManagerRenderCB * cb, void * data)
{
  PRIVATE(this)->preRenderCallbacks.push_back(SoRenderManagerP::RenderCBTouple(cb, data));
}

void
SoRenderManager::removePreRenderCallback(SoRenderManagerRenderCB * cb, void * data)
{
  std::vector<SoRenderManagerP::RenderCBTouple>::iterator findit =
    std::find(PRIVATE(this)->preRenderCallbacks.begin(),
         PRIVATE(this)->preRenderCallbacks.end(),
         SoRenderManagerP::RenderCBTouple(cb, data));
  if (findit != PRIVATE(this)->preRenderCallbacks.end()) {
    PRIVATE(this)->preRenderCallbacks.erase(findit);
  }
}

void
SoRenderManagerP::invokePreRenderCallbacks(void)
{
  std::vector<RenderCBTouple>::const_iterator cbit =
    this->preRenderCallbacks.begin();
  while (cbit != this->preRenderCallbacks.end()) {
    cbit->first(cbit->second, PUBLIC(this));
    ++cbit;
  }
}

void
SoRenderManager::addPostRenderCallback(SoRenderManagerRenderCB * cb, void * data)
{
  PRIVATE(this)->postRenderCallbacks.push_back(SoRenderManagerP::RenderCBTouple(cb, data));
}

void
SoRenderManager::removePostRenderCallback(SoRenderManagerRenderCB * cb, void * data)
{
  std::vector<SoRenderManagerP::RenderCBTouple>::iterator findit =
    std::find(PRIVATE(this)->postRenderCallbacks.begin(),
         PRIVATE(this)->postRenderCallbacks.end(),
         SoRenderManagerP::RenderCBTouple(cb, data));
  if (findit != PRIVATE(this)->postRenderCallbacks.end()) {
    PRIVATE(this)->postRenderCallbacks.erase(findit);
  }
}

void
SoRenderManagerP::invokePostRenderCallbacks(void)
{
  std::vector<RenderCBTouple>::const_iterator cbit =
    this->postRenderCallbacks.begin();
  while (cbit != this->postRenderCallbacks.end()) {
    cbit->first(cbit->second, PUBLIC(this));
    ++cbit;
  }
}

#undef PRIVATE
#undef PUBLIC
