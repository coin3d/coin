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
  \class SoTransformVec3f SoTransformVec3f.h Inventor/engines/SoTransformVec3f.h
  \brief The SoTransformVec3f class transforms 3D vectors by a matrix.
  \ingroup engines
*/

#include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

/*!
  \var SoMFVec3f SoTransformVec3f::vector
  Set of 3D vector inputs to transform.
*/
/*!
  \var SoMFMatrix SoTransformVec3f::matrix
  Set of transformation matrices to use on the vectors.
*/
/*!
  \var SoEngineOutput SoTransformVec3f::point
  (SoMFVec3f) Transformed points.
*/
/*!
  \var SoEngineOutput SoTransformVec3f::direction
  (SoMFVec3f) Transformed vector directions.
*/
/*!
  \var SoEngineOutput SoTransformVec3f::normalDirection
  (SoMFVec3f) Normalized transformed vector directions.
*/

SO_ENGINE_SOURCE(SoTransformVec3f);

// doc in parent
void
SoTransformVec3f::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoTransformVec3f);
}

/*!
  Default constructor.
*/
SoTransformVec3f::SoTransformVec3f(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoTransformVec3f);

  SO_ENGINE_ADD_INPUT(vector, (0.0f, 0.0f, 0.0f));
  SO_ENGINE_ADD_INPUT(matrix, (SbMatrix::identity()));

  SO_ENGINE_ADD_OUTPUT(point, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(direction, SoMFVec3f);
  SO_ENGINE_ADD_OUTPUT(normalDirection, SoMFVec3f);
}

/*!
  Destructor is protected because explicit destruction of engines is
  not allowed.
*/
SoTransformVec3f::~SoTransformVec3f()
{
}

// doc in parent
void
SoTransformVec3f::evaluate(void)
{
  int numvec = this->vector.getNum();
  int nummatrices = this->matrix.getNum();

  int numoutputs = SbMax(numvec, nummatrices);

  SO_ENGINE_OUTPUT(point, SoMFVec3f, setNum(numoutputs));
  SO_ENGINE_OUTPUT(direction, SoMFVec3f, setNum(numoutputs));
  SO_ENGINE_OUTPUT(normalDirection, SoMFVec3f, setNum(numoutputs));

  SbVec3f pt, dir, ndir;

  for (int i = 0; i < numoutputs; i++) {
    const SbVec3f & v = this->vector[SbMin(i, numvec-1)];
    const SbMatrix & m = this->matrix[SbMin(i, nummatrices-1)];
    m.multVecMatrix(v, pt);
    m.multDirMatrix(v, dir);
    ndir = dir;
    ndir.normalize();
    SO_ENGINE_OUTPUT(point, SoMFVec3f, set1Value(i, pt));
    SO_ENGINE_OUTPUT(direction, SoMFVec3f, set1Value(i, dir));
    SO_ENGINE_OUTPUT(normalDirection, SoMFVec3f, set1Value(i, ndir));
  }
}
