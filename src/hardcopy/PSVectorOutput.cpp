/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoPSVectorOutput HardCopy/SoPSVectorOutput.h
  \brief The SoPSVectorOutput class is used for writing Postscript.
  \ingroup hardcopy

  \since Coin 2.1
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

#include <Inventor/annex/HardCopy/SoPSVectorOutput.h>
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
  PRIVATE(this) = new SoPSVectorOutputP;
  PRIVATE(this)->colored = TRUE;
}

/*!
  Destructor.
*/
SoPSVectorOutput::~SoPSVectorOutput()
{
  delete PRIVATE(this);
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
