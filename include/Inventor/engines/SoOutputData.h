/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_OUTPUTDATA_H
#define COIN_OUTPUTDATA_H

#include <Inventor/lists/SbList.h>

class SoEngine;
class SoEngineOutput;
class SbName;
class SoInput;
class SoOutput;
class SoOutputDataEntry;
class SoType;

class COIN_DLL_API SoEngineOutputData {
public:
  SoEngineOutputData();
  SoEngineOutputData(const SoEngineOutputData *data);
  SoEngineOutputData(int approxnum);
  ~SoEngineOutputData();

  void addOutput(const SoEngine *base, const char *name,
                 const SoEngineOutput *output, SoType type);

  int getNumOutputs(void) const;
  const SbName & getOutputName(int index) const;
  SoEngineOutput * getOutput(const SoEngine *engine, int index) const;
  int getIndex(const SoEngine *engine, const SoEngineOutput *output) const;
  const SoType & getType(int index) const;
  SbBool readDescriptions(SoInput *in, SoEngine *engine) const;
  void writeDescriptions(SoOutput *out, SoEngine *engine) const;

private:
  SbList <SoOutputDataEntry*> outputlist;
};

#endif // !COIN_OUTPUTDATA_H
