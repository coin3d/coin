/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOSELECTONE_H
#define COIN_SOSELECTONE_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSFInt32.h>

class SoEngineOutput;


class COIN_DLL_API SoSelectOne : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoSelectOne);

public:
  SoSFInt32 index;
  SoMField * input;

  SoEngineOutput * output; // The SoSField-version of the SoMField-type.

  SoSelectOne(SoType inputtype);
  static void initClass(void);

private:
  SoSelectOne(void);
  ~SoSelectOne();

  virtual void evaluate(void);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void writeInstance(SoOutput * out);

  void initialize(const SoType inputfieldtype);

  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

  // SoSelectOne instances uses a dynamic set of inputs and outputs,
  // as they are not common for all instances of the class (like for
  // most of the other engines).
  SoFieldData * dynamicinput;
  SoEngineOutputData * dynamicoutput;

  // Avoid a g++/egcs warning on the private default constructor.
  friend class dummy;
};

#endif // !COIN_SOSELECTONE_H
