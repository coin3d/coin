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

#ifndef COIN_SOCOUNTER_H
#define COIN_SOCOUNTER_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFShort.h>


class COIN_DLL_EXPORT SoCounter : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoCounter);

public:
  SoSFShort min;
  SoSFShort max;
  SoSFShort step;
  SoSFTrigger trigger;
  SoSFShort reset;

  SoEngineOutput output;  //SoSFShort
  SoEngineOutput syncOut; //SoSFTrigger

  SoCounter();

  static void initClass();

protected:
  ~SoCounter();

private:
  virtual void evaluate();
  virtual void inputChanged(SoField * which);

  int numSteps;
  short value;
};

#endif // !COIN_SOCOUNTER_H
