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

#ifndef __SONODESENSOR_H__
#define __SONODESENSOR_H__

#include <Inventor/sensors/SoDataSensor.h>

#if defined(COIN_EXCLUDE_SONODESENSOR)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SONODESENSOR

class SoNodeSensor : public SoDataSensor {
  typedef SoDataSensor inherited;

public:
  SoNodeSensor(void);
  SoNodeSensor(SoSensorCB * func, void * data);
  virtual ~SoNodeSensor(void);

  void attach(SoNode * node);
  void detach(void);
  SoNode * getAttachedNode(void) const;

private:
  virtual void dyingReference(void);
  SoNode * convict;
};

#endif // !__SONODESENSOR_H__
