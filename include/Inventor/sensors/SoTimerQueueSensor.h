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

#ifndef COIN_SOTIMERQUEUESENSOR_H
#define COIN_SOTIMERQUEUESENSOR_H

#include <Inventor/sensors/SoSensor.h>
#include <Inventor/SbTime.h>


class COIN_DLL_API SoTimerQueueSensor : public SoSensor {
  typedef SoSensor inherited;

public:
  SoTimerQueueSensor(void);
  SoTimerQueueSensor(SoSensorCB * func, void * data);
  virtual ~SoTimerQueueSensor(void);

  const SbTime & getTriggerTime(void) const;
  virtual void schedule(void);
  virtual void unschedule(void);
  virtual SbBool isScheduled(void) const;
  virtual void trigger(void);

protected:
  void setTriggerTime(const SbTime & time);
  SbBool scheduled;

private:
  virtual SbBool isBefore(const SoSensor * s) const;
  SbTime triggertime;
};

#endif // !COIN_SOTIMERQUEUESENSOR_H
