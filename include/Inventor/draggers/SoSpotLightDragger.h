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

#ifndef COIN_SOSPOTLIGHTDRAGGER_H
#define COIN_SOSPOTLIGHTDRAGGER_H

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>

class SoSensor;
class SoFieldSensor;
class SbPlaneProjector;


class COIN_DLL_EXPORT SoSpotLightDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoSpotLightDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(beam);
  SO_KIT_CATALOG_ENTRY_HEADER(beamActive);
  SO_KIT_CATALOG_ENTRY_HEADER(beamPlacement);
  SO_KIT_CATALOG_ENTRY_HEADER(beamScale);
  SO_KIT_CATALOG_ENTRY_HEADER(beamSep);
  SO_KIT_CATALOG_ENTRY_HEADER(beamSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(rotator);
  SO_KIT_CATALOG_ENTRY_HEADER(translator);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorRotInv);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorSep);


public:
  static void initClass(void);
  SoSpotLightDragger(void);

  SoSFRotation rotation;
  SoSFVec3f translation;
  SoSFFloat angle;

protected:
  ~SoSpotLightDragger();
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void doneCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  void dragStart(void);
  void drag(void);
  void dragFinish(void);

  void setBeamScaleFromAngle(float beamangle);

  SoFieldSensor * rotFieldSensor;
  SoFieldSensor * translFieldSensor;
  SoFieldSensor * angleFieldSensor;
  SbPlaneProjector * planeProj;
};

#endif // !COIN_SOSPOTLIGHTDRAGGER_H
