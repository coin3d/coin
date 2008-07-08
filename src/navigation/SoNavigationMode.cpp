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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>

#include <Inventor/navigation/SoNavigationMode.h>
#include <Inventor/navigation/SoNavigationControl.h>

#include <Inventor/navigation/SoIdleMode.h>
#include <Inventor/navigation/SoRotateMode.h>
#include <Inventor/navigation/SoPanMode.h>
#include <Inventor/navigation/SoZoomMode.h>

#include "SoEvent.h"

// *************************************************************************

/*!
  \class SoNavigationMode Inventor/navigation/SoNavigationMode.h
  \brief Abstract base class for implementing a navigation submode.

  This class is internal and should mostly be of interest for developers
  planning on implementing their own navigation systems with new navigation
  submodes.

  It is abstract.  The handleEvent method needs to be reimplemented
  by subclasses.

  \ingroup navigation
*/

/*
  FIXME: the internal mode state data is still flawed since the same mode
  object can potentially go on the stack multiple times. Need to only put
  clones on the stack or something, because data must be initialized on
  push, not on original object creation, and each stack-level needs
  its own data.
*/


// *************************************************************************

struct Transition {
  const SoEvent * trigger;
  const SoEvent * condition;
  SoNavigationMode * mode;
  SbBool stack;
};

class SoNavigationModeP {
public:
  SoNavigationModeP(void);

  SbName modename;

  SbVec2s initpos;
  SbVec2s prevpos;
  SbVec2s currentpos;

  SoNavigation1DInputValueFunc * valuefunc1d;
  SoNavigation2DInputValueFunc * valuefunc2d;
  SoNavigation3DInputValueFunc * valuefunc3d;
  void * valuefuncclosure;

  SbBool aborted;
  SbBool finished;
}; // SoNavigationModeP

// *************************************************************************

SO_NAVIGATION_MODE_SOURCE(SoNavigationMode);

/*!
  Static class initializer.
*/

void
SoNavigationMode::initClass(void)
{
  assert(SoNavigationMode::classTypeId == SoType::badType() &&
         "don't init() twice!");
  /* Set up entry in the type system. */
  SoNavigationMode::classTypeId = SoType::createType(SoType::badType(),
                                                     "SoNavigationMode",
                                                     NULL /* createfunc */,
                                                     0 /* data */);
  cc_coin_atexit_static_internal((coin_atexit_f*) cleanClass);
}

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.  The \a modename argument is the name of the mode.
  It is often useful to create multiple instances of the same mode
  but with different names in a navigation system.
*/

SoNavigationMode::SoNavigationMode(SbName modename)
{
  PRIVATE(this) = new SoNavigationModeP;
  PRIVATE(this)->modename = modename;
}

/*!
  Destructor.
*/

SoNavigationMode::~SoNavigationMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Returns the name of the mode, given to the constructor.
*/

SbName
SoNavigationMode::getModeName(void) const
{
  return PRIVATE(this)->modename;
}

/*!
  \sa get1DValueFunc, set2DValueFunc, set3DValueFunc
*/

void
SoNavigationMode::set1DValueFunc(SoNavigation1DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = func;
  PRIVATE(this)->valuefunc2d = NULL;
  PRIVATE(this)->valuefunc3d = NULL;
  PRIVATE(this)->valuefuncclosure = closure;
}

void
SoNavigationMode::set2DValueFunc(SoNavigation2DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = NULL;
  PRIVATE(this)->valuefunc2d = func;
  PRIVATE(this)->valuefunc3d = NULL;
  PRIVATE(this)->valuefuncclosure = closure;
}

void
SoNavigationMode::set3DValueFunc(SoNavigation3DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = NULL;
  PRIVATE(this)->valuefunc2d = NULL;
  PRIVATE(this)->valuefunc3d = func;
  PRIVATE(this)->valuefuncclosure = closure;
}

/*!
  \sa set1DValueFunc, get2DValueFunc, get3DValueFunc
*/

SoNavigation1DInputValueFunc *
SoNavigationMode::get1DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc1d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc1d;
}

SoNavigation2DInputValueFunc *
SoNavigationMode::get2DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc2d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc2d;
}

SoNavigation3DInputValueFunc *
SoNavigationMode::get3DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc3d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc3d;
}

/*!
  This is the entry point for events coming from the
  SoNavigationSystem.  Some initial information is gathered before the
  event is passed on to the virtual, abstract handleEvent method.

  \sa handleEvent
*/

SbBool
SoNavigationMode::processEvent(const SoEvent * event, const SoNavigationControl * ctrl)
{
  if (event->isOfType(SoLocation2Event::getClassTypeId()))
    PRIVATE(this)->prevpos = PRIVATE(this)->currentpos;

  PRIVATE(this)->currentpos = event->getPosition();

  return this->handleEvent(event, ctrl);
}

/*!
  Not implemented yet.

  This function should return a visualization scene graph in case the
  navigation submode also includes visualization hints for the 3D
  display.
*/

SoNode *
SoNavigationMode::getSceneGraph(void)
{
  return NULL;
}

/*!
  \fn SbBool SoNavigationMode::handleEvent(const SoEvent * event, const SoNavigationControl * ctrl) = 0

  Virtual abstract method that should be overridden to implement the
  navigation submode.

  All events passed through the system while this mode is active is
  passed to this method, including the events that cause init(),
  abort(), and finish() event transitions.

*/

