#ifndef COIN_SOTIMERSENSOR_H
#define COIN_SOTIMERSENSOR_H

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
  void reschedule(const SbTime & schedtime);

private:
  virtual void trigger(void);

  SbTime base, interval;
  SbBool setbasetime;
  SbBool istriggering;
};

#endif // !COIN_SOTIMERSENSOR_H
