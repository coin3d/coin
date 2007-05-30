/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoSceneManagerP.h"

#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoGLRenderAction.h>
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
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoDB.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

SbBool SoSceneManagerP::touchtimer = TRUE;
SbBool SoSceneManagerP::cleanupfunctionset = FALSE;

SoSceneManagerP::SoSceneManagerP(SoSceneManager * publ) 
{
  PUBLIC(this) = publ;
  this->dummynode = new SoInfo;
  this->dummynode->ref();
  this->doublebuffer = TRUE;
  this->stereostencilmaskvp = SbViewportRegion(0, 0);
  this->stereostencilmask = NULL;
  this->stereostenciltype = SoSceneManager::MONO;
}

SoSceneManagerP::~SoSceneManagerP()
{
  this->dummynode->unref();
  if (this->camera) {
    this->camera->unref();
  }
}

void 
SoSceneManagerP::clearBuffers(SbBool color, SbBool depth) 
{
  const SbColor bgcol = PUBLIC(this)->getBackgroundColor();
  GLbitfield mask = 0;
  if (color) mask |= GL_COLOR_BUFFER_BIT;
  if (depth) mask |= GL_DEPTH_BUFFER_BIT;
  glClearColor(bgcol[0], bgcol[1], bgcol[2], 0.0f);
  glClear(mask);
}

// Internal callback.
void
SoSceneManagerP::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "start");
#endif // debug

  SoSceneManager * sm = (SoSceneManager *)data;

  // Need to recheck the "active" flag, as it could have changed since
  // it was tested in the SoSceneManager::scheduleRedraw() call.
  if (PRIVATE(sm)->isActive()) { sm->redraw(); }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "done\n\n");
#endif // debug
}

// Internal callback.
void
SoSceneManagerP::nodesensorCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::nodesensorCB",
                         "detected change in scene graph");
#endif // debug
  ((SoSceneManager *)data)->scheduleRedraw();
}

void
SoSceneManagerP::prerendercb(void * userdata, SoGLRenderAction * action)
{
  // remove callback again
  action->removePreRenderCallback(prerendercb, userdata);
  // MSVC7 on 64-bit Window wants it to go through this cast.
  const uintptr_t bitfield = (uintptr_t)userdata;
  GLbitfield mask = (GLbitfield)bitfield;

#if COIN_DEBUG && 0 // debug
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  SoDebugError::postInfo("SoSceneManagerP::prerendercb",
                         "GL_VIEWPORT=<%d, %d, %d, %d>",
                         view[0], view[1], view[2], view[3]);
#endif // debug
  
  // clear the viewport
  glClear(mask);

  // We need to set the depth function here _after_ the glClear() call
  // to work around what seems like an nVidia bug. If alpha testing is
  // used without this call, the alpha test will always discard
  // fragments. It basically seems like the alpha component is always
  // 0.0 without this call. pederb, 2006-10-10

  // GL_LEQUAL is the default depth function in SGI Inventor
  glDepthFunc(GL_LEQUAL);
}

void
SoSceneManagerP::render(SoGLRenderAction * action,
                        const SbBool initmatrices,
                        const SbBool clearwindow,
                        const SbBool clearzbuffer)
{
  GLbitfield mask = 0;
  if (clearwindow) mask |= GL_COLOR_BUFFER_BIT;
  if (clearzbuffer) mask |= GL_DEPTH_BUFFER_BIT;

  if (mask) {
    if (this->flags & SoSceneManagerP::FLAG_RGBMODE) {
      glClearColor(this->backgroundcolor[0],
                   this->backgroundcolor[1],
                   this->backgroundcolor[2],
                   0.0f);
    }
    else {
      glClearIndex((GLfloat) this->backgroundindex);
    }
    // Registering a callback is needed since the correct GL viewport
    // is set by SoGLRenderAction before rendering. It might not be
    // correct when we get here.
    // This callback is removed again in the prerendercb function
    action->addPreRenderCallback(this->prerendercb, 
                                 (void*) (uintptr_t) mask);
  }

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
  this->lock();
  if (this->rootsensor && this->rootsensor->isScheduled()) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "rootsensor unschedule");
