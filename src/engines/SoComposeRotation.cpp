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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#include <Inventor/engines/SoComposeRotation.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>
#include <Inventor/fields/SoMFRotation.h>

SO_ENGINE_SOURCE(SoComposeRotation);

SoComposeRotation::SoComposeRotation()
{
  SO_ENGINE_CONSTRUCTOR(SoComposeRotation);

  SO_ENGINE_ADD_INPUT(axis,(0,0,0));
  SO_ENGINE_ADD_INPUT(angle,(0));

  SO_ENGINE_ADD_OUTPUT(rotation,SoMFRotation);
}

void
SoComposeRotation::initClass()
{
  SO_ENGINE_INIT_CLASS(SoComposeRotation,SoEngine,"Engine");
}

//
// private members
//
SoComposeRotation::~SoComposeRotation()
{
}

void
SoComposeRotation::evaluate()
{
  int numAxis=axis.getNum();
  int numAngle=angle.getNum();
  
  int numOut=numAxis>numAngle?numAxis:numAngle;
  
  SO_ENGINE_OUTPUT(rotation,SoMFRotation,setNum(numOut));

  int i;
  
  float angleVal;
  for (i=0;i<numOut;i++) {
    const SbVec3f axisVal=i<numAxis?axis[i]:axis[numAxis-1];
    angleVal=i<numAngle?angle[i]:angle[numAngle-1];
 
    SO_ENGINE_OUTPUT(rotation,SoMFRotation,set1Value(i,axisVal,angleVal));
  }
}
