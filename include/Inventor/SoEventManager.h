#ifndef COIN_SOEVENTMANAGER_H
#define COIN_SOEVENTMANAGER_H

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

#include <Inventor/SbBasic.h>

class SoCamera;
class SbVec2s;
class SoEvent;
class SoNode;
class SbViewportRegion;
class SoNavigationSystem;
class SoHandleEventAction;

class COIN_DLL_API SoEventManager {
 public:
  SoEventManager(void);
  virtual ~SoEventManager();
  
  enum NavigationState {
    NO_NAVIGATION,
    JUST_NAVIGATION,
    MIXED_NAVIGATION
  };

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  virtual void setSceneGraph(SoNode * const sceneroot);
  virtual SoNode * getSceneGraph(void) const;

  virtual void setHandleEventAction(SoHandleEventAction * hea);
  virtual SoHandleEventAction * getHandleEventAction(void) const;

  virtual SbBool processEvent(const SoEvent * const event);
  
  void setNavigationState(NavigationState state);
  void setNavigationSystem(SoNavigationSystem * system);
  
  NavigationState getNavigationState(void) const;
  SoNavigationSystem * getNavigationSystem(void) const;
  
  void setSize(const SbVec2s & newsize);
  void setOrigin(const SbVec2s & newOrigin);
  void setViewportRegion(const SbViewportRegion & newregion);

protected:
  virtual SbBool actuallyProcessEvent(const SoEvent * const event);

private:
  class SoEventManagerP * pimpl;
};

#endif // !COIN_SOEVENTMANAGER_H
