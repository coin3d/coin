/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_OUTPUTDATA_H
#define COIN_OUTPUTDATA_H

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
  SoEngineOutputData();
  SoEngineOutputData(const SoEngineOutputData * data);
  SoEngineOutputData(int approxnum);
  ~SoEngineOutputData();

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
  SbBool readDescriptions(SoInput *in, SoEngine * engine) const;
  void writeDescriptions(SoOutput *out, SoEngine * engine) const;

private:

  void addOutputInternal(const SoFieldContainer * base, const char *name,
                         const SoEngineOutput * output, SoType type);
  SoEngineOutput * getOutputInternal(const SoFieldContainer * base, int index) const;
  int getIndexInternal(const SoFieldContainer * base, const SoEngineOutput * output) const;

  SbList <SoOutputDataEntry*> outputlist;
};

#endif // !COIN_OUTPUTDATA_H
