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

typedef void SoGLRenderPassCB(void * userdata);


class COIN_DLL_EXPORT SoGLRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGLRenderAction);

public:
  SoGLRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoGLRenderAction();

  static void initClass(void);

  enum TransparencyType {
    SCREEN_DOOR,
    ADD, DELAYED_ADD, SORTED_OBJECT_ADD,
    BLEND, DELAYED_BLEND, SORTED_OBJECT_BLEND
  };

  enum AbortCode {
    CONTINUE, ABORT, PRUNE, DELAY
  };

  typedef AbortCode SoGLRenderAbortCB(void * userdata);

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void) const;
  void setUpdateArea(const SbVec2f & origin, const SbVec2f & size);
  void getUpdateArea(SbVec2f & origin, SbVec2f & size) const;
  void setAbortCallback(SoGLRenderAbortCB * const func, void * const userdata);
  void setTransparencyType(const TransparencyType type);
  TransparencyType getTransparencyType(void) const;
  void setSmoothing(const SbBool smooth);
  SbBool isSmoothing(void) const;
  void setNumPasses(const int num);
  int getNumPasses(void) const;
  void setPassUpdate(const SbBool flag);
  SbBool isPassUpdate(void) const;
  void setPassCallback(SoGLRenderPassCB * const func, void * const userdata);
  void setCacheContext(const uint32_t context);
  uint32_t getCacheContext(void) const;

  void addDelayedPath(SoPath * path);
  SbBool isRenderingDelayedPaths(void) const;

  SbBool handleTransparency(SbBool istransparent = FALSE);
  int getCurPass(void) const;
  SbBool abortNow(void);

  void setRenderingIsRemote(SbBool isremote);
  SbBool getRenderingIsRemote(void) const;

protected:
  virtual void beginTraversal(SoNode * node);
  virtual void endTraversal(SoNode * node);

private:
  void addTransPath(SoPath * path);
  void doPathSort(void);

  SbViewportRegion viewport;
  int numpasses;
  TransparencyType transparencytype;
  SbBool smoothing;
  SbBool passupdate;
  SoGLRenderPassCB * passcallback;
  void * passcallbackdata;
  SoGLRenderAbortCB * abortcallback;
  void * abortcallbackdata;
  uint32_t cachecontext;
  SbBool firstrender;
  int currentpass;
  SbBool didhavetransparent;
  SbBool isblendenabled;
  void disableBlend(const SbBool force = FALSE);
  void enableBlend(const SbBool force = FALSE);
  SoPathList delayedpaths;
  SbBool delayedrender;
  SbBool sortrender;
  SoPathList transpobjpaths;
  SbList<float> transpobjdistances;
  class SoGetBoundingBoxAction * bboxaction;
  SbVec2f updateorigin, updatesize;
  SbBool renderingremote;
};

#endif // !COIN_SOGLRENDERACTION_H
