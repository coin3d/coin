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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOTRIGGERANY_H
#define COIN_SOTRIGGERANY_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoSFTrigger.h>


class COIN_DLL_EXPORT SoTriggerAny : public SoEngine {
  typedef SoEngine inherited;
  SO_ENGINE_HEADER(SoTriggerAny);

public:
  SoSFTrigger input0;
  SoSFTrigger input1;
  SoSFTrigger input2;
  SoSFTrigger input3;
  SoSFTrigger input4;
  SoSFTrigger input5;
  SoSFTrigger input6;
  SoSFTrigger input7;
  SoSFTrigger input8;
  SoSFTrigger input9;

  SoEngineOutput output;  // SoSFTrigger

  SoTriggerAny();

  static void initClass();

protected:
  ~SoTriggerAny();

private:
  virtual void evaluate();
};

#endif // !COIN_SOTRIGGERANY_H
