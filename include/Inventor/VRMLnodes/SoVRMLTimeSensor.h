/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLTIMESENSOR_H
#define COIN_SOVRMLTIMESENSOR_H

#include <Inventor/engines/SoNodeEngine.h>
#include <Inventor/engines/SoSubNodeEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>

class SoSensor;

class COIN_DLL_API SoVRMLTimeSensor : public SoNodeEngine
{
  typedef SoNodeEngine inherited;
  SO_NODEENGINE_HEADER(SoVRMLTimeSensor);

public:
  static void initClass(void);
  SoVRMLTimeSensor(void);

  SoSFTime cycleInterval;
  SoSFBool enabled;
  SoSFBool loop;
  SoSFTime startTime;
  SoSFTime stopTime;

  virtual void notify(SoNotList * list);
  virtual void handleEvent(SoHandleEventAction * action);

  SoEngineOutput cycleTime; // (SoSFTime)
  SoEngineOutput fraction_changed; // (SoSFFloat)
  SoEngineOutput isActive; // (SoSFBool)
  SoEngineOutput time; // (SoSFTime)

protected:
  virtual ~SoVRMLTimeSensor();


  virtual void writeInstance(SoOutput * out);
  virtual void inputChanged(SoField * whichInput);

private:
  virtual void evaluate(void);
  SoSFTime timeIn;
  class SoVRMLTimeSensorP * pimpl;
}; // class SoVRMLTimeSensor

#endif // ! COIN_SOVRMLTIMESENSOR_H