#endif // debug
    this->rootsensor->unschedule();
  }
  this->unlock();
  // Apply the SoGLRenderAction to the scenegraph root.
  if (this->scene) action->apply(this->scene);


  // Automatically re-triggers rendering if any animation stuff is
  // connected to the realTime field.
  if (SoSceneManagerP::touchtimer) {
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

// render once in correct draw style
void 
SoSceneManagerP::renderSingle(SoGLRenderAction * action,
                              SbBool initmatrices,
                              SbBool clearwindow,
                              SbBool clearzbuffer)
{
  SoState * state = action->getState();
  state->push();

  SoNode * node = this->dummynode;

  if (!this->texturesenabled) {
    SoTextureQualityElement::set(state, node, 0.0f);
    SoTextureOverrideElement::setQualityOverride(state, TRUE);
  }
  
  switch (this->rendermode) {
  case SoSceneManager::AS_IS:
    this->render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoSceneManager::WIREFRAME:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    this->render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoSceneManager::POINTS:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::POINTS);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    this->render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoSceneManager::HIDDEN_LINE:
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
      this->render(action, initmatrices, FALSE, FALSE);

      // reenable draw masks
      glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, FALSE);
      this->render(action, initmatrices, FALSE, FALSE);    
    }
    break;
  case SoSceneManager::WIREFRAME_OVERLAY:
      SoPolygonOffsetElement::set(state, node, 1.0f, 1.0f,
                                  SoPolygonOffsetElement::FILLED, TRUE);
      SoOverrideElement::setPolygonOffsetOverride(state, node, TRUE);
      this->render(action, initmatrices, clearwindow, clearzbuffer);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      
      SoLazyElement::setDiffuse(state, node, 1, &this->overlaycolor, 
                                &this->colorpacker);
      SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
      SoMaterialBindingElement::set(state, node, SoMaterialBindingElement::OVERALL);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setLightModelOverride(state, node, TRUE);
      SoOverrideElement::setDiffuseColorOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, TRUE);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      this->render(action, initmatrices, FALSE, FALSE);    
    break;

  case SoSceneManager::BOUNDING_BOX:
    SoComplexityTypeElement::set(state, node, SoComplexityTypeElement::BOUNDING_BOX);
    SoOverrideElement::setComplexityTypeOverride(state, node, TRUE);
    this->render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  default:
    assert(0 && "unknown rendering mode");
    break;
  }
  state->pop();
}

