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

class SoEngineOutputData {
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
