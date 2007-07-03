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

#include <assert.h>
#include <stdio.h>

#include <Inventor/navigation/SoIdleMode.h>

// *************************************************************************

/*!
  \class SoIdleMode Inventor/navigation/SoIdleMode.h
  \brief Class implementing an idle mode.

  This class implements an idle navigation mode.

  \ingroup navigation
*/

SO_NAVIGATION_MODE_SOURCE(SoIdleMode);

void
SoIdleMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoIdleMode, SoNavigationMode);
}

/*!
  Constructor.
*/

SoIdleMode::SoIdleMode(SbName modename)
  : inherited(modename)
{
  this->pimpl = NULL;
}

/*!
  Destructor.
*/

SoIdleMode::~SoIdleMode(void)
{
}

/*!
*/

SoNavigationMode *
SoIdleMode::clone(void) const
{
  return new SoIdleMode(this->getModeName());
}

/*!
  Implements idling.  Returns always FALSE.
*/

SbBool
SoIdleMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  return FALSE;
}

