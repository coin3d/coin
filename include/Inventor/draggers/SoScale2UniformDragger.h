#ifndef COIN_SOSCALE2UNIFORMDRAGGER_H
#define COIN_SOSCALE2UNIFORMDRAGGER_H

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
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbVec3f.h>

class SoSensor;
class SoFieldSensor;
class SbLineProjector;


class COIN_DLL_API SoScale2UniformDragger : public SoDragger {
  typedef SoDragger inherited;

  SO_KIT_HEADER(SoScale2UniformDragger);

  SO_KIT_CATALOG_ENTRY_HEADER(feedback);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackActive);
  SO_KIT_CATALOG_ENTRY_HEADER(feedbackSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(scaler);
  SO_KIT_CATALOG_ENTRY_HEADER(scalerActive);
  SO_KIT_CATALOG_ENTRY_HEADER(scalerSwitch);


public:
  static void initClass(void);
  SoScale2UniformDragger(void);

  SoSFVec3f scaleFactor;

protected:
  ~SoScale2UniformDragger();
  virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways = FALSE);

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
};

#endif // !COIN_SOSCALE2UNIFORMDRAGGER_H
