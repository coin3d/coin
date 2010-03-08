#ifndef COIN_SOCENTERBALLDRAGGER_H
#define COIN_SOCENTERBALLDRAGGER_H

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
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;
class SoCenterballDraggerP;

class COIN_DLL_API SoCenterballDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoCenterballDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(XAxis);
  SO_KIT_CATALOG_ENTRY_HEADER(XAxisSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(XCenterChanger);
  SO_KIT_CATALOG_ENTRY_HEADER(XRotator);
  SO_KIT_CATALOG_ENTRY_HEADER(YAxis);
  SO_KIT_CATALOG_ENTRY_HEADER(YAxisSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(YCenterChanger);
  SO_KIT_CATALOG_ENTRY_HEADER(YRotator);
  SO_KIT_CATALOG_ENTRY_HEADER(ZAxis);
  SO_KIT_CATALOG_ENTRY_HEADER(ZAxisSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(ZCenterChanger);
  SO_KIT_CATALOG_ENTRY_HEADER(ZRotator);
  SO_KIT_CATALOG_ENTRY_HEADER(antiSquish);
  SO_KIT_CATALOG_ENTRY_HEADER(lightModel);
  SO_KIT_CATALOG_ENTRY_HEADER(rot2X90);
  SO_KIT_CATALOG_ENTRY_HEADER(rotX90);
  SO_KIT_CATALOG_ENTRY_HEADER(rotY90);
  SO_KIT_CATALOG_ENTRY_HEADER(rotator);
  SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);
  SO_KIT_CATALOG_ENTRY_HEADER(translateToCenter);

public:
  static void initClass(void);
  SoCenterballDragger(void);

  virtual void saveStartParameters(void);

  SoSFRotation rotation;
  SoSFVec3f center;

protected:
  virtual ~SoCenterballDragger(void);
  void transferCenterDraggerMotion(SoDragger * childdragger);
  void setSwitches(SoDragger * activechild);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);
  static void kidStartCB(void * f, SoDragger * d);
  static void kidFinishCB(void * f, SoDragger * d);

  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);

  SoFieldSensor * rotFieldSensor;
  SoFieldSensor * centerFieldSensor;

private:
  SbVec3f savedtransl;
  SbVec3f savedcenter;
  void addChildDragger(SoDragger *child);
  void removeChildDragger(const char *childname);

private:
  SbLazyPimplPtr<SoCenterballDraggerP> pimpl;

  // NOT IMPLEMENTED:
  SoCenterballDragger(const SoCenterballDragger & rhs);
  SoCenterballDragger & operator & (const SoCenterballDragger & rhs);
}; // SoCenterballDragger

#endif // !COIN_SOCENTERBALLDRAGGER_H
