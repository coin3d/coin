#ifndef COIN_SONAVIGATIONMODE_H
#define COIN_SONAVIGATIONMODE_H

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

#include <Inventor/SbName.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>

#include <Inventor/SbBasic.h>
#include <Inventor/navigation/SoSubMode.h>

class SbRotation;
class SoNode;
class SoCamera;
class SoEvent;

class SoNavigationSystem;
class SoNavigationControl;

class SoNavigationMode;
class SoNavigationModeP;

typedef float SoNavigation1DInputValueFunc(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
typedef SbVec2f SoNavigation2DInputValueFunc(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
typedef SbVec3f SoNavigation3DInputValueFunc(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);

class COIN_DLL_API SoNavigationMode {
  SO_NAVIGATION_MODE_HEADER(SoNavigationMode);

public:
  static void initClass(void);

  // predefined set of input value functions
  static float getAbsMouseMovePixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveVerticalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveVerticalNormalizedDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveHorizontalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveHorizontalNormalizedDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getAbsMouseCenterPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseCenterVerticalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseCenterHorizontalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseCenterAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);
  static float getMouseMoveCenterAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl);

public:
  SoNavigationMode(SbName modename);
  virtual ~SoNavigationMode(void);

  virtual SoNavigationMode * clone(void) const = 0;

  SbName getModeName(void) const;

  void set1DValueFunc(SoNavigation1DInputValueFunc * func, void * closure);
  SoNavigation1DInputValueFunc * get1DValueFunc(void ** closureptr = NULL) const;
  void set2DValueFunc(SoNavigation2DInputValueFunc * func, void * closure);
  SoNavigation2DInputValueFunc * get2DValueFunc(void ** closureptr = NULL) const;
  void set3DValueFunc(SoNavigation3DInputValueFunc * func, void * closure);
  SoNavigation3DInputValueFunc * get3DValueFunc(void ** closureptr = NULL) const;

  SbBool processEvent(const SoEvent * event, const SoNavigationControl * ctrl);

  virtual SoNode * getSceneGraph(void);

  virtual SbBool handleEvent(const SoEvent * event, const SoNavigationControl * ctrl) = 0;

  virtual void init(const SoEvent * event, const SoNavigationControl * ctrl);
  virtual void abort(const SoEvent * event, const SoNavigationControl * ctrl);
  virtual void finish(const SoEvent * event, const SoNavigationControl * ctrl);

  SbBool isAborted(void) const;
  SbBool isFinished(void) const;

protected:
  float get1DValue(const SoNavigationControl * ctrl) const;
  SbVec2f get2DValue(const SoNavigationControl * ctrl) const;
  SbVec3f get3DValue(const SoNavigationControl * ctrl) const;

  SbVec2s getInitialPosition(void) const;
  SbVec2s getPreviousPosition(void) const;
  SbVec2s getCurrentPosition(void) const;

  SbVec2f getInitialNormalizedPosition(const SoNavigationControl * ctrl) const;
  SbVec2f getPreviousNormalizedPosition(const SoNavigationControl * ctrl) const;
  SbVec2f getCurrentNormalizedPosition(const SoNavigationControl * ctrl) const;

  void abort(void);
  void finish(void);

private:
  SoNavigationModeP * pimpl;

}; // SoNavigationMode

#endif // !COIN_SONAVIGATIONMODE_H
