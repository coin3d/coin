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

#ifndef COIN_SOELAPSEDTIME_H
#define COIN_SOELAPSEDTIME_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>


class COIN_DLL_EXPORT SoElapsedTime : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoElapsedTime);

public:
  static void initClass(void);
  SoElapsedTime(void);

  SoSFTime timeIn;
  SoSFFloat speed;
  SoSFBool on;
  SoSFBool pause;
  SoSFTrigger reset;

  SoEngineOutput timeOut; // SoSFTime

protected:
  ~SoElapsedTime();

private:
  virtual void evaluate(void);
  virtual void inputChanged(SoField * which);

  SbTime pausetime, lasttime, currtime;
  enum {RUNNING, STOPPED, PAUSED} status;
};

#endif // !COIN_SOELAPSEDTIME_H
