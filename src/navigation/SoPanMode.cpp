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

#include <stdio.h>
#include <assert.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoCamera.h>

#include <Inventor/navigation/SoNavigationControl.h>

#include <Inventor/navigation/SoPanMode.h>

// *************************************************************************

/*!
  \class SoPanMode Inventor/navigation/SoPanMode.h
  \brief Class that implements the panning mode.

  This class implements the panning mode.

  \ingroup navigation
*/

SO_NAVIGATION_MODE_SOURCE(SoPanMode);

void
SoPanMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoPanMode, SoNavigationMode);
}

/*!
  Constructor.
*/

SoPanMode::SoPanMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  this->pimpl = NULL;
}

/*!
  Destructor.
*/

SoPanMode::~SoPanMode(void)
{
}

SoNavigationMode *
SoPanMode::clone(void) const
{
  return new SoPanMode(this->getModeName());
}

/*!
  This method implements the panning operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
SoPanMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  SoCamera * camera = ctrl->getCamera();
  if (!camera) {
    return FALSE;
  }

  // Find projection points for the current and the reference mouse
  // coordinates.
  SbViewVolume vv = camera->getViewVolume(ctrl->getViewportAspect());
  SbPlane panningplane = vv.getPlane(camera->focalDistance.getValue());
  SbLine line;
  SbVec3f current_planept, initial_planept;
  vv.projectPointToLine(this->getCurrentNormalizedPosition(ctrl), line);
  panningplane.intersect(line, current_planept);
  vv.projectPointToLine(this->getInitialNormalizedPosition(ctrl), line);
  panningplane.intersect(line, initial_planept);

  ctrl->restoreCamera();
  ctrl->moveCamera(initial_planept - current_planept);

  return TRUE;
}
