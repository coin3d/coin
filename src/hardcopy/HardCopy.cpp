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

// *************************************************************************

/*!
  \page hardcopy_overview An overview of the hardcopy support.


  FIXME: document HardCopy extension with:

    * features

    * usage example (just link to
      <URL:http://source.coin3d.org/SoGuiExamples/annex/hardcopy.cpp.in>?)

    * list of current limitations

  20030704 mortene.


  \since Coin 2.1
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

// *************************************************************************

/*!
  \class SoHardCopy SoHardCopy.h HardCopy/SoHardCopy.h
  \brief The SoHardCopy class is a static class for initializing the hardcopy support.
  \ingroup hardcopy
*/

// *************************************************************************

#include <HardCopy/SoHardCopy.h>
#include <HardCopy/SoVectorizePSAction.h>

// *************************************************************************

/*!
  Initialization of the hardcopy support happens automatically from
  SoDB::init(), so the application programmer should normally not have
  to worry about it.
*/
void
SoHardCopy::init(void)
{
  SoVectorizeAction::initClass();
  SoVectorizePSAction::initClass();
}

/*!
  Returns name of the hardcopy extension.
*/
const char *
SoHardCopy::getProductName(void)
{
  return "Systems in Motion hard-copy support for Coin.";
}

/*!
  Version number will always match Coin version number.
*/
const char *
SoHardCopy::getVersion(void)
{
  return COIN_VERSION;
}

// *************************************************************************
