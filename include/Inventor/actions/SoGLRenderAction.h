#ifndef COIN_SOGLRENDERACTION_H
#define COIN_SOGLRENDERACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/lists/SbList.h>

typedef void SoGLRenderPassCB(void * userdata);
typedef void SoGLPreRenderCB(void * userdata, class SoGLRenderAction * action);

class COIN_DLL_API SoGLRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGLRenderAction);

public:
  SoGLRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoGLRenderAction();

  static void initClass(void);

  enum TransparencyType {
    SCREEN_DOOR,
    ADD, DELAYED_ADD, SORTED_OBJECT_ADD,
    BLEND, DELAYED_BLEND, SORTED_OBJECT_BLEND,
    // The remaining are Coin extensions to the common Inventor API
    SORTED_OBJECT_SORTED_TRIANGLE_ADD,
    SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
    NONE
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

  virtual void invalidateState(void);

  void addPreRenderCallback(SoGLPreRenderCB * func, void * userdata);
  void removePreRenderCallback(SoGLPreRenderCB * func, void * userdata);

protected:
  virtual void beginTraversal(SoNode * node);
  virtual void endTraversal(SoNode * node);

private:
  void addTransPath(SoPath * path);
  void doPathSort(void);

  class SoGLRenderActionP * pimpl;
  friend class SoGLRenderActionP;
};

#endif // !COIN_SOGLRENDERACTION_H
