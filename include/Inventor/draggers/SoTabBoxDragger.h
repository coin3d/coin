#ifndef COIN_SOTABBOXDRAGGER_H
#define COIN_SOTABBOXDRAGGER_H

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
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;
class SoTabBoxDraggerP;

class COIN_DLL_API SoTabBoxDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoTabBoxDragger);
  SO_KIT_CATALOG_ENTRY_HEADER(boxGeom);
  SO_KIT_CATALOG_ENTRY_HEADER(surroundScale);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane1Xf);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane2Xf);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane3Xf);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane4Xf);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane5Xf);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6Sep);
  SO_KIT_CATALOG_ENTRY_HEADER(tabPlane6Xf);

public:
  static void initClass(void);
  SoTabBoxDragger(void);

  SoSFVec3f translation;
  SoSFVec3f scaleFactor;

  void adjustScaleTabSize(void);

protected:
  virtual ~SoTabBoxDragger(void);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void invalidateSurroundScaleCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  SoFieldSensor * translFieldSensor;
  SoFieldSensor * scaleFieldSensor;

private:
  void initTransformNodes(void);

private:
  SbLazyPimplPtr<SoTabBoxDraggerP> pimpl;

  // NOT IMPLEMENTED:
  SoTabBoxDragger(const SoTabBoxDragger & rhs);
  SoTabBoxDragger & operator = (const SoTabBoxDragger & rhs);
}; // SoTabBoxDragger

#endif // !COIN_SOTABBOXDRAGGER_H
