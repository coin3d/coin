/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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

class COIN_DLL_API SoTimeCounter : public SoEngine {
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

  virtual void inputChanged(SoField * which);
  virtual void writeInstance(SoOutput * out);

private:
  virtual void evaluate(void);
  void calcStarttime(short value);

  SbBool ispaused;
  double pausetimeincycle;
  SbBool firstoutputenable;
  short outputvalue;
  int numsteps;
  int stepnum;
  double starttime;
  double cyclelen;
  short findOutputValue(double timeincycle) const;
  void calcDutySteps(void);
  void calcNumSteps(void);

  SbList <double> dutylimits;
};

#endif // !COIN_SOTIMECOUNTER_H
