#ifndef COIN_SOSCXMLZOOMTARGET_H
#define COIN_SOSCXMLZOOMTARGET_H

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

class SoCamera;
class SoPerspectiveCamera;
class SoOrthographicCamera;
class SoFrustumCamera;

#define COIN_NAVIGATION_ZOOM_TARGET_EVENT_PREFIX SOSCXML_NAVIGATION_TARGETTYPE ".Zoom"

class COIN_DLL_API SoScXMLZoomTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLZoomTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLZoomTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLZoomTarget * singleton(void);

  static const SbName & BEGIN(void);
  static const SbName & UPDATE(void);
  static const SbName & END(void);
  static const SbName & ZOOM(void);
  static const SbName & ZOOM_IN(void);
  static const SbName & ZOOM_OUT(void);
  static const SbName & RESET(void);

  static void zoom(SoCamera * camera, float factor);
  static void reset(SoCamera * camera);

protected:
  SoScXMLZoomTarget(void);
  virtual ~SoScXMLZoomTarget(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  class PImpl;
  static SoScXMLZoomTarget * theSingleton;

}; // SoScXMLZoomTarget

#endif // !COIN_SOSCXMLZOOMTARGET_H
