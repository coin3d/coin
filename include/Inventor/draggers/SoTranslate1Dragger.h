#ifndef COIN_SOTRANSLATE1DRAGGER_H
#define COIN_SOTRANSLATE1DRAGGER_H

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
#include <Inventor/fields/SoSFFloat.h>

class SoSensor;
class SoFieldSensor;
class SbLineProjector;
class SoTranslate1DraggerP;

class COIN_DLL_API SoTranslate1Dragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoTranslate1Dragger);
  SO_KIT_CATALOG_ENTRY_HEADER(feedback);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackActive);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(translator);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorActive);
  SO_KIT_CATALOG_ENTRY_HEADER(translatorSwitch);

public:
static void initClass(void);
  SoTranslate1Dragger(void);

  SoSFVec3f translation;
  SoSFFloat minTranslation;
  SoSFFloat maxTranslation;

protected:
  virtual ~SoTranslate1Dragger(void);
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);

  virtual void setMotionMatrix(const SbMatrix & matrix);

  static void startCB(void * f, SoDragger * d);
  static void motionCB(void * f, SoDragger * d);
  static void finishCB(void * f, SoDragger * d);
  static void fieldSensorCB(void * f, SoSensor * s);
  static void valueChangedCB(void * f, SoDragger * d);

  void dragStart(void);
  void drag(void);
  void dragFinish(void);

  SoFieldSensor * fieldSensor;
  SbLineProjector * lineProj;

private:
  SbVec3f clampMatrix(SbMatrix & m) const;
  SbLazyPimplPtr<SoTranslate1DraggerP> pimpl;

  // NOT IMPLEMENTED:
  SoTranslate1Dragger(const SoTranslate1Dragger & rhs);
  SoTranslate1Dragger & operator = (const SoTranslate1Dragger & rhs);
}; // SoTranslate1Dragger

#endif // !COIN_SOTRANSLATE1DRAGGER_H
