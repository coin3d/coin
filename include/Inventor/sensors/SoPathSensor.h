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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOPATHSENSOR_H__
#define __SOPATHSENSOR_H__

#include <Inventor/sensors/SoDataSensor.h>

#if defined(COIN_EXCLUDE_SOPATHSENSOR)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOPATHSENSOR

class SoPathSensor : public SoDataSensor {
  typedef SoDataSensor inherited;

public:
  SoPathSensor(void);
  SoPathSensor(SoSensorCB * func, void * data);
  virtual ~SoPathSensor(void);

  void attach(SoPath * path);
  void detach(void);
  SoPath * getAttachedPath(void) const;

protected:
  void notify(SoNotList * list);

private:
  virtual void dyingReference(void);
  SoPath * convict;
};

#endif // !__SOPATHSENSOR_H__
