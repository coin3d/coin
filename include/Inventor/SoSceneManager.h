/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOSCENEMANAGER_H
#define COIN_SOSCENEMANAGER_H

#include <stdio.h> // For NULL definition
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


class SoSceneManager
{
public:
  /*!
    FIXME: write doc
  */
  typedef void SoSceneManagerRenderCB(void * userData, SoSceneManager * mgr);

  SoSceneManager(void);
  virtual ~SoSceneManager();
  virtual void render(const SbBool clearWindow = TRUE,
                      const SbBool clearZbuffer = TRUE);
  virtual SbBool processEvent(const SoEvent * const event);
  void reinitialize(void);
  void scheduleRedraw(void);
  virtual void setSceneGraph(SoNode * const newScene);
  virtual SoNode * getSceneGraph(void) const;
  void setWindowSize(const SbVec2s & newSize);
  const SbVec2s & getWindowSize(void) const;
  void setSize(const SbVec2s & newSize);
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
  static void nodesensorCB(void * data, SoSensor * sensor);
  static void redrawshotTriggeredCB(void * data, SoSensor * sensor);

  SoSceneManagerRenderCB * renderCB;
  void * renderCBdata;

  SoGLRenderAction * glAction;
  SbBool deleteGLAction;
  SoHandleEventAction * handleeventaction;
  SbBool deletehandleeventaction;

  SoNode * scene;
  SoNodeSensor * rootsensor;
  SoOneShotSensor * redrawshot;

  SbColor backgroundcolor;
  int backgroundIndex;
  SbBool rgbMode;
  SbBool active;

  uint32_t redrawPri;
};

#endif // !COIN_SOSCENEMANAGER_H
