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

#ifndef COIN_SOTIMECOUNTER_H
#define COIN_SOTIMECOUNTER_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFShort.h>

class COIN_DLL_EXPORT SoTimeCounter : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoTimeCounter);

public:
  SoSFTime timeIn;
  SoSFShort min;
  SoSFShort max;
  SoSFShort step;
  SoSFBool on;
  SoSFFloat frequency;
  SoMFFloat duty;
  SoSFShort reset;
  SoSFTrigger syncIn;

  SoEngineOutput output;  // SoSFShort
  SoEngineOutput syncOut; // SoSFTrigger

  SoTimeCounter();
  static void initClass();

protected:
  virtual ~SoTimeCounter();

private:

  virtual void evaluate(void);
  virtual void inputChanged(SoField * which);

  SbBool prevon;
  SbBool ispaused;
  double pausetimeincycle;
  SbBool firstoutputenable;
  short outputvalue;
  int numsteps;
  int stepnum;
  double starttime;
  double cyclelen;
  short findOutputValue(double timeincycle) const;
  void setOutputValue(short value);
  void calcDutySteps(void);
  void calcNumSteps(void);

  SbList <double> dutylimits;
};

#endif // !COIN_SOTIMECOUNTER_H
