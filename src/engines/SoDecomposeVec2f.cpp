/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoDecomposeVec2f SoCompose.h Inventor/engines/SoCompose.h
  \brief The SoDecomposeVec2f class is used to decompose 2D vectors into two floats.
  \ingroup engines
*/

#include <Inventor/engines/SoDecomposeVec2f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoDecomposeVec2f);

/*!
  \var SoMFVec2f SoDecomposeVec2f::vector
  Set of input vectors to be decomposed into their coordinate
  elements.
*/
/*!
  \var SoEngineOutput SoDecomposeVec2f::x
  (SoMFFloat) First coordinates of the input vectors.
*/
/*!
  \var SoEngineOutput SoDecomposeVec2f::y
  (SoMFFloat) Second coordinates of the input vectors.
*/


#ifndef DOXYGEN_SKIP_THIS // No need to document these.

SoDecomposeVec2f::SoDecomposeVec2f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoDecomposeVec2f);

  SO_ENGINE_ADD_INPUT(vector,(0,0));

  SO_ENGINE_ADD_OUTPUT(x,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(y,SoMFFloat);
}

// Documented in superclass.
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

// Documented in superclass.
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

#endif // !DOXYGEN_SKIP_THIS
