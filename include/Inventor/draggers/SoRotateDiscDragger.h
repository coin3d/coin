#ifndef COIN_SOROTATEDISCDRAGGER_H
#define COIN_SOROTATEDISCDRAGGER_H

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

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/tools/SbLazyPimplPtr.h>
#include <Inventor/fields/SoSFRotation.h>

class SoSensor;
class SoFieldSensor;
class SbPlaneProjector;
class SoRotateDiscDraggerP;

class COIN_DLL_API SoRotateDiscDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoRotateDiscDragger);
  SO_KIT_CATALOG_ENTRY_HEADER(feedback);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackActive);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(rotator);
  SO_KIT_CATALOG_ENTRY_HEADER(rotatorActive);
  SO_KIT_CATALOG_ENTRY_HEADER(rotatorSwitch);

public:
  static void initClass(void);
  SoRotateDiscDragger(void);

  SoSFRotation rotation;

protected:
  virtual ~SoRotateDiscDragger(void);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);

  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void doneCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  void dragStart(void);
  void drag(void);
  void dragFinish(void);

  SoFieldSensor * fieldSensor;
  SbPlaneProjector * planeProj;

private:
  SbLazyPimplPtr<SoRotateDiscDraggerP> pimpl;

  // NOT IMPLEMENTED:
  SoRotateDiscDragger(const SoRotateDiscDragger & rhs);
  SoRotateDiscDragger & operator = (const SoRotateDiscDragger & rhs);
}; // SoRotateDiscDragger

#endif // !COIN_SOROTATEDISCDRAGGER_H
