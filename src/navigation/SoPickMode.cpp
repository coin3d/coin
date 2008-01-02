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

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/SbVec3d.h>

#include <Inventor/navigation/SoNavigationControl.h>
#include <Inventor/navigation/SoPickMode.h>

// *************************************************************************

class SoPickModeP {
public:
  SoPickModeP(SoPickMode * api);
  ~SoPickModeP(void);

  SbPList callbacks;
}; // SoPickModeP

#define PRIVATE(obj) ((obj)->pimpl)

SO_NAVIGATION_MODE_SOURCE(SoPickMode);

void
SoPickMode::initClass(void)
{
  SO_NAVIGATION_MODE_INIT_CLASS(SoPickMode, SoNavigationMode);
}

/*!
  Constructor.
*/

SoPickMode::SoPickMode(SbName name)
: inherited(name)
{
  PRIVATE(this) = new SoPickModeP(this);
}

/*!
  Destructor.
*/

SoPickMode::~SoPickMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

SoNavigationMode *
SoPickMode::clone(void) const
{
  return new SoPickMode(this->getModeName());
}

/*!
  This method handles the pitching motion of the camera, based on user input.
*/

SbBool
SoPickMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (!event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if (mbevent->getButton() == SoMouseButtonEvent::BUTTON1) {
    if (mbevent->getState() == SoButtonEvent::DOWN) {
      SbVec3d pos;
      SoPath * path = ctrl->pick(event->getPosition(), pos);
      if ( path ) {
        this->invokePickCallbacks(pos, path);
      } else {
        this->invokePickCallbacks(SbVec3d(0, 0, 0), NULL);
      }
    } else {
      // button up - do nothing, but don't pass event on
    }
    return TRUE;
  }
  return FALSE;
}

void
SoPickMode::addPickCallback(SoPickModePickCB * cb, void * closure)
{
  PRIVATE(this)->callbacks.append((void *) cb);
  PRIVATE(this)->callbacks.append(closure);
}

void
SoPickMode::removePickCallback(SoPickModePickCB * cb, void * closure)
{
  const int max = PRIVATE(this)->callbacks.getLength();
  int i;
  for ( i = 0; i < max; i += 2 ) {
    SoPickModePickCB * callback = (SoPickModePickCB *)
      PRIVATE(this)->callbacks[i];
    void * data = PRIVATE(this)->callbacks[i+1];
    if (callback == cb && data == closure) {
      PRIVATE(this)->callbacks.remove(i+1);
      PRIVATE(this)->callbacks.remove(i);
      return;
    }
  }
}

void
SoPickMode::invokePickCallbacks(const SbVec3d & pos, const SoPath * path)
{
  const int max = PRIVATE(this)->callbacks.getLength();
  int i;
  for ( i = 0; i < max; i += 2 ) {
    SoPickModePickCB * callback = (SoPickModePickCB *)
      PRIVATE(this)->callbacks[i];
    void * closure = PRIVATE(this)->callbacks[i+1];
    callback(closure, pos, path);
  }
}

#undef PRIVATE

// *************************************************************************

SoPickModeP::SoPickModeP(SoPickMode * api)
{
}

SoPickModeP::~SoPickModeP(void)
{
}

// *************************************************************************
