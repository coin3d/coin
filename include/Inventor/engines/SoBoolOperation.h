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

#ifndef COIN_SOBOOLOPERATION_H
#define COIN_SOBOOLOPERATION_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFEnum.h>


class COIN_DLL_API SoBoolOperation : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoBoolOperation);

public:
  enum Operation {
    CLEAR, SET,
    A, NOT_A,
    B, NOT_B,
    A_OR_B, NOT_A_OR_B, A_OR_NOT_B, NOT_A_OR_NOT_B,
    A_AND_B, NOT_A_AND_B, A_AND_NOT_B, NOT_A_AND_NOT_B,
    A_EQUALS_B, A_NOT_EQUALS_B
  };

  SoMFBool a;
  SoMFBool b;
  SoMFEnum operation;

  SoEngineOutput output;  //SoMFBool
  SoEngineOutput inverse; //SoMFBool

  SoBoolOperation();

  static void initClass();

protected:
  ~SoBoolOperation();

private:
  virtual void evaluate();
};

#endif // !COIN_SOBOOLOPERATION_H
