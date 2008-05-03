#ifndef COIN_SONAVIGATIONSYSTEM_H
#define COIN_SONAVIGATIONSYSTEM_H

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

// IMPORTANT NOTE: This class is part of a development DEAD END in
// Coin that WILL BE DEPRECATED and removed in a future version.
// It is only included because of 3rd party dependencies created towards
// the development branch of Coin before the Coin 3.0 release.
// Use ScXML-based navigation instead.  It's the way of the future.

#include <Inventor/SbName.h>
#include <Inventor/SbVec3f.h>

#include <Inventor/SbBasic.h>

class SbViewportRegion;
class SoNode;
class SoPath;
class SoCamera;
class SoEvent;

class SoNavigationMode;
class SoNavigationSystem;
class SoNavigationControl;

#define SO_IDLER_SYSTEM                 "idler"
#define SO_IDLER_IDLE_MODE              "idler:idle"

#define SO_EXAMINER_SYSTEM              "examiner"
#define SO_EXAMINER_IDLE_MODE           "examiner:idle"
#define SO_EXAMINER_ROTATE_MODE         "examiner:rotate"
#define SO_EXAMINER_WAITFORZOOM_MODE    "examiner:wait_for_zoom"
#define SO_EXAMINER_ZOOM_MODE           "examiner:zoom"
#define SO_EXAMINER_WAITFORPAN_MODE     "examiner:wait_for_pan"
#define SO_EXAMINER_PAN_MODE            "examiner:pan"
#define SO_EXAMINER_WAITFORCENTER_MODE  "examiner:wait_for_center"
#define SO_EXAMINER_CENTER_MODE         "examiner:center"

#define SO_PLANEVIEWER_SYSTEM           "planeviewer"
#define SO_PLANEVIEWER_IDLE_MODE        "planeviewer:idle"
#define SO_PLANEVIEWER_PAN_MODE         "planeviewer:pan"
#define SO_PLANEVIEWER_WAITFORZOOM_MODE "planeviewer:wait_for_zoom"
#define SO_PLANEVIEWER_ZOOM_MODE        "planeviewer:zoom"

// some simple button1-modes
#define SO_PANNER_SYSTEM                "panner"
#define SO_PANNER_IDLE_MODE             "panner:idle"
#define SO_PANNER_PAN_MODE              "panner:pan"

#define SO_ZOOMER_SYSTEM                "zoomer"
#define SO_ZOOMER_IDLE_MODE             "zoomer:idle"
#define SO_ZOOMER_ZOOM_MODE             "zoomer:zoom"

#define SO_ROTATER_SYSTEM               "rotater"
#define SO_ROTATER_IDLE_MODE            "rotater:idle"
#define SO_ROTATER_ROTATE_MODE          "rotater:rotate"

#define SO_CENTERER_SYSTEM              "centerer"
#define SO_CENTERER_IDLE_MODE           "centerer:idle"
#define SO_CENTERER_CENTER_MODE         "centerer:center"

#define SO_PICKER_SYSTEM                "picker"
#define SO_PICKER_IDLE_MODE             "picker:idle"
#define SO_PICKER_PICK_MODE             "picker:pick"

#define SO_DEFAULT_SYSTEM               SO_EXAMINER_SYSTEM

typedef
  void SoNavigationModeChangeCB(void * closure, SoNavigationSystem * system);

class SoNavigationSystemP;

class COIN_DLL_API SoNavigationSystem {
public:
  static void initClass(void);
  static void cleanClass(void);

  static SbBool registerSystem(SoNavigationSystem * system);
  static SbBool unregisterSystem(SoNavigationSystem * system);
  static SbBool isRegistered(SoNavigationSystem * system);

  static SoNavigationSystem * getByName(SbName name);
  static SoNavigationSystem * createByName(SbName name);

public:
  SoNavigationSystem(SbName name);
  ~SoNavigationSystem(void);

  SoNavigationSystem * clone(void) const;

  void addModeChangeCallback(SoNavigationModeChangeCB * cb, void * closure);
  void removeModeChangeCallback(SoNavigationModeChangeCB * cb, void * closure);

  void setSceneGraph(SoNode * scenegraph);
  void setCamera(SoCamera * camera);
  void setViewport(const SbViewportRegion & viewport);

  void viewAll(void);
  void viewPart(SoPath * path, const SbVec3f & in, const SbVec3f & up);
  void viewPart(SoNode * node, const SbVec3f & in, const SbVec3f & up);

  SbBool processEvent(const SoEvent * event);
  SoNavigationControl * getNavigationControl(void) const;

  SbName getName(void) const;

  SbName getCurrentModeName(void) const;
  SoNavigationMode * getMode(SbName name) const;
  SoNavigationMode * getCurrentMode(void) const;

  enum TransitionType {
    INITIAL,
    STACK,
    SWITCH,
    FINISH,
    ABORT
  };

  void addMode(SoNavigationMode * mode);
  void addModeTransition(SoNavigationMode * mode,
                         TransitionType type,
                         const SoEvent * trigger = NULL,
                         const SoEvent * condition = NULL);
  void addModeTransition(SoNavigationMode * mode1,
                         SoNavigationMode * mode2,
                         TransitionType type,
                         const SoEvent * trigger,
                         const SoEvent * condition = NULL);

protected:
  friend class SoSceneManager;
  friend class SoEventManager;
  void invokeModeChangeCallbacks(void);

private:
  SoNavigationSystemP * pimpl;

}; // SoNavigationSystem

#endif // !COIN_SONAVIGATIONSYSTEM_H
