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
  \class SoComposeVec2f SoComposeVec2f.h Inventor/engines/SoComposeVec2f.h
  \brief The SoComposeVec2f class is used to compose 2D coordinates from two floats.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoComposeVec2f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec2f);

/*!
  Default constructor.
*/
SoComposeVec2f::SoComposeVec2f()
{
  SO_ENGINE_CONSTRUCTOR(SoComposeVec2f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec2f);
}

// overloaded from parent
void
SoComposeVec2f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeVec2f);
}

//
// private members
//
SoComposeVec2f::~SoComposeVec2f()
{
}

// overloaded from parent
void
SoComposeVec2f::evaluate()
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
