/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoGDIVectorOutput HardCopy/SoGDIVectorOutput.h
  \brief The SoGDIVectorOutput class is used for writing in GDI vector format.
  \ingroup hardcopy

  \since Coin 2.4
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

// *************************************************************************

#include <Inventor/annex/HardCopy/SoGDIVectorOutput.h>
#include <Inventor/SbBasic.h>

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out of the build before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SoGDIVectorOutput::SoGDIVectorOutput(void)
{
}

SoGDIVectorOutput::~SoGDIVectorOutput()
{
}

// *************************************************************************

SbBool
SoGDIVectorOutput::openFile(const char * filename)
{
  // FIXME: implement

  return FALSE;
}

void
SoGDIVectorOutput::openFile(HDC hdc)
{
  // FIXME: implement
}

void
SoGDIVectorOutput::openFile(void)
{
  // FIXME: implement
}


void
SoGDIVectorOutput::closeFile(void)
{
  // FIXME: implement
}

// *************************************************************************
