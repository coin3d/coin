/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoDecomposeVec2f SoDecomposeVec2f.h Inventor/engines/SoDecomposeVec2f.h
  \brief The SoDecomposeVec2f class is used to decompose 2D vectors into two floats.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoDecomposeVec2f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoDecomposeVec2f);

/*!
  Default constructor.
*/
SoDecomposeVec2f::SoDecomposeVec2f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoDecomposeVec2f);

  SO_ENGINE_ADD_INPUT(vector,(0,0));

  SO_ENGINE_ADD_OUTPUT(x,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(y,SoMFFloat);
}

// overloaded from parent
void
SoDecomposeVec2f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoDecomposeVec2f);
}

//
// private members
//
SoDecomposeVec2f::~SoDecomposeVec2f()
{
}

// overloaded from parent
void
SoDecomposeVec2f::evaluate()
{
  int num = this->vector.getNum();

  SO_ENGINE_OUTPUT(x,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(y,SoMFFloat,setNum(num));

  int i;
  for (i = 0; i < num; i++) {
    SO_ENGINE_OUTPUT(x,SoMFFloat,set1Value(i,vector[i][0]));
    SO_ENGINE_OUTPUT(y,SoMFFloat,set1Value(i,vector[i][1]));
  }
}
