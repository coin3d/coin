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

//$ BEGIN TEMPLATE InterpolateSource(SoInterpolateVec4f, SoMFVec4f, SbVec4f, (0,0,0,0), (0,0,0,0), (val1-val0)*alpha.getValue()+val0)

#include <Inventor/engines/SoInterpolateVec4f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>

SO_ENGINE_SOURCE(SoInterpolateVec4f);

SoInterpolateVec4f::SoInterpolateVec4f()
{
  SO_ENGINE_CONSTRUCTOR(SoInterpolateVec4f);

  SO_ENGINE_ADD_INPUT(input0,(0,0,0,0));
  SO_ENGINE_ADD_INPUT(input1,(0,0,0,0));

  SO_ENGINE_ADD_OUTPUT(output,SoMFVec4f);
}

void
SoInterpolateVec4f::initClass()
{
  SO_ENGINE_INIT_CLASS(SoInterpolateVec4f,SoInterpolate,"Interpolate");
}

//
// private members
//
SoInterpolateVec4f::~SoInterpolateVec4f()
{
}

void
SoInterpolateVec4f::evaluate()
{
  int num0=input0.getNum();
  int num1=input1.getNum();

  int numOut=num0>num1?num0:num1;

  SO_ENGINE_OUTPUT(output,SoMFVec4f,setNum(numOut));

  SbVec4f val0,val1,outVal;
  for (int i=0;i<numOut;i++) {
    val0=i<num0?input0[i]:input0[num0-1];
    val1=i<num1?input1[i]:input1[num1-1];

    outVal=(val1-val0)*alpha.getValue()+val0;
    SO_ENGINE_OUTPUT(output,SoMFVec4f,set1Value(i,outVal));
  }
}
//$ END TEMPLATE InterpolateSource
