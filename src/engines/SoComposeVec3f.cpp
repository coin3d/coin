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
  \class SoComposeVec3f SoComposeVec3f.h Inventor/engines/SoComposeVec3f.h
  \brief The SoComposeVec3f class is used to compose 3D coordinates from three floats.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoComposeVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec3f);

/*!
  Default constructor.
*/
SoComposeVec3f::SoComposeVec3f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeVec3f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));
  SO_ENGINE_ADD_INPUT(z,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec3f);
}

// overloaded from parent
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

// overloaded from parent
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
