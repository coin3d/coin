/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOGLRENDERACTION_H__
#define __SOGLRENDERACTION_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLRENDERACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOGLRENDERACTION

#include <Inventor/actions/SoAction.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/system/inttypes.h>

#if !defined(COIN_EXCLUDE_SOPATH)  
#include <Inventor/lists/SoPathList.h>
#endif // !COIN_EXCLUDE_SOPATH



class SoGLRenderAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoGLRenderAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoGLRenderAction();
//$ END TEMPLATE ActionHeader

public:

  enum TransparencyType {
    SCREEN_DOOR,
    ADD,        
    DELAYED_ADD, 
    SORTED_OBJECT_ADD,
    BLEND,            
    DELAYED_BLEND,    
    SORTED_OBJECT_BLEND,
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

#if !defined(COIN_EXCLUDE_SOPATH)
  void addDelayedPath(SoPath *path);
#endif // ! COIN_EXCLUDE_SOPATH
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

#if !defined(COIN_EXCLUDE_SOPATH)  
  SoPathList delayedPaths;
#endif // !COIN_EXCLUDE_SOPATH
  SbBool delayedRender;

};

#endif // !__SOGLRENDERACTION_H__
