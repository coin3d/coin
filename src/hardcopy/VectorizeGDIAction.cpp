/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoVectorizeGDIAction HardCopy/SoVectorizeGDIAction.h
  \brief The SoVectorizeGDIAction class is used for rendering to GDI format.
  \ingroup hardcopy

  FIXME: docs

  \since Coin 2.4
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

#include <Inventor/annex/HardCopy/SoVectorizeGDIAction.h>

#include <Inventor/errors/SoDebugError.h>
#include "VectorizeActionP.h"

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out of the build before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SO_ACTION_SOURCE(SoVectorizeGDIAction);

// *************************************************************************

// doc in parent
void
SoVectorizeGDIAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoVectorizeGDIAction, SoVectorizeAction);
}

SoVectorizeGDIAction::SoVectorizeGDIAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoVectorizeGDIAction);

  this->setOutput(new SoGDIVectorOutput);

  SoDebugError::postWarning("SoVectorizeGDIAction::SoVectorizeGDIAction",
                            "This class not yet properly implemented, attempts "
                            "at vectorization will be ignored.");
}

SoVectorizeGDIAction::~SoVectorizeGDIAction()
{
}

// *************************************************************************

/*!
  Returns the SoGDIVectorOutput used by this instance.
*/
SoGDIVectorOutput *
SoVectorizeGDIAction::getOutput(void) const
{
  return (SoGDIVectorOutput*)SoVectorizeAction::getOutput();
}

/*!
  Returns the SoGDIVectorOutput used by this instance. Provided
  for API compatibility with TGS HardCopy support.
*/
SoGDIVectorOutput *
SoVectorizeGDIAction::getGDIVectorOutput(void) const
{
  return (SoGDIVectorOutput*)SoVectorizeAction::getOutput();
}

// *************************************************************************
