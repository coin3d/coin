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

/*!
  \class SoDecomposeRotation SoDecomposeRotation.h Inventor/engines/SoDecomposeRotation.h
  \brief The SoDecomposeRotation class is used to decompose a rotation into angle and axis.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoDecomposeRotation.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoDecomposeRotation);

/*!
  Default constructor.
*/
SoDecomposeRotation::SoDecomposeRotation()
{
  SO_ENGINE_CONSTRUCTOR(SoDecomposeRotation);

  SO_ENGINE_ADD_INPUT(rotation,(0.0f, 0.0f, 1.0f, 0.0f));

  SO_ENGINE_ADD_OUTPUT(axis,SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(angle,SoMFFloat);
}

// overloaded from parent
void
SoDecomposeRotation::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoDecomposeRotation);
}

//
// private members
//
SoDecomposeRotation::~SoDecomposeRotation()
{
}

// overloaded from parent
void
SoDecomposeRotation::evaluate()
{
  int num = this->rotation.getNum();

  SO_ENGINE_OUTPUT(axis,SoMFVec3f,setNum(num));
  SO_ENGINE_OUTPUT(angle,SoMFFloat,setNum(num));

  int i;
  float angleVal;
  SbVec3f axisVal;
  for (i = 0; i < num; i++) {
    this->rotation[i].getValue(axisVal,angleVal);
    SO_ENGINE_OUTPUT(axis,SoMFVec3f,set1Value(i,axisVal));
    SO_ENGINE_OUTPUT(angle,SoMFFloat,set1Value(i,angleVal));
  }
}
