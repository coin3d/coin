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

#ifndef COIN_SOENGINE_H
#define COIN_SOENGINE_H

#include <Inventor/fields/SoFieldContainer.h>

class SoEngineList;
class SoEngineOutput;
class SoEngineOutputList;
class SoEngineOutputData;

class COIN_DLL_API SoEngine : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  static void initClass(void);
  static void initClasses(void);
  static SoType getClassTypeId(void);

  void evaluateWrapper(void);

  virtual int getOutputs(SoEngineOutputList & l) const;
  SoEngineOutput * getOutput(const SbName & outputname) const;
  SbBool getOutputName(const SoEngineOutput * output, SbName & outputname) const;
  virtual const SoEngineOutputData * getOutputData(void) const = 0;
  static SoEngine * getByName(const SbName & name);
  static int getByName(const SbName & name, SoEngineList & el);

  SbBool isNotifying(void) const;
  virtual void notify(SoNotList * nl);

  SoEngine * copy(void) const;
  virtual SoFieldContainer * copyThroughConnection(void) const;
  SbBool shouldCopy(void) const;

  virtual void writeInstance(SoOutput * out);


protected:
  SoEngine(void);
  virtual ~SoEngine();
  virtual void evaluate(void) = 0;

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void inputChanged(SoField * which);

  static const SoFieldData ** getInputDataPtr(void);
  static const SoEngineOutputData ** getOutputDataPtr(void);

  void writeOutputTypes(SoOutput * out);

private:
  virtual void destroy(void); // overloads SoBase::destroy()

  static SoType classTypeId;
  // FIXME: the ifndef wrapper is a workaround for a bug in Doxygen
  // 1.0.0, where private members in a structure doesn't "inherit" the
  // private status of their "parent". (This has been confirmed to be
  // a bug by Dimitri.) Remove the workaround when a fixed Doxygen
  // appears. 20000201 mortene.
#ifndef DOXYGEN_SKIP_THIS
  struct {
    unsigned int isnotifying : 1;
  } stateflags;
#endif // !DOXYGEN_SKIP_THIS
};

#if !defined(COIN_INTERNAL)
#include <Inventor/engines/SoEngineOutput.h>
#endif // COIN_INTERNAL

#endif // !COIN_SOENGINE_H
