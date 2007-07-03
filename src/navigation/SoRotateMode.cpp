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

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#include <Inventor/navigation/SoNavigationControl.h>

#include <Inventor/navigation/SoRotateMode.h>

// *************************************************************************

/*!
  \class SoRotateMode Inventor/navigation/SoRotateMode.h
  \brief Class that implements the rotate mode.

  This class implements the object-rotate mode, rotating the camera
  around the focal point.

  \ingroup navigation
*/

class SoRotateModeP {
public:
  SoRotateModeP(SoRotateMode * api) {
    this->projector = NULL;
  }
  ~SoRotateModeP(void) {
    if (this->projector) {
      delete this->projector;
      this->projector = NULL;
    }
  }

public:
  SbSphereSheetProjector * projector;

}; // SoRotateModeP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_NAVIGATION_MODE_SOURCE(SoRotateMode);

void
SoRotateMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoRotateMode, SoNavigationMode);
}

/*!
  Constructor.
*/


SoRotateMode::SoRotateMode(SbName modename)
  : inherited(modename)
{
  PRIVATE(this) = new SoRotateModeP(this);
}

/*!
  Destructor.
*/

SoRotateMode::~SoRotateMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

SoNavigationMode *
SoRotateMode::clone(void) const
{
  return new SoRotateMode(this->getModeName());
}

/*!
  This method implements the object rotatio through tracking how the mouse
  location moves on a spheresheet-projector.

  Returns FALSE for unused events and TRUE for events that are used.
*/

SbBool
SoRotateMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (!event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  SbVec2f initpos(this->getInitialNormalizedPosition(ctrl));
  SbVec2f currentpos(this->getCurrentNormalizedPosition(ctrl));

  if (PRIVATE(this)->projector == NULL) {
    PRIVATE(this)->projector =
      new SbSphereSheetProjector(SbSphere(SbVec3f(0, 0, 0), 0.8f));
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    PRIVATE(this)->projector->setViewVolume(volume);
  }

  ctrl->restoreCamera();

  PRIVATE(this)->projector->project(initpos);
  SbRotation rot;
  PRIVATE(this)->projector->projectAndGetRotation(currentpos, rot);
  rot.invert();

  ctrl->reorientCamera(rot);
  return TRUE;
}

#undef PRIVATE
