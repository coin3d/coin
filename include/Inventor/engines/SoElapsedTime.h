/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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


class COIN_DLL_API SoElapsedTime : public SoEngine {
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
  virtual ~SoElapsedTime(void);

  virtual void writeInstance(SoOutput * out);

private:
  virtual void evaluate(void);
  virtual void inputChanged(SoField * which);

  SbTime pausetime, lasttime, currtime;
  enum {RUNNING, STOPPED, PAUSED} status;
};

#endif // !COIN_SOELAPSEDTIME_H
