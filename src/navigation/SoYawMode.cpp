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

#include <Inventor/events/SoLocation2Event.h>

#include <Inventor/navigation/SoNavigationControl.h>
#include <Inventor/navigation/SoYawMode.h>

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_NAVIGATION_MODE_SOURCE(SoYawMode);

void
SoYawMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoYawMode, SoNavigationMode);
}

SoYawMode::SoYawMode(SbName name)
 : inherited(name)
{
  PRIVATE(this) = NULL;
  this->set1DValueFunc(SoNavigationMode::getMouseMoveVerticalNormalizedDistance, NULL);
}

SoYawMode::~SoYawMode(void)
{
}

SoNavigationMode *
SoYawMode::clone(void) const
{
  return new SoYawMode(this->getModeName());
}

SbBool
SoYawMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }
  ctrl->restoreCamera();
  float value = this->get1DValue(ctrl);
  ctrl->yawCamera(value);
  return TRUE;
}

#undef PRIVATE
