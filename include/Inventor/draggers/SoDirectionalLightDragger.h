/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SODIRECTIONALLIGHTDRAGGER_H
#define COIN_SODIRECTIONALLIGHTDRAGGER_H

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoSensor;
class SoFieldSensor;


class SoDirectionalLightDragger : public SoDragger {
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
  SoSFVec3f center;

protected:
  ~SoDirectionalLightDragger();
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);
  virtual void setDefaultOnNonWritingFields(void);
  
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);
  
  SoFieldSensor * rotFieldSensor;
  SoFieldSensor * centerFieldSensor;
};

#endif // !COIN_SODIRECTIONALLIGHTDRAGGER_H
