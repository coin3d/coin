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
  \class SoComposeVec2f SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoComposeVec2f class is used to compose 2D vectors from two floats.
  \ingroup engines
*/

#include <Inventor/engines/SoComposeVec2f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec2f);

/*!
  \var SoMFFloat SoComposeVec2f::x
  First coordinates of the output vectors.
*/
/*!
  \var SoMFFloat SoComposeVec2f::y
  Second coordinates of the output vectors.
*/
/*!
  \var SoEngineOutput SoComposeVec2f::vector
  (SoMFVec2f) 2D vectors.
*/


#ifndef DOXYGEN_SKIP_THIS // No need to document these.

SoComposeVec2f::SoComposeVec2f(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeVec2f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec2f);
}

// Documented in superclass.
void
SoComposeVec2f::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeVec2f);
}

//
// private members
//
SoComposeVec2f::~SoComposeVec2f()
{
}

// Documented in superclass.
void
SoComposeVec2f::evaluate(void)
{
  int numX=x.getNum();
  int numY=y.getNum();

  int numOut=numX>numY?numX:numY;

  SO_ENGINE_OUTPUT(vector,SoMFVec2f,setNum(numOut));

  int i;
  float xVal,yVal;
  for (i=0;i<numOut;i++) {
    xVal=i<numX?x[i]:x[numX-1];
    yVal=i<numY?y[i]:y[numY-1];

    SO_ENGINE_OUTPUT(vector,SoMFVec2f,set1Value(i,xVal,yVal));
  }
}

#endif // !DOXYGEN_SKIP_THIS
