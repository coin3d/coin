#ifndef COIN_SODIRECTIONALLIGHTDRAGGER_H
#define COIN_SODIRECTIONALLIGHTDRAGGER_H

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
class SoDirectionalLightDraggerP;

class COIN_DLL_API SoDirectionalLightDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoDirectionalLightDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(rotator);
  SO_KIT_CATALOG_ENTRY_HEADER(translator);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorRotInv);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorSep);


public:
  static void initClass(void);
  SoDirectionalLightDragger(void);

  SoSFRotation rotation;
  SoSFVec3f translation;

protected:
  virtual ~SoDirectionalLightDragger(void);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);

  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  SoFieldSensor * rotFieldSensor;
  SoFieldSensor * translFieldSensor;

private:
  SbLazyPimplPtr<SoDirectionalLightDraggerP> pimpl;

  // NOT IMPLEMENTED:
  SoDirectionalLightDragger(const SoDirectionalLightDragger & rhs);
  SoDirectionalLightDragger & operator = (const SoDirectionalLightDragger & rhs);
}; // SoDirectionalLightDragger

#endif // !COIN_SODIRECTIONALLIGHTDRAGGER_H
