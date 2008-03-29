#ifndef COIN_SONAVIGATIONCONTROL_H
#define COIN_SONAVIGATIONCONTROL_H

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

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>

#include <Inventor/SbBasic.h>

class SoPath;
class SoCamera;
class SoNode;
class SbRotation;
class SbViewportRegion;

class SoNavigationControlP;

class COIN_DLL_API SoNavigationControl {
public:
  SoNavigationControl(void);
  virtual ~SoNavigationControl(void);

  void setSceneGraph(SoNode * scene);
  SoNode * getSceneGraph(void) const;

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  void setUpVector(const SbVec3f & up);
  const SbVec3f & getUpVector(void) const;

  void saveCamera(void) const;
  void restoreCamera(void) const;

  SoPath * pick(SbVec2s pos, SbVec3d & pickpos) const;

  void viewAll(void) const;

  void viewPart(SoPath * path,
                const SbVec3f & in, const SbVec3f & up) const;

  void pointDir(const SbVec3f & dir) const;

  void reorientCamera(const SbRotation & rot) const;
  void reorientCamera(const SbVec3f & pointat) const;
  void pitchCamera(float angle) const;
  void yawCamera(float angle) const;
  void rollCamera(float angle) const;
  void resetRoll(void) const;

  void moveCamera(const SbVec3f & distance) const;
  void moveCamera(float factor, const SbBool keepfocalpoint = TRUE) const;

  void setViewport(const SbViewportRegion & vp);
  SbVec2s getViewportSize(void) const;
  float getViewportAspect(void) const;

protected:

private:
  SoNavigationControlP * pimpl;

}; // SoNavigationControl

#endif  // !COIN_SONAVIGATIONCONTROL_H
