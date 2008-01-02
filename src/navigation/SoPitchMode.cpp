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

#include <assert.h>
#include <stdio.h>

#include <Inventor/events/SoLocation2Event.h>

#include <Inventor/navigation/SoNavigationControl.h>
#include <Inventor/navigation/SoPitchMode.h>

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_NAVIGATION_MODE_SOURCE(SoPitchMode);

void
SoPitchMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoPitchMode, SoNavigationMode);
}

/*!
  Constructor.
*/

SoPitchMode::SoPitchMode(SbName name)
: inherited(name)
{
  PRIVATE(this) = NULL;
  this->set1DValueFunc(SoNavigationMode::getMouseMoveVerticalNormalizedDistance, NULL);
}

/*!
  Destructor.
*/

SoPitchMode::~SoPitchMode(void)
{
}

SoNavigationMode *
SoPitchMode::clone(void) const
{
  return new SoPitchMode(this->getModeName());
}

/*!
  This method handles the pitching motion of the camera, based on user input.
*/

SbBool
SoPitchMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }
  ctrl->restoreCamera();
  float value = this->get1DValue(ctrl);
  ctrl->pitchCamera(value);
  return TRUE;
}

#undef PRIVATE
