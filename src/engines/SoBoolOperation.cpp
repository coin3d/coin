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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoBoolOperation SoBoolOperation.h Inventor/engines/SoBoolOperation.h
  \brief The SoBoolOperation class performs boolean operations.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoBoolOperation.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_ENGINE_SOURCE(SoBoolOperation);

/*!
  Defalt constructor.
*/
SoBoolOperation::SoBoolOperation(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoBoolOperation);

  SO_ENGINE_ADD_INPUT(a,(FALSE));
  SO_ENGINE_ADD_INPUT(b,(FALSE));
  SO_ENGINE_ADD_INPUT(operation,(SoBoolOperation::A));

  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, CLEAR);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, SET);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_A);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_OR_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_A_OR_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_OR_NOT_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_A_OR_NOT_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_AND_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_A_AND_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_AND_NOT_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, NOT_A_AND_NOT_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_EQUALS_B);
  SO_ENGINE_DEFINE_ENUM_VALUE(Operation, A_NOT_EQUALS_B);

  SO_ENGINE_SET_MF_ENUM_TYPE(operation, Operation);

  SO_ENGINE_ADD_OUTPUT(output, SoMFBool);
  SO_ENGINE_ADD_OUTPUT(inverse, SoMFBool);
}

// doc from parent
void
SoBoolOperation::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoBoolOperation);
}

/*!
  Destructor.
*/
SoBoolOperation::~SoBoolOperation()
{
}

// doc from parent
void
SoBoolOperation::evaluate(void)
{
  int numA = this->a.getNum();
  int numB = this->b.getNum();
  int numOp = this->operation.getNum();

  int numOut = SbMax(SbMax(numA, numB), numOp);

  SO_ENGINE_OUTPUT(output,SoMFBool,setNum(numOut));
  SO_ENGINE_OUTPUT(inverse,SoMFBool,setNum(numOut));

  for (int i = 0; i < numOut; i++) {
    SbBool tmp_a = i < numA ? this->a[i] : this->a[numA-1];
    SbBool tmp_b = i < numB ? this->b[i] : this->b[numB-1];
    int tmp_op = i < numOp ? this->operation[i] : this->operation[numOp-1];

    SbBool val;

    switch (tmp_op) {
    case CLEAR:
      val = FALSE;
      break;
    case SET:
      val = TRUE;
      break;
    case A:
      val = tmp_a;
      break;
    case NOT_A:
      val = !tmp_a;
      break;
    case B:
      val = tmp_b;
      break;
    case NOT_B:
      val = !tmp_b;
      break;
    case A_OR_B:
      val = tmp_a || tmp_b;
      break;
    case NOT_A_OR_B:
      val = !tmp_a || tmp_b;
      break;
    case A_OR_NOT_B:
      val = tmp_a || !tmp_b;
      break;
    case NOT_A_OR_NOT_B:
      val = !tmp_a || !tmp_b;
      break;
    case A_AND_B:
      val = tmp_a && tmp_b;
      break;
    case NOT_A_AND_B:
      val = !tmp_a && tmp_b;
      break;
    case A_AND_NOT_B:
      val = tmp_a && !tmp_b;
      break;
    case NOT_A_AND_NOT_B:
      val = !tmp_a && !tmp_b;
      break;
    case A_EQUALS_B:
      val = (tmp_a==tmp_b);
      break;
    case A_NOT_EQUALS_B:
      val = (tmp_a != tmp_b);
      break;
    default:
#if COIN_DEBUG
      SoDebugError::postInfo("SoBoolOperation::evaluate",
                             "unknown bool operation");
#endif // COIN_DEBUG
      val = TRUE; // avoid compiler warning
      break;
    }

    SO_ENGINE_OUTPUT(output, SoMFBool, set1Value(i, val));
    SO_ENGINE_OUTPUT(inverse, SoMFBool, set1Value(i, !val));
  }
}
