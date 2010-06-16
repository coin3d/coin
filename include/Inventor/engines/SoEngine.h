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
  virtual ~SoEngine(void);
  virtual void evaluate(void) = 0;

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void inputChanged(SoField * which);

  static const SoFieldData ** getInputDataPtr(void);
  static const SoEngineOutputData ** getOutputDataPtr(void);

  void writeOutputTypes(SoOutput * out);

private:
  virtual void destroy(void); // overrides SoBase::destroy()

  static SoType classTypeId;

  enum InternalEngineFlags {
    FLAG_ISNOTIFYING = (1 << 0),
    FLAG_ISDIRTY = (1 << 1)
  };

  unsigned int flags;

  // needed for handling connections from SoEngineOutput
  friend class SoEngineOutput;
  void setDirty(void);
};

#if !defined(COIN_INTERNAL)
#include <Inventor/engines/SoEngineOutput.h>
#endif // COIN_INTERNAL

#endif // !COIN_SOENGINE_H
