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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSELECTONE_H
#define COIN_SOSELECTONE_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSFInt32.h>

class SoEngineOutput;


class COIN_DLL_EXPORT SoSelectOne : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoSelectOne);

public:
  SoSFInt32 index;
  SoMField * input;

  SoEngineOutput * output;

  SoSelectOne(SoType inputType);

  static void initClass();

private:
  SoSelectOne(void);
  ~SoSelectOne();
  virtual void evaluate();

  // Avoid a g++/egcs warning.
  friend class dummy;
};

#endif // !COIN_SOSELECTONE_H