/*!
  Invoked when a mode is started/initialized.  It is passed the event that
  triggered the mode change, or NULL in the case of being the default mode.

  This method is called just before handleEvent is called with the
  exact same event object.

  If overriding this event, control should still be passed down to this
  method.

  \sa handleEvent, abort(), finish()
*/

void
SoNavigationMode::init(const SoEvent * event, const SoNavigationControl * ctrl)
{
  PRIVATE(this)->initpos = event->getPosition();
  PRIVATE(this)->prevpos = event->getPosition();
  PRIVATE(this)->currentpos = event->getPosition();
  PRIVATE(this)->aborted = FALSE;
  PRIVATE(this)->finished = FALSE;
}

/*!
  This method is invoked when themode is aborted.  The event passed to
  this method has already been handed to handleEvent as well.

  \sa handleEvent, init(), finish()
*/

void
SoNavigationMode::abort(const SoEvent * event, const SoNavigationControl * ctrl)
{
  ctrl->restoreCamera();
}

/*!
  Invoked when a mode is completed.  The event object passed to this method
  has already been handed to handleEvent as well.

  \sa handleEvent, init(), abort()
*/

void
SoNavigationMode::finish(const SoEvent * event, const SoNavigationControl * ctrl)
{
  ctrl->saveCamera();
}

// *************************************************************************

float
SoNavigationMode::get1DValue(const SoNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc1d);
  return PRIVATE(this)->valuefunc1d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

SbVec2f
SoNavigationMode::get2DValue(const SoNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc2d);
  return PRIVATE(this)->valuefunc2d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

SbVec3f
SoNavigationMode::get3DValue(const SoNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc3d);
  return PRIVATE(this)->valuefunc3d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

/*!
  Returns the initial pointer position in the viewport, from when the
  navigation submode was started.

  \sa getPreviousPosition, getCurrentPosition, getInitialNormalizedPosition
*/

SbVec2s
SoNavigationMode::getInitialPosition(void) const
{
  return PRIVATE(this)->initpos;
}

/*!
  Returns the previous points position in the viewport.

  \sa getInitialPosition, getCurrentPosition, getPreviousNormalizedPosition
*/

SbVec2s
SoNavigationMode::getPreviousPosition(void) const
{
  return PRIVATE(this)->prevpos;
}

/*!
  Returns the current pointer position in the viewport.

  \sa getInitialPosition, getPreviousPosition, getCurrentNormalizedPosition
*/

SbVec2s
SoNavigationMode::getCurrentPosition(void) const
{
  return PRIVATE(this)->currentpos;
}

/*!
  Returns the initial pointer position in normalized coordinates.

  \sa getInitialPosition, getPreviousNormalizedPosition, getCurrentNormalizedPosition
*/

SbVec2f
SoNavigationMode::getInitialNormalizedPosition(const SoNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getInitialPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the previuos pointer position in normalized coordinates.

  \sa getPreviousPosition, getInitialNormalizedPosition, getCurrentNormalizedPosition
*/

SbVec2f 
SoNavigationMode::getPreviousNormalizedPosition(const SoNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getPreviousPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the current pointer position in normalized coordinates.

  \sa getCurrentPosition, getInitialNormalizedPosition, getPreviousNormalizedPosition
*/


SbVec2f
SoNavigationMode::getCurrentNormalizedPosition(const SoNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getCurrentPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

// *************************************************************************

float
SoNavigationMode::getAbsMouseMovePixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2f vec((float)(currentpos[0] - initpos[0]), 
              (float)(currentpos[1] - initpos[1]));
  return vec.length();
}

float
SoNavigationMode::getMouseMoveVerticalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  return float(currentpos[1] - initpos[1]);
}

float
SoNavigationMode::getMouseMoveVerticalNormalizedDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2s vp = ctrl->getViewportSize();
  return float(currentpos[1] - initpos[1]) / float(vp[1]);
}

float
SoNavigationMode::getMouseMoveHorizontalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  return float(currentpos[0] - initpos[0]);
}

float
SoNavigationMode::getMouseMoveHorizontalNormalizedDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2s vp = ctrl->getViewportSize();
  return float(currentpos[0] - initpos[0]) / float(vp[0]);
}

float
SoNavigationMode::getAbsMouseCenterPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

float
SoNavigationMode::getMouseCenterVerticalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

float
SoNavigationMode::getMouseCenterHorizontalPixelDistance(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

float
SoNavigationMode::getMouseMoveAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

float
SoNavigationMode::getMouseCenterAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

float
SoNavigationMode::getMouseMoveCenterAngle(void * closure, const SoNavigationMode * mode, const SoNavigationControl * ctrl)
{
  return 0.0f;
}

void
SoNavigationMode::abort(void)
{
  if (this->isFinished()) return;
  PRIVATE(this)->aborted = TRUE;
}

void
SoNavigationMode::finish(void)
{
  if (this->isAborted()) return;
  PRIVATE(this)->finished = TRUE;
}

SbBool
SoNavigationMode::isAborted(void) const
{
  return PRIVATE(this)->aborted;
}

SbBool
SoNavigationMode::isFinished(void) const
{
  return PRIVATE(this)->finished;
}

#undef PRIVATE

// *************************************************************************
// private class implementation

SoNavigationModeP::SoNavigationModeP(void)
{
  this->initpos.setValue(0, 0);
  this->prevpos.setValue(0, 0);
  this->currentpos.setValue(0, 0);
  this->valuefunc1d = NULL;
  this->valuefunc2d = NULL;
  this->valuefunc3d = NULL;
  this->valuefuncclosure = NULL;
  this->aborted = FALSE;
  this->finished = FALSE;
}
