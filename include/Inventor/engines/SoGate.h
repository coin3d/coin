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
