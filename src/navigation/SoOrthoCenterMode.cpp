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

#include <stdio.h>
#include <assert.h>

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbPlane.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCamera.h>

#include <Inventor/navigation/SoNavigationControl.h>

#include <Inventor/navigation/SoOrthoCenterMode.h>

// *************************************************************************

/*!
  \class SoCenterMode Inventor/navigation/SoCenterMode.h
  \brief Class that implements a focalpoint centering mode.

  This class implements the focalpoint centering mode, moving the camera
  without changing the view axis.

  \ingroup navigation
*/

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_NAVIGATION_MODE_SOURCE(SoOrthoCenterMode);

void
SoOrthoCenterMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoOrthoCenterMode, SoNavigationMode);
}

/*!
  Constructor.
*/

SoOrthoCenterMode::SoOrthoCenterMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  PRIVATE(this) = NULL;
}

/*!
  Destructor.
*/

SoOrthoCenterMode::~SoOrthoCenterMode(void)
{
}

/*!
*/

SoNavigationMode *
SoOrthoCenterMode::clone(void) const
{
  return new SoOrthoCenterMode(this->getModeName());
}

/*!
  This method implements the camera move operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
SoOrthoCenterMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (! event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if (mbevent->getState() != SoButtonEvent::DOWN) {
    return FALSE;
  }

  SoCamera * camera = ctrl->getCamera();
  if (!camera) {
    return FALSE;
  }

  SbVec3d pickpos;
  SoPath * path = ctrl->pick(event->getPosition(), pickpos);

  if (path != NULL) {
    SbRotation rot = camera->orientation.getValue();
    SbVec3f up(0, 1, 0);
    SbVec3f right(1, 0, 0);
    SbVec3f pos = camera->position.getValue();
    rot.multVec(up, up);
    rot.multVec(right, right);

    SbPlane plane(pos, pos+up, pos+right);

    SbVec3f pickposf((float)pickpos[0], (float)pickpos[1], (float)pickpos[2]);
    float distance = plane.getDistance(pickposf);
    SbVec3f normal = plane.getNormal();
    normal.normalize();

    SbVec3f newpos = pickposf - (normal * distance);
    camera->position.setValue(newpos);
    // fprintf(stderr, "orthomode move from <%f %f %f> to <%f %f %f>\n",
    //         pos[0], pos[1], pos[2], newpos[0], newpos[1], newpos[2]);
    this->finish();
  } else {
    this->abort();
  }
  return TRUE;
}

#undef PRIVATE

// *************************************************************************
