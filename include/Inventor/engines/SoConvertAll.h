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

#ifndef COIN_SOCONVERTALL_H
#define COIN_SOCONVERTALL_H

#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/engines/SoEngineOutput.h>


class SoConvertAll : public SoFieldConverter {
  typedef SoFieldConverter inherited;

  // SoConvertAll doesn't have a createInstance() method (because it
  // doesn't have a default constructor), so use the ABSTRACT macros.
  SO_ENGINE_ABSTRACT_HEADER(SoConvertAll);

public:
  static void initClass(void);
  SoConvertAll(const SoType from, const SoType to);

protected:
  virtual ~SoConvertAll();
  virtual void evaluate(void);
  virtual SoField * getInput(SoType type);
  virtual SoEngineOutput * getOutput(SoType type);

private:
  typedef void converter_func(SoField * from, SoField * to);

  static void register_converter(converter_func * f, SoType from, SoType to);

  static SbDict * converter_dict;
  converter_func * convertvalue;

  SoField * input;
  SoEngineOutput output;

  SoFieldData * inputdata_instance;
  SoEngineOutputData * outputdata_instance;
};

#endif // !COIN_SOCONVERTALL_H
