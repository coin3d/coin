#ifndef COIN_SOCONVERTALL_H
#define COIN_SOCONVERTALL_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/engines/SoEngineOutput.h>

// *************************************************************************

class SoConvertAll : public SoFieldConverter {
  typedef SoFieldConverter inherited;

  // SoConvertAll doesn't have a createInstance() method (because it
  // doesn't have a default constructor), so use the ABSTRACT macros.
  SO_ENGINE_ABSTRACT_HEADER(SoConvertAll);

public:
  static void initClass(void);
  SoConvertAll(const SoType from, const SoType to);

protected:
  virtual ~SoConvertAll(void);
  virtual void evaluate(void);
  virtual SoField * getInput(SoType type);
  virtual SoEngineOutput * getOutput(SoType type);

private:
  typedef void converter_func(SoField * from, SoField * to);
  converter_func * convertvalue;

  SoField * input;
  SoEngineOutput output;

  SoFieldData * inputdata_instance;
  SoEngineOutputData * outputdata_instance;
};

#endif // !COIN_SOCONVERTALL_H
