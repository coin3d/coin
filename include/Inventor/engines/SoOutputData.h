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

#ifndef COIN_OUTPUTDATA_H
#define COIN_OUTPUTDATA_H

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>

class SoEngine;
class SoNodeEngine;
class SoFieldContainer;
class SoEngineOutput;
class SbName;
class SoInput;
class SoOutput;
class SoOutputDataEntry;
class SoType;

class COIN_DLL_API SoEngineOutputData {
public:
  SoEngineOutputData(void);
  SoEngineOutputData(const SoEngineOutputData * data);
  SoEngineOutputData(int approxnum);
  ~SoEngineOutputData(void);

  void addOutput(const SoEngine * base, const char *name,
                 const SoEngineOutput * output, SoType type);
  void addOutput(const SoNodeEngine * base, const char *name,
                 const SoEngineOutput * output, SoType type);

  int getNumOutputs(void) const;
  const SbName & getOutputName(int index) const;
  SoEngineOutput * getOutput(const SoEngine * engine, int index) const;
  SoEngineOutput * getOutput(const SoNodeEngine * engine, int index) const;
  int getIndex(const SoEngine * engine, const SoEngineOutput * output) const;
  int getIndex(const SoNodeEngine * engine, const SoEngineOutput * output) const;
  const SoType & getType(int index) const;
  SbBool readDescriptions(SoInput * input, SoEngine * engine) const;
  void writeDescriptions(SoOutput * out, SoEngine * engine) const;

private:
  SbBool hasOutput(const char * name) const;
  void addOutputInternal(const SoFieldContainer * base, const char *name,
                         const SoEngineOutput * output, SoType type);
  SoEngineOutput * getOutputInternal(const SoFieldContainer * base, int index) const;
  int getIndexInternal(const SoFieldContainer * base, const SoEngineOutput * output) const;

  SbList <SoOutputDataEntry*> outputlist;
};

#endif // !COIN_OUTPUTDATA_H
