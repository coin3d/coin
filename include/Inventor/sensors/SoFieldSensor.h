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

#ifndef COIN_SOFIELDSENSOR_H
#define COIN_SOFIELDSENSOR_H

#include <Inventor/sensors/SoDataSensor.h>


class COIN_DLL_EXPORT SoFieldSensor : public SoDataSensor {
  typedef SoDataSensor inherited;

public:
  SoFieldSensor(void);
  SoFieldSensor(SoSensorCB * func, void * data);
  virtual ~SoFieldSensor(void);

  void attach(SoField * field);
  void detach(void);
  SoField * getAttachedField(void) const;

  virtual void trigger(void);

private:
  virtual void notify(SoNotList * l);
  virtual void dyingReference(void);

  SoField * convict;
};

#endif // !COIN_SOFIELDSENSOR_H
