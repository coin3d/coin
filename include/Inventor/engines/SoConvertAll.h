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
  SoField * input;
  SoEngineOutput output;

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
};

#endif // !COIN_SOCONVERTALL_H
