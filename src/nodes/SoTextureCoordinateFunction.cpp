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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateFunction SoTextureCoordinateFunction.h Inventor/nodes/SoTextureCoordinateFunction.h
  \brief The SoTextureCoordinateFunction class ...
  \ingroup nodes

  FIXME: write class doc
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

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateFunction class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateFunction::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoTextureCoordinateFunction);
}
