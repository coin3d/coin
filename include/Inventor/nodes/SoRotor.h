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
