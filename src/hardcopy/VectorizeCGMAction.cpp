/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVectorizeCGMAction HardCopy/SoVectorizeCGMAction.h
  \brief The SoVectorizeCGMAction class is used for rendering to CGM format.
  \ingroup hardcopy

  FIXME: docs

  \since Coin 2.4
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

#include <Inventor/annex/HardCopy/SoVectorizeCGMAction.h>

#include <Inventor/errors/SoDebugError.h>
#include "VectorizeActionP.h"

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out of the build before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SO_ACTION_SOURCE(SoVectorizeCGMAction);

// *************************************************************************

// doc in parent
void
SoVectorizeCGMAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoVectorizeCGMAction, SoVectorizeAction);
}

/*!
  Default constructor.
*/
SoVectorizeCGMAction::SoVectorizeCGMAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoVectorizeCGMAction);

  this->setOutput(new SoCGMVectorOutput);

  SoDebugError::postWarning("SoVectorizeCGMAction::SoVectorizeCGMAction",
                            "This class not yet properly implemented, attempts "
                            "at vectorization will be ignored.");
}

/*!
  Default destructor.
*/
SoVectorizeCGMAction::~SoVectorizeCGMAction()
{
}

// *************************************************************************

/*!
  Returns the SoCGMVectorOutput used by this instance.
*/
SoCGMVectorOutput *
SoVectorizeCGMAction::getOutput(void) const
{
  return (SoCGMVectorOutput*)SoVectorizeAction::getOutput();
}

/*!
  Returns the SoCGMVectorOutput used by this instance. Provided
  for API compatibility with TGS HardCopy support.
*/
SoCGMVectorOutput *
SoVectorizeCGMAction::getCGMVectorOutput(void) const
{
  return (SoCGMVectorOutput*)SoVectorizeAction::getOutput();
}

// *************************************************************************
