/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateFunction SoTextureCoordinateFunction.h Inventor/nodes/SoTextureCoordinateFunction.h
  \brief The SoTextureCoordinateFunction class is an abstract base class for texture coordinate generating nodes
  \ingroup nodes

  Classes reimplementing SoTextureCoordinateFunction generate texture coordinates
  by projecting object space surface points using some function.
*/

#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/nodes/SoSubNodeP.h>



// *************************************************************************

SO_NODE_ABSTRACT_SOURCE(SoTextureCoordinateFunction);

/*!
  Constructor.
*/
SoTextureCoordinateFunction::SoTextureCoordinateFunction()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateFunction);
}

/*!
  Destructor.
*/
SoTextureCoordinateFunction::~SoTextureCoordinateFunction()
{
}

// doc in super
void
SoTextureCoordinateFunction::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoTextureCoordinateFunction, SO_FROM_INVENTOR_1);
}





