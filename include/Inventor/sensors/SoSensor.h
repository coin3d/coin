#ifndef COIN_SOSENSOR_H
#define COIN_SOSENSOR_H

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

#include <Inventor/SbBasic.h>

class SoSensor;

typedef void SoSensorCB(void * data, SoSensor * sensor);

class COIN_DLL_API SoSensor {
public:
  SoSensor(void);
  SoSensor(SoSensorCB * func, void * data);
  virtual ~SoSensor(void);

  void setFunction(SoSensorCB * callbackfunction);
  SoSensorCB * getFunction(void) const;
  void setData(void * callbackdata);
  void * getData(void) const;

  virtual void schedule(void) = 0;
  virtual void unschedule(void) = 0;
  virtual SbBool isScheduled(void) const = 0;

  virtual void trigger(void);

  virtual SbBool isBefore(const SoSensor * s) const = 0;
  void setNextInQueue(SoSensor * next);
  SoSensor * getNextInQueue(void) const;

  static void initClass(void);

protected:
  SoSensorCB * func;
  void * funcData;
};

#endif // !COIN_SOSENSOR_H
