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

#include <Inventor/SbVec3f.h>

//$ BEGIN TEMPLATE InterpolateSource(SoInterpolateRotation, SoMFRotation, SbRotation, (SbVec3f(0,0,1),0), (SbVec3f(0,0,1),0), SbRotation::slerp(val0,val1,alpha.getValue()))

#include <Inventor/engines/SoInterpolateRotation.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>

SO_ENGINE_SOURCE(SoInterpolateRotation);

SoInterpolateRotation::SoInterpolateRotation()
{
  SO_ENGINE_CONSTRUCTOR(SoInterpolateRotation);

  SO_ENGINE_ADD_INPUT(input0,(SbVec3f(0,0,1),0));
  SO_ENGINE_ADD_INPUT(input1,(SbVec3f(0,0,1),0));

  SO_ENGINE_ADD_OUTPUT(output,SoMFRotation);
}

void
SoInterpolateRotation::initClass()
{
  SO_ENGINE_INIT_CLASS(SoInterpolateRotation,SoInterpolate,"Interpolate");
}

//
// private members
//
SoInterpolateRotation::~SoInterpolateRotation()
{
}

void
SoInterpolateRotation::evaluate()
{
  int num0=input0.getNum();
  int num1=input1.getNum();

  int numOut=num0>num1?num0:num1;

  SO_ENGINE_OUTPUT(output,SoMFRotation,setNum(numOut));

  SbRotation val0,val1,outVal;
  for (int i=0;i<numOut;i++) {
    val0=i<num0?input0[i]:input0[num0-1];
    val1=i<num1?input1[i]:input1[num1-1];

    outVal=SbRotation::slerp(val0,val1,alpha.getValue());
    SO_ENGINE_OUTPUT(output,SoMFRotation,set1Value(i,outVal));
  }
}
//$ END TEMPLATE InterpolateSource
