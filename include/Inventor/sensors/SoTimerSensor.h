/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOTIMERSENSOR_H__
#define __SOTIMERSENSOR_H__

#include <Inventor/sensors/SoTimerQueueSensor.h>


class SoTimerSensor : public SoTimerQueueSensor {
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
  virtual SbBool isScheduled(void);
  void reschedule(const SbTime & schedtime);

private:
  virtual void trigger(void);

  SbTime base, interval;
  SbBool notsetBaseTime;
  SbBool istriggering, wasunscheduled;
};

#endif // !__SOTIMERSENSOR_H__
