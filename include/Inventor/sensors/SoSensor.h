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

#ifndef __SOSENSOR_H__
#define __SOSENSOR_H__

#include <Inventor/SbBasic.h>

class SoSensor;

typedef void SoSensorCB(void * data, SoSensor * sensor);

class SoSensor {
public:
  SoSensor(void);
  SoSensor(SoSensorCB * func, void * data);
  virtual ~SoSensor(void);

  void setFunction(SoSensorCB * callbackfunction);
  SoSensorCB * getFunction(void) const;
  void setData(void * callbackData);
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

#endif // !__SOSENSOR_H__
