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

#ifndef COIN_SOTIMERSENSOR_H
#define COIN_SOTIMERSENSOR_H

#include <Inventor/sensors/SoTimerQueueSensor.h>


class COIN_DLL_API SoTimerSensor : public SoTimerQueueSensor {
  typedef SoTimerQueueSensor inherited;

public:
  SoTimerSensor(void);
  SoTimerSensor(SoSensorCB * func, void * data);
  virtual ~SoTimerSensor(void);

  void setBaseTime(const SbTime & base);
  const SbTime & getBaseTime(void) const;
  void setInterval(const SbTime & interval);
  const SbTime & getInterval(void) const;

  virtual void schedule(void);
  virtual void unschedule(void);
  virtual SbBool isScheduled(void) const;
  void reschedule(const SbTime & schedtime);

private:
  virtual void trigger(void);

  SbTime base, interval;
  SbBool setbasetime;
  SbBool istriggering, wasunscheduled;
};

#endif // !COIN_SOTIMERSENSOR_H
