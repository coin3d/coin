#ifndef COIN_SOEVENTMANAGER_H
#define COIN_SOEVENTMANAGER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/tools/SbPimplPtr.h>

class SoCamera;
class SbVec2s;
class SoEvent;
class SoNode;
class SbViewportRegion;
class SoNavigationSystem;
class SoHandleEventAction;
class SoScXMLStateMachine;
class SoEventManagerP;

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
  SoNavigationSystem * getNavigationSystem(void) const;

  int getNumSoScXMLStateMachines(void) const;
  SoScXMLStateMachine * getSoScXMLStateMachine(int idx) const;
  void addSoScXMLStateMachine(SoScXMLStateMachine * sm);
  void removeSoScXMLStateMachine(SoScXMLStateMachine * sm);

  void setNavigationSystem(SoNavigationSystem * system);
  NavigationState getNavigationState(void) const;

  void setSize(const SbVec2s & newsize);
  void setOrigin(const SbVec2s & newOrigin);
  void setViewportRegion(const SbViewportRegion & newregion);
  const SbViewportRegion & getViewportRegion(void) const;

protected:
  virtual SbBool actuallyProcessEvent(const SoEvent * const event);

private:
  SbPimplPtr<SoEventManagerP> pimpl;

  SoEventManager(const SoEventManager & rhs); // N/A
  SoEventManager & operator = (const SoEventManager & rhs); // N/A

}; // SoEventManager

#endif // !COIN_SOEVENTMANAGER_H
