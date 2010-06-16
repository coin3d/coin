#ifndef COIN_SOSCXMLMISCTARGET_H
#define COIN_SOSCXMLMISCTARGET_H

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

#define SOSCXML_NAVIGATION_MISC_TARGET_EVENT_PREFIX SOSCXML_NAVIGATION_TARGETTYPE ".Misc"

class COIN_DLL_API SoScXMLMiscTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLMiscTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLMiscTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLMiscTarget * singleton(void);

  static const SbName & VIEW_ALL(void);
  static const SbName & REDRAW(void);
  static const SbName & POINT_AT(void);
  static const SbName & SET_FOCAL_DISTANCE(void);
  static const SbName & SET_CAMERA_POSITION(void);

protected:
  SoScXMLMiscTarget(void);
  virtual ~SoScXMLMiscTarget(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  class PImpl;
  static SoScXMLMiscTarget * theSingleton;

}; // SoScXMLMiscTarget

#endif // !COIN_SOSCXMLMISCTARGET_H
