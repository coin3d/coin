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

#ifndef __SOBOOLOPERATION_H__
#define __SOBOOLOPERATION_H__

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFEnum.h>


class SoBoolOperation : public SoEngine {
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

#endif // !__SOBOOLOPERATION_H__
