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

#ifndef COIN_SOGLRENDERACTION_H
#define COIN_SOGLRENDERACTION_H

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/lists/SbList.h>


class SoGLRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGLRenderAction);

public:

  enum TransparencyType {
    SCREEN_DOOR,
    ADD,
    DELAYED_ADD,
    SORTED_OBJECT_ADD,
    BLEND,
    DELAYED_BLEND,
    SORTED_OBJECT_BLEND
  };

  enum AbortCode {
    CONTINUE,
    ABORT,
    PRUNE,
    DELAY
  };

  typedef AbortCode   SoGLRenderAbortCB(void *userData);
  typedef void        SoGLRenderPassCB(void *userData);


public:
  SoGLRenderAction(const SbViewportRegion &viewportRegion);
  virtual ~SoGLRenderAction();

  static void initClass(void);

  void setViewportRegion(const SbViewportRegion &newRegion);
  const SbViewportRegion &getViewportRegion() const;
  void setUpdateArea(const SbVec2f &origin, const SbVec2f &size);
  void getUpdateArea(SbVec2f &origin, SbVec2f &size) const;
  void setAbortCallback(SoGLRenderAbortCB * const func, void * const userData);
  void setTransparencyType(const TransparencyType type);
  TransparencyType getTransparencyType() const;
  void setSmoothing(const SbBool smooth);
  SbBool isSmoothing() const;
  void setNumPasses(const int num);
  int getNumPasses() const;
  void setPassUpdate(const SbBool flag);
  SbBool isPassUpdate() const;
  void setPassCallback(SoGLRenderPassCB * const func, void *const userData);
  void setCacheContext(const uint32_t context);
  uint32_t getCacheContext() const;

  void addDelayedPath(SoPath *path);
  SbBool isRenderingDelayedPaths() const;

public:

  SbBool handleTransparency(SbBool isTransparent = FALSE);
  int getCurPass() const;
  SbBool abortNow();

protected:
  virtual void beginTraversal(SoNode *node);
  virtual void endTraversal(SoNode *node);

private:
  SbViewportRegion viewport;
  int numPasses;
  TransparencyType transType;
  SbBool smoothing;
  SbBool passUpdate;
  SoGLRenderPassCB *passCB;
  void *passCBdata;
  SoGLRenderAbortCB *abortCB;
  void *abortCBdata;
  uint32_t cacheContext;
  SbBool firstRender;
  int currentPass;
  SbBool didHaveTransparent;
  SbBool isBlendEnabled;
  void disableBlend(const SbBool force = FALSE);
  void enableBlend(const SbBool force = FALSE);

  SoPathList delayedPaths;
  SbBool delayedRender;

  SbBool sortRender;
  SoPathList transpObjPaths;
  SbList <float> transpObjDistances;
  class SoGetBoundingBoxAction *bboxAction;
  void addTransPath(SoPath *path);
  void doPathSort();
};

#endif // !COIN_SOGLRENDERACTION_H
