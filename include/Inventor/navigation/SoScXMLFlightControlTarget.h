#ifndef COIN_SOSCXMLFLIGHTCONTROLTARGET_H
#define COIN_SOSCXMLFLIGHTCONTROLTARGET_H

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


#define COIN_NAVIGATION_FLIGHT_CONTROL_TARGET_EVENT_PREFIX SOSCXML_NAVIGATION_TARGETTYPE ".FlightControl"

class COIN_DLL_API SoScXMLFlightControlTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLFlightControlTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLFlightControlTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLFlightControlTarget * singleton(void);

  static const SbName & PITCH(void);
  static const SbName & YAW(void);
  static const SbName & ROLL(void);
  static const SbName & RESET_ROLL(void);

  static void pitch(SoCamera * camera, float radians);
  static void yaw(SoCamera * camera, float radians);
  static void roll(SoCamera * camera, float radians);

  static void resetRoll(SoCamera * camera, const SbVec3f & upvector);

protected:
  SoScXMLFlightControlTarget(void);
  virtual ~SoScXMLFlightControlTarget(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  class PImpl;
  static SoScXMLFlightControlTarget * theSingleton;

}; // SoScXMLFlightControlTarget

#endif // !COIN_SOSCXMLFLIGHTCONTROLTARGET_H

