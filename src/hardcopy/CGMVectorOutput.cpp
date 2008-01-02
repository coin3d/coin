/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoCGMVectorOutput HardCopy/SoCGMVectorOutput.h
  \brief The SoCGMVectorOutput class is used for writing in CGM vector format.
  \ingroup hardcopy

  \since Coin 2.4
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

// *************************************************************************

#include <Inventor/annex/HardCopy/SoCGMVectorOutput.h>
#include <Inventor/SbBasic.h>

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out of the build before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SoCGMVectorOutput::SoCGMVectorOutput(void)
{
}

SoCGMVectorOutput::~SoCGMVectorOutput()
{
}

// *************************************************************************

SbBool
SoCGMVectorOutput::openFile(const char * filename)
{
  // FIXME: implement
  return FALSE;
}

// *************************************************************************
 
void
SoCGMVectorOutput::setBinary(SbBool flag)
{
  // FIXME: implement
}

SbBool
SoCGMVectorOutput::isBinary(void) const
{
  // FIXME: implement
  return FALSE;
}
 
void
SoCGMVectorOutput::setVersion(int vers)
{
  // FIXME: implement
}

int
SoCGMVectorOutput::getVersion(void) const
{
  // FIXME: implement
  return 1;
}
 
void
SoCGMVectorOutput::setIndexed(SbBool flag)
{
  // FIXME: implement
}

SbBool
SoCGMVectorOutput::isIndexed(void) const
{
  // FIXME: implement
  return TRUE;
}

// *************************************************************************
