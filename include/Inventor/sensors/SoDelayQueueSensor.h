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

#ifndef COIN_SODELAYQUEUESENSOR_H
#define COIN_SODELAYQUEUESENSOR_H

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>


class SoDelayQueueSensor : public SoSensor {
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
