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

#ifndef COIN_SOENGINE_H
#define COIN_SOENGINE_H

#include <Inventor/fields/SoFieldContainer.h>

class SoEngineList;
class SoEngineOutput;
class SoEngineOutputList;
class SoEngineOutputData;

class SoEngine : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  void evaluateWrapper();

protected:
  SoEngine();
  virtual ~SoEngine();
  virtual void evaluate() = 0;

protected:

  virtual SbBool readInstance(SoInput *in, unsigned short flags);
  virtual void inputChanged(SoField * which);

  static const SoFieldData ** getInputDataPtr(void);
  static const SoEngineOutputData ** getOutputDataPtr(void);

  void writeOutputTypes(SoOutput *out);

public:
  static SoType getClassTypeId(void);
  virtual int getOutputs(SoEngineOutputList & l) const;

  SoEngineOutput * getOutput(const SbName & outputname) const;
  SbBool getOutputName(const SoEngineOutput * output, SbName & outputname) const;
  SoEngine * copy(void) const;
  static SoEngine * getByName(const SbName & name);
  static int getByName(const SbName & name, SoEngineList & list);

  static void initClass(void);
  static void initClasses(void);

  virtual void notify(SoNotList * list);
  virtual void writeInstance(SoOutput *out);
  virtual SoFieldContainer * copyThroughConnection(void) const;
  virtual const SoEngineOutputData *getOutputData(void) const = 0;
  SbBool shouldCopy(void) const;
  SbBool isNotifying(void) const;

private:
  static SoType classTypeId;
  struct {
    unsigned int dirty : 1;
    unsigned int isnotifying : 1;
  } stateflags;

  friend class SoEngineOutput;
};

#if !defined(COIN_INTERNAL)
#include <Inventor/engines/SoEngineOutput.h>
#endif // COIN_INTERNAL

#endif // !COIN_SOENGINE_H
