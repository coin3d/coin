/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  int lastoutput;
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