// render scene according to current stereo mode
void 
SoSceneManagerP::renderStereo(SoGLRenderAction * action,
                              SbBool initmatrices,
                              SbBool clearwindow,
                              SbBool clearzbuffer)
{
  SoCamera * camera = this->getCamera();
  if (!camera) return;

  this->clearBuffers(TRUE, TRUE);
  camera->setStereoAdjustment(this->stereooffset);
  camera->setStereoMode(SoCamera::LEFT_VIEW);
  
  SbBool stenciltestenabled = glIsEnabled(GL_STENCIL_TEST);

  // left eye
  switch (this->stereomode) {
  case SoSceneManager::ANAGLYPH:
    glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    this->renderSingle(action, initmatrices, FALSE, FALSE);
    break;
  case SoSceneManager::QUAD_BUFFER:
    glDrawBuffer(this->doublebuffer ? GL_BACK_LEFT : GL_FRONT_LEFT);
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoSceneManager::INTERLEAVED_ROWS:
  case SoSceneManager::INTERLEAVED_COLUMNS:
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

  camera->setStereoMode(SoCamera::RIGHT_VIEW);

  // right eye
  switch (this->stereomode) {
  case SoSceneManager::ANAGLYPH:
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    this->renderSingle(action, initmatrices, FALSE, TRUE);
    break;
  case SoSceneManager::QUAD_BUFFER:
    glDrawBuffer(this->doublebuffer ? GL_BACK_RIGHT : GL_FRONT_RIGHT);
    this->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case SoSceneManager::INTERLEAVED_ROWS:
  case SoSceneManager::INTERLEAVED_COLUMNS:
    glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
    this->renderSingle(action, initmatrices, FALSE, FALSE);
    break;
  default:
    assert(0 && "unknown stereo mode");
    break;
  }

  // restore / post render operations
  camera->setStereoMode(SoCamera::MONOSCOPIC);

  switch (stereomode) {
  case SoSceneManager::ANAGLYPH:
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    break;
  case SoSceneManager::QUAD_BUFFER:
    glDrawBuffer(this->doublebuffer ? GL_BACK : GL_FRONT);
    break;
  case SoSceneManager::INTERLEAVED_ROWS:
  case SoSceneManager::INTERLEAVED_COLUMNS:
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
SoSceneManagerP::initStencilBufferForInterleavedStereo(void)
{
  const SbViewportRegion & currentvp = this->glaction->getViewportRegion();
  if (this->stereostencilmaskvp == currentvp) { return; } // the common case

  SoSceneManager::StereoMode s = PUBLIC(this)->getStereoMode();
  assert((s == SoSceneManager::INTERLEAVED_ROWS) ||
         (s == SoSceneManager::INTERLEAVED_COLUMNS));

  // Find out whether or not we need to regenerate the mask data.
  SbBool allocnewmask = (this->stereostencilmask == NULL);

  const SbVec2s neworigin = currentvp.getViewportOriginPixels();
  const SbVec2s newsize = currentvp.getViewportSizePixels();

  const SbVec2s oldorigin = this->stereostencilmaskvp.getViewportOriginPixels();
  const SbVec2s oldsize = this->stereostencilmaskvp.getViewportSizePixels();

  allocnewmask = allocnewmask ||
    ((oldsize[0] + 7) / 8 * oldsize[1]) < ((newsize[0] + 7) / 8 * newsize[1]);

  const SbBool fillmask = allocnewmask || (this->stereostenciltype != s) ||
    ((s == SoSceneManager::INTERLEAVED_ROWS) && (oldsize[0] != newsize[0]));

  const SbBool layoutchange = !(this->stereostencilmaskvp == currentvp);

  const short bytewidth = (newsize[0] + 7) / 8;

  if (allocnewmask) {
    delete[] this->stereostencilmask;
    this->stereostencilmask = new GLubyte[bytewidth * newsize[1]];
  }

  this->stereostencilmaskvp = currentvp;

  if (fillmask) {
    GLubyte * mask = this->stereostencilmask;

    if (s == SoSceneManager::INTERLEAVED_COLUMNS) {
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

    this->stereostenciltype = s;
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
                 this->stereostencilmask);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

void
SoSceneManagerP::setCamera(SoCamera * camera)
{
  if (this->camera) {
    this->camera->unref();
  }
  this->camera = camera;
  if (camera) camera->ref();
}

SoCamera * 
SoSceneManagerP::getCamera(void) 
{
  if (this->camera) return this->camera;
  
  this->searchaction.setType(SoCamera::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  SbBool old = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  this->searchaction.apply(PUBLIC(this)->getSceneGraph());
  SoBaseKit::setSearchingChildren(old);
  SoFullPath * path = (SoFullPath*) this->searchaction.getPath();
  if (path) {
    SoNode * tail = path->getTail();
    this->searchaction.reset();
    return (SoCamera*) tail;
  }
  return NULL;
}

void
SoSceneManagerP::cleanup(void)
{
  SoSceneManagerP::touchtimer = TRUE;
  SoSceneManagerP::cleanupfunctionset = FALSE;
}

#undef PRIVATE
#undef PUBLIC
