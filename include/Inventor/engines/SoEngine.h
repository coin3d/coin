/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOENGINE_H__
#define __SOENGINE_H__

#include <Inventor/fields/SoFieldContainer.h>

class SoEngineList;
class SoEngineOutput;
class SoEngineOutputList;


class SoEngine : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  static SoType getClassTypeId();
  virtual int getOutputs(SoEngineOutputList & list) const;
  SoEngineOutput * getOutput(const SbName & outputName) const;
  SbBool getOutputName(const SoEngineOutput * output, SbName & outputName) const;
  SoEngine * copy(void) const;
  static SoEngine * getByName(const SbName & name);
  static int getByName(const SbName & name, SoEngineList & list);

  virtual void evaluate() = 0;

  static void initClass();
  static void initEngines();
  void startEvaluate();

protected:
  SoEngineOutputList * outputList;
  virtual void inputChanged(SoField * which);
  virtual void notify(SoNotList * list);

private:
  static SoType classTypeId;

};

#endif // !__SOENGINE_H__
