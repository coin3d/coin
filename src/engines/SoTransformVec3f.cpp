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
  \class SoTransformVec3f SoTransformVec3f.h Inventor/engines/SoTransformVec3f.h
  \brief The SoTransformVec3f class can be used to trasnform 3D vectors by a matrix.
  \ingroup engines
  
  FIXME: doc
*/

#include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>


SO_ENGINE_SOURCE(SoTransformVec3f);

/*!
  Default constructor.
*/
SoTransformVec3f::SoTransformVec3f()
{
  SO_ENGINE_CONSTRUCTOR(SoTransformVec3f);

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
