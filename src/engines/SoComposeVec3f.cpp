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

/*!
  \class SoComposeVec3f SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoComposeVec3f class is used to compose 3D vectors from floats.
  \ingroup engines
*/

#include <Inventor/engines/SoComposeVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec3f);

/*!
  \var SoMFFloat SoComposeVec3f::x
  First coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec3f::y
  Second coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec3f::z
  Third coordinates of the output vectors.
*/
/*!
  \var SoEngineOutput SoComposeVec3f::vector
  (SoMFVec3f) 3D vectors.
*/

#ifndef DOXYGEN_SKIP_THIS // No need to document these.

SoComposeVec3f::SoComposeVec3f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeVec3f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));
  SO_ENGINE_ADD_INPUT(z,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec3f);
}

// Documented in superclass.
void
SoComposeVec3f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeVec3f);
}

//
// private members
//
SoComposeVec3f::~SoComposeVec3f()
{
}

// Documented in superclass.
void
SoComposeVec3f::evaluate()
{
  int numX=x.getNum();
  int numY=y.getNum();
  int numZ=z.getNum();

  int numOut=numX>numY?numX:numY;
  numOut=numZ>numOut?numZ:numOut;

  SO_ENGINE_OUTPUT(vector,SoMFVec3f,setNum(numOut));

  int i;
  float xVal,yVal,zVal;
  for (i=0;i<numOut;i++) {
    xVal=i<numX?x[i]:x[numX-1];
    yVal=i<numY?y[i]:y[numY-1];
    zVal=i<numZ?z[i]:z[numZ-1];

    SO_ENGINE_OUTPUT(vector,SoMFVec3f,set1Value(i,xVal,yVal,zVal));
  }
}

#endif // !DOXYGEN_SKIP_THIS
