/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

class COIN_DLL_API SoRotor : public SoRotation {
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
