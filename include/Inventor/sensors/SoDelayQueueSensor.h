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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SODELAYQUEUESENSOR_H
#define COIN_SODELAYQUEUESENSOR_H

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>


class COIN_DLL_EXPORT SoDelayQueueSensor : public SoSensor {
  typedef SoSensor inherited;

public:
  SoDelayQueueSensor(void);
  SoDelayQueueSensor(SoSensorCB * func, void * data);
  virtual ~SoDelayQueueSensor(void);

  void setPriority(uint32_t pri);
  uint32_t getPriority(void) const;
  static uint32_t getDefaultPriority(void);
  virtual void schedule(void);
  virtual void unschedule(void);
  virtual SbBool isScheduled(void) const;

  virtual SbBool isIdleOnly(void) const;

protected:
  SbBool scheduled;

private:
  virtual SbBool isBefore(const SoSensor * s) const;
  uint32_t priority;
};

#endif // !COIN_SODELAYQUEUESENSOR_H
