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

#ifndef COIN_SOGATE_H
#define COIN_SOGATE_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFName.h>

class SoEngineOutput;


class COIN_DLL_EXPORT SoGate : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoGate);

public:
  SoGate(SoType type);

  SoSFBool enable;
  SoSFTrigger trigger;
  SoMField * input;

  SoEngineOutput * output;

  static void initClass(void);

protected:
  virtual void inputChanged(SoField * which);
  virtual SbBool readInstance(SoInput *in, unsigned short flags);
  virtual void writeInstance(SoOutput *out);

private:
  friend class dummyClassToStopCompilerNagging;
  SoGate(void);
  ~SoGate();
  void commonConstructor();
  void initInputOutput(const SoType type);

  virtual void evaluate();

  SoFieldData *gateInputData;
  SoEngineOutputData *gateOutputData;

  SoSFName typeField;
};

#endif // !COIN_SOGATE_H
