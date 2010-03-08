#ifndef COIN_SOSCXMLROTATETARGET_H
#define COIN_SOSCXMLROTATETARGET_H

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

class SbVec3f;
class SbRotation;
class SoCamera;

#define COIN_NAVIGATION_ROTATE_TARGET_EVENT_PREFIX COIN_NAVIGATION_EVENT_PREFIX ".Rotate"

class COIN_DLL_API SoScXMLRotateTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLRotateTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLRotateTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLRotateTarget * singleton(void);

  static const SbName & BEGIN(void);
  static const SbName & UPDATE(void);
  static const SbName & END(void);
  static const SbName & SET_FOCAL_POINT(void);

  //void rotateCamera();
  static void setFocalPoint(SoCamera * camera, const SbVec3f & worldspace);
  static void setFocalPoint(SoCamera * camera, const SbVec3f & worldspace, const SbVec3f & upvector);
  static void reorientCamera(SoCamera * camera, const SbRotation & rot);

protected:
  SoScXMLRotateTarget(void);
  virtual ~SoScXMLRotateTarget(void);

  // sent events
  static const SbName & TRIGGER_SPIN(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  class PImpl;
  static SoScXMLRotateTarget * theSingleton;

}; // SoScXMLRotateTarget

#endif // !COIN_SOSCXMLROTATETARGET_H
