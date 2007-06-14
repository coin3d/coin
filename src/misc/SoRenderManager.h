#ifndef COIN_SORENDERMANAGER_H
#define COIN_SORENDERMANAGER_H

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

#include <Inventor/system/gl.h>
#include <Inventor/SoSceneManager.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoLazyElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

class SoCamera;
class SoInfo;

class COIN_DLL_API SoRenderManager {
public:
  SoRenderManager(SoSceneManager * scenemanager);
  ~SoRenderManager();

  void render(SoGLRenderAction * action,
              const SbBool initmatrices = TRUE,
              const SbBool clearwindow = TRUE,
              const SbBool clearzbuffer = TRUE);

  void attachRootSensor(SoNode * const sceneroot);
  void detachRootSensor(void);
  static void nodesensorCB(void * data, SoSensor * sensor);
  static void prerendercb(void * userdata, SoGLRenderAction * action);
  void touch(void);

  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }

protected:
  void actuallyRender(SoGLRenderAction * action,
                      const SbBool initmatrices = TRUE,
                      const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  
  void renderSingle(SoGLRenderAction * action,
                    SbBool initmatrices,
                    SbBool clearwindow,
                    SbBool clearzbuffer);
  
  void renderStereo(SoGLRenderAction * action,
                    SbBool initmatrices,
                    SbBool clearwindow,
                    SbBool clearzbuffer);

  void initStencilBufferForInterleavedStereo(void);
  void clearBuffers(SbBool color, SbBool depth);  

private:
  class SoSceneManager * scenemanager;  
  SoNodeSensor * rootsensor;
  SoInfo * dummynode;
  SbBool texturesenabled;
  SbColor overlaycolor;
  SoColorPacker colorpacker;
  SbViewportRegion stereostencilmaskvp;
  GLubyte * stereostencilmask;
  SoSceneManager::StereoMode stereostenciltype;

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
};


#endif // !COIN_SORENDERMANAGER_H
