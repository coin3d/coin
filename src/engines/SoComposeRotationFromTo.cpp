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

/*!
  \class SoComposeRotationFromTo SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoComposeRotationFromTo class is used to compose rotations based on from and to vectors.
  \ingroup engines
*/

#include <Inventor/engines/SoComposeRotationFromTo.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeRotationFromTo);

/*!
  \var SoMFVec3f SoComposeRotationFromTo::from
  Set of input vectors to rotate \e from.
*/
/*!
  \var SoMFVec3f SoComposeRotationFromTo::to
  Set of input vectors to rotate \e to.
*/
/*!
  \var SoEngineOutput SoComposeRotationFromTo::rotation

  (SoMFRotation) Set of output rotations, where each element is the
  rotation you need to apply to the \e from vector (of the same index)
  to make it point in the \e to direction.
*/

#ifndef DOXYGEN_SKIP_THIS // No need to document these.

// Default constructor.
SoComposeRotationFromTo::SoComposeRotationFromTo(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeRotationFromTo);

  SO_ENGINE_ADD_INPUT(from,(0.0f,0.0f,1.0f));
  SO_ENGINE_ADD_INPUT(to,(0.0f,0.0f,1.0f));

  SO_ENGINE_ADD_OUTPUT(rotation,SoMFRotation);
}

// doc in parent
void
SoComposeRotationFromTo::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeRotationFromTo);
}

// private destructor
SoComposeRotationFromTo::~SoComposeRotationFromTo()
{
}

// doc in parent
void
SoComposeRotationFromTo::evaluate(void)
{
  int numFrom = from.getNum();
  int numTo = to.getNum();
  int numOut = SbMax(numFrom, numTo);

  SO_ENGINE_OUTPUT(rotation, SoMFRotation, setNum(numOut));

  for (int i = 0; i < numOut; i++) {
    const SbVec3f fromVec = this->from[SbMin(i, numFrom-1)];
    const SbVec3f toVec = this->to[SbMin(i, numTo-1)];
    SO_ENGINE_OUTPUT(rotation, SoMFRotation, set1Value(i, SbRotation(fromVec, toVec)));
  }
}

#endif // !DOXYGEN_SKIP_THIS
