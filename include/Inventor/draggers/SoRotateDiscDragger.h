#ifndef COIN_SOROTATEDISCDRAGGER_H
#define COIN_SOROTATEDISCDRAGGER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/draggers/SoDragger.h>
#include <Inventor/fields/SoSFRotation.h>

class SoSensor;
class SoFieldSensor;
class SbPlaneProjector;


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
  ~SoRotateDiscDragger();
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
};

#endif // !COIN_SOROTATEDISCDRAGGER_H
