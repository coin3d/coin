#ifndef COIN_SODELAYQUEUESENSOR_H
#define COIN_SODELAYQUEUESENSOR_H

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

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class COIN_DLL_API SoDelayQueueSensor : public SoSensor {
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
  virtual void trigger(void);

protected:
  SbBool scheduled;

private:
  virtual SbBool isBefore(const SoSensor * s) const;
  uint32_t priority;
};

#endif // !COIN_SODELAYQUEUESENSOR_H
