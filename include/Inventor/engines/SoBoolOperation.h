/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
