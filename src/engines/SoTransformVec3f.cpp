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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTransformVec3f SoTransformVec3f.h Inventor/engines/SoTransformVec3f.h
  \brief The SoTransformVec3f class can be used to trasnform 3D vectors by a matrix.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>


SO_ENGINE_SOURCE(SoTransformVec3f);

/*!
  Default constructor.
*/
SoTransformVec3f::SoTransformVec3f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoTransformVec3f);

  SO_ENGINE_ADD_INPUT(vector, (0.0f, 0.0f, 0.0f));
  SO_ENGINE_ADD_INPUT(matrix, (SbMatrix::identity()));

  SO_ENGINE_ADD_OUTPUT(point, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(direction, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(normalDirection, SoMFVec3f);
}

// doc in parent
void
SoTransformVec3f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoTransformVec3f);
}

// private destructor
SoTransformVec3f::~SoTransformVec3f()
{
}

// doc in parent
void
SoTransformVec3f::evaluate()
{
  int numVec = this->vector.getNum();
  int numMatrix = this->matrix.getNum();

  int numOut = SbMax(numVec, numMatrix);

  SO_ENGINE_OUTPUT(point, SoMFVec3f, setNum(numOut));
  SO_ENGINE_OUTPUT(direction, SoMFVec3f, setNum(numOut));
  SO_ENGINE_OUTPUT(normalDirection, SoMFVec3f, setNum(numOut));

  SbVec3f pt, dir, ndir;

  for (int i = 0; i < numOut; i++) {
    const SbVec3f &v = this->vector[SbMin(i, numVec-1)];
    const SbMatrix &m = this->matrix[SbMin(i, numMatrix-1)];
    m.multVecMatrix(v, pt);
    m.multDirMatrix(v, dir);
    ndir = dir;
    ndir.normalize();
    SO_ENGINE_OUTPUT(point, SoMFVec3f, set1Value(i, pt));
    SO_ENGINE_OUTPUT(direction, SoMFVec3f, set1Value(i, dir));
    SO_ENGINE_OUTPUT(normalDirection, SoMFVec3f, set1Value(i, ndir));
  }
}
