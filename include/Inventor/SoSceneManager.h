#ifndef COIN_SOSCENEMANAGER_H
#define COIN_SOSCENEMANAGER_H

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

#include <Inventor/SbColor.h>
#include <Inventor/SbVec2s.h>

class SbViewportRegion;
class SoEvent;
class SoGLRenderAction;
class SoHandleEventAction;
class SoNode;
class SoNodeSensor;
class SoOneShotSensor;
class SoSensor;

class SoSceneManager;
typedef void SoSceneManagerRenderCB(void * userdata, SoSceneManager * mgr);


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
  void setHandleEventAction(SoHandleEventAction * hea);
  SoHandleEventAction * getHandleEventAction(void) const;

  static uint32_t getDefaultRedrawPriority(void);
  static void enableRealTimeUpdate(const SbBool flag);
  static SbBool isRealTimeUpdateEnabled(void);

protected:
  int isActive(void) const;
  void redraw(void);

private:
  class SoSceneManagerP * pimpl;
  friend class SoSceneManagerP;
};

#endif // !COIN_SOSCENEMANAGER_H
