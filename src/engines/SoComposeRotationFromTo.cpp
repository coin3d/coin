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
  \class SoComposerotationFromTo SoComposerotationFromTo.h Inventor/engines/SoComposerotationFromTo.h
  \brief The SoComposerotationFromTo class is used to compose rotations based on from and to vectors.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoComposeRotationFromTo.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoMFRotation.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeRotationFromTo);

/*!
  Default constructor.
*/
SoComposeRotationFromTo::SoComposeRotationFromTo()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeRotationFromTo);

  SO_ENGINE_ADD_INPUT(from,(0.0f,0.0f,1.0f));
  SO_ENGINE_ADD_INPUT(to,(0.0f,0.0f,1.0f));

  SO_ENGINE_ADD_OUTPUT(rotation,SoMFRotation);
}

// doc in parent
void
SoComposeRotationFromTo::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeRotationFromTo);
}

// private destructor
SoComposeRotationFromTo::~SoComposeRotationFromTo()
{
}

// doc in parent
void
SoComposeRotationFromTo::evaluate()
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
