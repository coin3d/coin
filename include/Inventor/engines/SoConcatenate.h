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

#ifndef COIN_SOCONCATENATE_H
#define COIN_SOCONCATENATE_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMField.h>


class COIN_DLL_API SoConcatenate : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoConcatenate);

  enum { NUMINPUTS = 10 }; // Constant.

public:
  static void initClass(void);
  SoConcatenate(SoType inputType);

  SoMField * input[NUMINPUTS];

  SoEngineOutput * output;

private:
  SoConcatenate(void);
  ~SoConcatenate();

  virtual void evaluate(void);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void writeInstance(SoOutput * out);

  void initialize(const SoType inputfieldtype);

  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

  // SoConcatenate instances uses a dynamic set of inputs and outputs,
  // as they are not common for all instances of the class (like for
  // most of the other engines).
  SoFieldData * dynamicinput;
  SoEngineOutputData * dynamicoutput;

  // Avoid a g++/egcs warning due to the private default constructor.
  friend class dummy;
};

#endif // !COIN_SOCONCATENATE_H
