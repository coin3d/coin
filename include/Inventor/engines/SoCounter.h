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

#ifndef COIN_SOCOUNTER_H
#define COIN_SOCOUNTER_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFShort.h>


class COIN_DLL_API SoCounter : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoCounter);

public:
  static void initClass(void);
  SoCounter(void);

  SoSFShort min;
  SoSFShort max;
  SoSFShort step;
  SoSFTrigger trigger;
  SoSFShort reset;

  SoEngineOutput output;  // SoSFShort
  SoEngineOutput syncOut; // SoSFTrigger

protected:
  virtual ~SoCounter(void);

private:
  virtual void evaluate(void);
  virtual void inputChanged(SoField * which);

  int numsteps;
  short value;
};

#endif // !COIN_SOCOUNTER_H
