/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/engines/SoComposeVec4f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>

SO_ENGINE_SOURCE(SoComposeVec4f);

SoComposeVec4f::SoComposeVec4f()
{
  SO_ENGINE_CONSTRUCTOR(SoComposeVec4f);

  SO_ENGINE_ADD_INPUT(x,(0.0));
  SO_ENGINE_ADD_INPUT(y,(0.0));
  SO_ENGINE_ADD_INPUT(z,(0.0));
  SO_ENGINE_ADD_INPUT(w,(0.0));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec4f);
}

void
SoComposeVec4f::initClass()
{
  SO_ENGINE_INIT_CLASS(SoComposeVec4f,SoEngine,"Engine");
}

//
// private members
//
SoComposeVec4f::~SoComposeVec4f()
{
}

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
