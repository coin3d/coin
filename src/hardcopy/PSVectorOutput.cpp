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
  \class SoPSVectorOutput HardCopy/SoPSVectorOutput.h
  \brief The SoPSVectorOutput class is used for writing Postscript.
  \ingroup hardcopy

  \since Coin 2.1
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

#include <HardCopy/SoPSVectorOutput.h>
#include <Inventor/SbBasic.h>

class SoPSVectorOutputP {
public:
  SbBool colored;
};

#define PRIVATE(p) (p->pimpl)

/*!
  Constructor.
*/
SoPSVectorOutput::SoPSVectorOutput()
{
}

/*!
  Destructor.
*/
SoPSVectorOutput::~SoPSVectorOutput()
{
}

/*!
  Sets whether the geometry should be colored.
*/
void
SoPSVectorOutput::setColored(SbBool flag)
{
  PRIVATE(this)->colored = flag;
}

/*!
  Returns whether geometry is colored.
*/
SbBool
SoPSVectorOutput::getColored(void) const
{
  return PRIVATE(this)->colored;
}

#undef PRIVATE
