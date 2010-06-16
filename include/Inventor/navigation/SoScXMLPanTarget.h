#ifndef COIN_SOSCXMLPANTARGET_H
#define COIN_SOSCXMLPANTARGET_H

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

#include <Inventor/navigation/SoScXMLNavigationTarget.h>

class SbVec2f;
class SbPlane;
class SoCamera;

#define COIN_NAVIGATION_PAN_TARGET_EVENT_PREFIX COIN_NAVIGATION_EVENT_PREFIX ".Pan"

class COIN_DLL_API SoScXMLPanTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLPanTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLPanTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLPanTarget * singleton(void);

  static const SbName & BEGIN(void);
  static const SbName & UPDATE(void);
  static const SbName & END(void);
  static const SbName & SET_FOCAL_POINT(void);
  static const SbName & MOVE(void);

  static void panCamera(SoCamera * camera,
                        float vpaspect,
                        const SbPlane & panplane,
                        const SbVec2f & previous,
                        const SbVec2f & current);

  static void panSetFocalPoint(SoCamera * camera, const SbVec3f & worldspace);

  static void translateCamera(SoCamera * camera, const SbVec3f & translation, SbBool cameraspace = FALSE);

protected:
  SoScXMLPanTarget(void);
  virtual ~SoScXMLPanTarget(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  class PImpl;
  static SoScXMLPanTarget * theSingleton;

}; // SoScXMLPanTarget

#endif // !COIN_SOSCXMLPANTARGET_H
