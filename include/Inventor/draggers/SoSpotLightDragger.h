#ifndef COIN_SOSPOTLIGHTDRAGGER_H
#define COIN_SOSPOTLIGHTDRAGGER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>

class SoSensor;
class SoFieldSensor;
class SbPlaneProjector;


class COIN_DLL_API SoSpotLightDragger : public SoDragger {
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
