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
