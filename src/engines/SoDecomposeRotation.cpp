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

/*!
  \class SoDecomposeRotation SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoDecomposeRotation class is used to decompose a rotation into angle and axis.
  \ingroup engines
*/

#include <Inventor/engines/SoDecomposeRotation.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoDecomposeRotation);

/*!
  \var SoMFRotation SoDecomposeRotation::rotation
  Input rotations to decompose into axis + angle values.
*/
/*!
  \var SoEngineOutput SoDecomposeRotation::axis
  (SoMFVec3f) The axis settings of the input rotations.
*/
/*!
  \var SoEngineOutput SoDecomposeRotation::angle
  (SoMFFloat) The angle values of the input rotations.
*/

#ifndef DOXYGEN_SKIP_THIS // No need to document these.

// Default constructor.
SoDecomposeRotation::SoDecomposeRotation()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoDecomposeRotation);
  
  SO_ENGINE_ADD_INPUT(rotation, (0.0f, 0.0f, 0.0f, 1.0f));
  
  SO_ENGINE_ADD_OUTPUT(axis, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(angle, SoMFFloat);
}

// Documented in superclass.
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

// Documented in superclass.
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

#endif // !DOXYGEN_SKIP_THIS
