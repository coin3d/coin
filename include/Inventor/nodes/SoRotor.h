/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOROTOR_H
#define COIN_SOROTOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbTime.h>

class SoSensor;
class SoOneShotSensor;
class SoFieldSensor;

class COIN_DLL_EXPORT SoRotor : public SoRotation {
  typedef SoRotation inherited;

  SO_NODE_HEADER(SoRotor);

public:
  static void initClass(void);
  SoRotor(void);

  SoSFFloat speed;
  SoSFBool on;

protected:
  virtual ~SoRotor();

private:
  SbTime starttime;
  SbVec3f startaxis;
  float startangle;
  SoOneShotSensor * oneshotsensor;
  SoFieldSensor * onfieldsensor;
  SoFieldSensor * rotfieldsensor;
  SoFieldSensor * speedfieldsensor;
  static void oneshotSensorCB(void * d, SoSensor * s);
  static void fieldSensorCB(void * d, SoSensor * s);

  void setRotation(void);
};

#endif // !COIN_SOROTOR_H
