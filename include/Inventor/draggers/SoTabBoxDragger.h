/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOTABBOXDRAGGER_H
#define COIN_SOTABBOXDRAGGER_H

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;


class SoTabBoxDragger : public SoDragger {
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

protected:
  ~SoTabBoxDragger();
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void invalidateSurroundScaleCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  SoFieldSensor * translFieldSensor;
  SoFieldSensor * scaleFieldSensor;

private:
  void initTransformNodes(void);
};

#endif // !COIN_SOTABBOXDRAGGER_H
