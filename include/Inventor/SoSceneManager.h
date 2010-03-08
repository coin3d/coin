#ifndef COIN_SOSCENEMANAGER_H
#define COIN_SOSCENEMANAGER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbVec2s.h>

class SbViewportRegion;
class SoEvent;
class SoGLRenderAction;
class SoAudioRenderAction;
class SoHandleEventAction;
class SoNode;
class SoCamera;
class SoNodeSensor;
class SoOneShotSensor;
class SoSensor;
class Superimposition;
class SbColor;

typedef void SoSceneManagerRenderCB(void * userdata, class SoSceneManager * mgr);

class COIN_DLL_API SoSceneManager {
public:
  SoSceneManager(void);
  virtual ~SoSceneManager();

  virtual void render(const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  virtual void render(SoGLRenderAction * action,
                      const SbBool initmatrices = TRUE,
                      const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;
  virtual SbBool processEvent(const SoEvent * const event);
  void reinitialize(void);
  void scheduleRedraw(void);
  virtual void setSceneGraph(SoNode * const sceneroot);
  virtual SoNode * getSceneGraph(void) const;
  void setWindowSize(const SbVec2s & newsize);
  const SbVec2s & getWindowSize(void) const;
  void setSize(const SbVec2s & newsize);
  const SbVec2s & getSize(void) const;
  void setOrigin(const SbVec2s & newOrigin);
  const SbVec2s & getOrigin(void) const;
  void setViewportRegion(const SbViewportRegion & newRegion);
  const SbViewportRegion & getViewportRegion(void) const;
  void setBackgroundColor(const SbColor & color);
  const SbColor & getBackgroundColor(void) const;
  void setBackgroundIndex(const int index);
  int getBackgroundIndex(void) const;
  void setRGBMode(const SbBool onOrOff);
  SbBool isRGBMode(void) const;
  virtual void activate(void);
  virtual void deactivate(void);
  void setRenderCallback(SoSceneManagerRenderCB * f,
                         void * const userData = NULL);
  SbBool isAutoRedraw(void) const;
  void setRedrawPriority(const uint32_t priority);
  uint32_t getRedrawPriority(void) const;
  void setAntialiasing(const SbBool smoothing, const int numPasses);
  void getAntialiasing(SbBool & smoothing, int & numPasses) const;
  void setGLRenderAction(SoGLRenderAction * const action);
  SoGLRenderAction * getGLRenderAction(void) const;
  void setAudioRenderAction(SoAudioRenderAction * const action);
  SoAudioRenderAction * getAudioRenderAction(void) const;
  void setHandleEventAction(SoHandleEventAction * hea);
  SoHandleEventAction * getHandleEventAction(void) const;

  static uint32_t getDefaultRedrawPriority(void);
  static void enableRealTimeUpdate(const SbBool flag);
  static SbBool isRealTimeUpdateEnabled(void);

protected:
  int isActive(void) const;
  void redraw(void);

private:
  friend class SoSceneManagerP;
  class SoSceneManagerP * pimpl;

}; // SoSceneManager

#endif // !COIN_SOSCENEMANAGER_H
