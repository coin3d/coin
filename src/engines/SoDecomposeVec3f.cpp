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

#include <Inventor/engines/SoDecomposeVec3f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoType.h>

SO_ENGINE_SOURCE(SoDecomposeVec3f);

SoDecomposeVec3f::SoDecomposeVec3f()
{
  SO_ENGINE_CONSTRUCTOR(SoDecomposeVec3f);

  SO_ENGINE_ADD_INPUT(vector,(0,0,0));

  SO_ENGINE_ADD_OUTPUT(x,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(y,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(z,SoMFFloat);
}

void
SoDecomposeVec3f::initClass()
{
  SO_ENGINE_INIT_CLASS(SoDecomposeVec3f,SoEngine,"Engine");
}

//
// private members
//
SoDecomposeVec3f::~SoDecomposeVec3f()
{
}

void
SoDecomposeVec3f::evaluate()
{
  int num=vector.getNum();

  SO_ENGINE_OUTPUT(x,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(y,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(z,SoMFFloat,setNum(num));

  int i;
  for (i=0;i<num;i++) {
    SO_ENGINE_OUTPUT(x,SoMFFloat,set1Value(i,vector[i][0]));
    SO_ENGINE_OUTPUT(y,SoMFFloat,set1Value(i,vector[i][1]));
    SO_ENGINE_OUTPUT(z,SoMFFloat,set1Value(i,vector[i][2]));
  }
}
