#ifndef COIN_SOGLRENDERACTION_H
#define COIN_SOGLRENDERACTION_H

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

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/lists/SbList.h>

typedef void SoGLRenderPassCB(void * userdata);
typedef void SoGLPreRenderCB(void * userdata, class SoGLRenderAction * action);
typedef float SoGLSortedObjectOrderCB(void * userdata, SoGLRenderAction * action);

class SoGLRenderActionP;

class COIN_DLL_API SoGLRenderAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoGLRenderAction);

public:
  static void initClass(void);

  SoGLRenderAction(const SbViewportRegion & viewportregion);
  virtual ~SoGLRenderAction(void);

  enum TransparencyType {
    SCREEN_DOOR,
    ADD, DELAYED_ADD, SORTED_OBJECT_ADD,
    BLEND, DELAYED_BLEND, SORTED_OBJECT_BLEND,
    // The remaining are Coin extensions to the common Inventor API
    SORTED_OBJECT_SORTED_TRIANGLE_ADD,
    SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
    NONE, SORTED_LAYERS_BLEND
  };

  enum TransparentDelayedObjectRenderType {
    ONE_PASS,
    NONSOLID_SEPARATE_BACKFACE_PASS
  };

  enum AbortCode {
    CONTINUE, ABORT, PRUNE, DELAY
  };

  enum SortedObjectOrderStrategy {
    BBOX_CENTER,
    BBOX_CLOSEST_CORNER,
    BBOX_FARTHEST_CORNER,
    CUSTOM_CALLBACK
  };

  typedef AbortCode SoGLRenderAbortCB(void * userdata);

  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void) const;
  void setUpdateArea(const SbVec2f & origin, const SbVec2f & size);
  void getUpdateArea(SbVec2f & origin, SbVec2f & size) const;
  void setAbortCallback(SoGLRenderAbortCB * const func, void * const userdata);
  void getAbortCallback(SoGLRenderAbortCB * & func_out, void * & userdata_out) const;
  void setTransparencyType(const TransparencyType type);
  TransparencyType getTransparencyType(void) const;
  void setTransparentDelayedObjectRenderType(TransparentDelayedObjectRenderType type);
  TransparentDelayedObjectRenderType getTransparentDelayedObjectRenderType(void) const;
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
  void setCurPass(const int passnum, const int numpasses);
  int getCurPass(void) const;
  SbBool abortNow(void);

  void setRenderingIsRemote(SbBool isremote);
  SbBool getRenderingIsRemote(void) const;

  virtual void invalidateState(void);

  void addPreRenderCallback(SoGLPreRenderCB * func, void * userdata);
  void removePreRenderCallback(SoGLPreRenderCB * func, void * userdata);
 
  void setSortedLayersNumPasses(int num);
  int getSortedLayersNumPasses(void) const;

  void setSortedObjectOrderStrategy(const SortedObjectOrderStrategy strategy,
                                    SoGLSortedObjectOrderCB * cb = NULL,
                                    void * closure = NULL);

  void setDelayedObjDepthWrite(SbBool write);
  SbBool getDelayedObjDepthWrite(void) const;

  SbBool isRenderingTranspPaths(void) const;
  SbBool isRenderingTranspBackfaces(void) const;

protected:
  friend class SoGLRenderActionP; // calls beginTraversal
  virtual void beginTraversal(SoNode * node);
  virtual void endTraversal(SoNode * node);

private:
  SbPimplPtr<SoGLRenderActionP> pimpl;

  SoGLRenderAction(const SoGLRenderAction & rhs);
  SoGLRenderAction & operator = (const SoGLRenderAction & rhs);

}; // SoGLRenderAction

#endif // !COIN_SOGLRENDERACTION_H
