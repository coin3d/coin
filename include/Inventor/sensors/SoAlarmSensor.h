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

#ifndef __SOALARMSENSOR_H__
#define __SOALARMSENSOR_H__

#include <Inventor/sensors/SoTimerQueueSensor.h>

#if defined(COIN_EXCLUDE_SOALARMSENSOR)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOALARMSENSOR

class SoAlarmSensor : public SoTimerQueueSensor {
  typedef SoTimerQueueSensor inherited;

public:
  SoAlarmSensor(void);
  SoAlarmSensor(SoSensorCB * func, void * data);
  virtual ~SoAlarmSensor(void);

  void setTime(const SbTime & absTime);
  void setTimeFromNow(const SbTime & relTime);
  const SbTime & getTime(void) const;

private:
  SbTime alarm;
};

#endif // !__SOALARMSENSOR_H__
