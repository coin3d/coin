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
  \class SoComposeVec4f SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoComposeVec4f class is used to compose 4D vectors from four floats.
  \ingroup engines
*/

#include <Inventor/engines/SoComposeVec4f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec4f);

/*!
  \var SoMFFloat SoComposeVec4f::x
  First coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec4f::y
  Second coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec4f::z
  Third coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec4f::w
  Fourth coordinates of the output vectors.
*/
/*!
  \var SoEngineOutput SoComposeVec4f::vector
  (SoMFVec4f) 4D vectors.
*/

#ifndef DOXYGEN_SKIP_THIS // No need to document these.

SoComposeVec4f::SoComposeVec4f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeVec4f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));
  SO_ENGINE_ADD_INPUT(z,(0.0f));
  SO_ENGINE_ADD_INPUT(w,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec4f);
}

// Documented in superclass.
void
SoComposeVec4f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeVec4f);
}

//
// private members
//
SoComposeVec4f::~SoComposeVec4f()
{
}

// Documented in superclass.
void
SoComposeVec4f::evaluate()
{
  int numX=x.getNum();
  int numY=y.getNum();
  int numZ=z.getNum();
  int numW=w.getNum();

  int numOut=numX>numY?numX:numY;
  numOut=numZ>numOut?numZ:numOut;
  numOut=numW>numOut?numW:numOut;

  SO_ENGINE_OUTPUT(vector,SoMFVec4f,setNum(numOut));

  int i;
  float xVal,yVal,zVal,wVal;
  for (i=0;i<numOut;i++) {
    xVal=i<numX?x[i]:x[numX-1];
    yVal=i<numY?y[i]:y[numY-1];
    zVal=i<numZ?z[i]:z[numZ-1];
    wVal=i<numW?w[i]:w[numZ-1];

    SO_ENGINE_OUTPUT(vector,SoMFVec4f,set1Value(i,xVal,yVal,zVal,wVal));
  }
}

#endif // !DOXYGEN_SKIP_THIS
