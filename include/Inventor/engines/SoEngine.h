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

#if defined(COIN_EXCLUDE_SOENGINE)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOENGINE

#if defined(__SOLIB_MAKEASBASE__)
// Disable everything except SoEngine.
#define COIN_EXCLUDE_SOBOOLOPERATION 1
#define COIN_EXCLUDE_SOCALCULATOR 1
#define COIN_EXCLUDE_SOCOMPOSEMATRIX 1
#define COIN_EXCLUDE_SOCOMPOSEROTATION 1
#define COIN_EXCLUDE_SOCOMPOSEROTATIONFROMTO 1
#define COIN_EXCLUDE_SOCOMPOSEVEC2F 1
#define COIN_EXCLUDE_SOCOMPOSEVEC3F 1
#define COIN_EXCLUDE_SOCOMPOSEVEC4F 1
#define COIN_EXCLUDE_SOCOMPUTEBOUNDINGBOX 1
#define COIN_EXCLUDE_SOCONCATENATE 1
#define COIN_EXCLUDE_SOCOUNTER 1
#define COIN_EXCLUDE_SODECOMPOSEMATRIX 1
#define COIN_EXCLUDE_SODECOMPOSEROTATION 1
#define COIN_EXCLUDE_SODECOMPOSEVEC2F 1
#define COIN_EXCLUDE_SODECOMPOSEVEC3F 1
#define COIN_EXCLUDE_SODECOMPOSEVEC4F 1
#define COIN_EXCLUDE_SOELAPSEDTIME 1
#define COIN_EXCLUDE_SOFIELDCONVERTER 1
#define COIN_EXCLUDE_SOCONVERTALL 1 // dependent on SoFieldConverter
#define COIN_EXCLUDE_SOGATE 1
#define COIN_EXCLUDE_SOINTERPOLATE 1
#define COIN_EXCLUDE_SOINTERPOLATEFLOAT 1
#define COIN_EXCLUDE_SOINTERPOLATEROTATION 1
#define COIN_EXCLUDE_SOINTERPOLATEVEC2F 1
#define COIN_EXCLUDE_SOINTERPOLATEVEC3F 1
#define COIN_EXCLUDE_SOINTERPOLATEVEC4F 1
#define COIN_EXCLUDE_SOONESHOT 1
#define COIN_EXCLUDE_SOONOFF 1
#define COIN_EXCLUDE_SOSELECTONE 1
#define COIN_EXCLUDE_SOTIMECOUNTER 1
#define COIN_EXCLUDE_SOTRANSFORMVEC3F 1
#define COIN_EXCLUDE_SOTRIGGERANY 1
#endif // __SOLIB_MAKEASBASE__

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
