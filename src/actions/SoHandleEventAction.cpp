/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoHandleEventAction SoHandleEventAction.h Inventor/actions/SoHandleEventAction.h
  \brief The SoHandleEventAction class distributes user events to the scene.
  \ingroup actions

  This is the action used by the GUI viewer classes to pass
  interaction events from the window system to the nodes in the scene
  graph.

  SoHandleEventAction also provides the functionality for tracking the
  object currently under the cursor, and functionality for "grabbing"
  the event focus.

  \sa SoEvent
*/

#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoSubActionP.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/misc/SoState.h>


// *************************************************************************

// The private data for the SoHandleEventAction.

class SoHandleEventActionP {
public:
  SoHandleEventActionP(SoHandleEventAction * o) {
    this->owner = o;
  }

  // Hidden private methods.

  void doPick(SoRayPickAction * ra);
  SoRayPickAction * getPickAction(void);

  // Hidden private variables.

  SbViewportRegion viewport;
  const SoEvent * event;
  SoNode * grabber;
  SoNode * pickroot;
  SbBool pickvalid;
  SbBool didpickall;
  SoRayPickAction * pickaction;

private:
  SoHandleEventAction * owner;
};

#define THIS (this->pimpl)

// *************************************************************************

SO_ACTION_SOURCE(SoHandleEventAction);

// Overridden from parent class.
void
SoHandleEventAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoHandleEventAction, SoAction);

  SO_ENABLE(SoHandleEventAction, SoSwitchElement);
  SO_ENABLE(SoHandleEventAction, SoViewVolumeElement);
  SO_ENABLE(SoHandleEventAction, SoViewportRegionElement);
  SO_ENABLE(SoHandleEventAction, SoWindowElement);

}

/*!
  Constructor.

  SoHandleEventAction needs a \a viewportregion to pass on to the
  raypick action instance it uses for being able to track objects
  under the mouse cursor.
*/
SoHandleEventAction::SoHandleEventAction(const SbViewportRegion & viewportregion)
{
  THIS = new SoHandleEventActionP(this);
  THIS->viewport = viewportregion;
  THIS->event = NULL;
  THIS->grabber = NULL;
  THIS->pickroot = NULL;
  THIS->pickvalid = FALSE;
  THIS->didpickall = FALSE;
  THIS->pickaction = NULL;

  SO_ACTION_CONSTRUCTOR(SoHandleEventAction);
}

/*!
  Destructor.
*/
SoHandleEventAction::~SoHandleEventAction()
{
  if (THIS->pickroot) THIS->pickroot->unref();
  delete THIS->pickaction;

  delete THIS;
}

/*!
  Set a new viewport region, replacing the one passed in the
  constructor.
*/
void
SoHandleEventAction::setViewportRegion(const SbViewportRegion & newregion)
{
  THIS->viewport = newregion;
  if (THIS->pickaction) THIS->pickaction->setViewportRegion(newregion);
}

/*!
  Returns the viewport region this action instance is using.
*/
const SbViewportRegion &
SoHandleEventAction::getViewportRegion(void) const
{
  return THIS->viewport;
}

/*!
  Set the event to distribute to the nodes of the scene.
*/
void
SoHandleEventAction::setEvent(const SoEvent * ev)
{
  THIS->event = ev;
}

/*!
  Returns the event this action is handling.
*/
const SoEvent *
SoHandleEventAction::getEvent(void) const
{
  return THIS->event;
}

/*!
  Marks the action instance as handled, hence terminates the action.

  The action is only marked as handled when a node in the graph
  "grabs" the event this action is carrying, so the handled flag will
  be \c FALSE after traversal if no nodes wanted the event.

  \sa isHandled()
*/
void
SoHandleEventAction::setHandled(void)
{
  this->setTerminated(TRUE);
}

/*!
  Returns whether or not the event has been handled by a node during
  scene graph traversal.

  \sa setHandled()
*/
SbBool
SoHandleEventAction::isHandled(void) const
{
  return this->hasTerminated();
}

/*!
  Set a \a node pointer which will get all future events handled by
  this action until releaseGrabber() is called.
*/
void
SoHandleEventAction::setGrabber(SoNode * node)
{
  // Check for inequality before executing code is not only good for
  // performance, but is also necessary to remove the potential for
  // infinite recursion. See comment in releaseGrabber().

  if (node != THIS->grabber) {
    this->releaseGrabber();
    THIS->grabber = node;
    if (node) node->grabEventsSetup();
  }
}

/*!
  Don't send the events to a "grabber" node anymore, use the default
  behavior of the action and pass them along to the scene graph again.

  \sa setGrabber()
*/
void
SoHandleEventAction::releaseGrabber(void)
{
  // Store old grabber node and set current node to NULL before
  // calling SoNode::grabEventsCleanup(), to avoid being vulnerable to
  // recursive calls from grabEventsCleanup() back to this method
  // (which happens from dragger classes).

  SoNode * old = THIS->grabber;
  THIS->grabber = NULL;
  if (old) old->grabEventsCleanup();
}

/*!
  Returns the grabber node, or \c NULL if no grabber is active.
*/
SoNode *
SoHandleEventAction::getGrabber(void) const
{
  return THIS->grabber;
}

/*!
  Sets the root \a node that is used for the pick action tracking the
  cursor.
*/
void
SoHandleEventAction::setPickRoot(SoNode * node)
{
  if (THIS->pickroot != NULL) THIS->pickroot->unref();
  THIS->pickroot = node;
  THIS->pickroot->ref();
  THIS->pickvalid = FALSE;
}

/*!
  Returns the root node that is used by nodes that is tracking the
  cursor.
*/
SoNode *
SoHandleEventAction::getPickRoot(void) const
{
  return THIS->pickroot;
}

/*!
  Sets the pick radius for cursor tracking.
*/
void
SoHandleEventAction::setPickRadius(const float radiusinpixels)
{
  THIS->getPickAction()->setRadius(radiusinpixels);
}

/*!
  Returns the SoPickedPoint information for the intersection point
  below the cursor.
*/
const SoPickedPoint *
SoHandleEventAction::getPickedPoint(void)
{
  SoRayPickAction * ra = THIS->getPickAction();
  if (!THIS->pickvalid || THIS->didpickall) {
    ra->setPickAll(FALSE);
    THIS->doPick(ra);
  }
  return ra->getPickedPoint();
}

/*!
  Returns a list of all intersection points below the mouse cursor.
*/
const SoPickedPointList &
SoHandleEventAction::getPickedPointList(void)
{
  SoRayPickAction * ra = THIS->getPickAction();
  if (!THIS->pickvalid || !THIS->didpickall) {
    ra->setPickAll(TRUE);
    THIS->doPick(ra);
  }
  return ra->getPickedPointList();
}

// Documented in superclass. Overridden to initialize local data
// members before executing the scene traversal.
void
SoHandleEventAction::beginTraversal(SoNode * node)
{
  assert(THIS->event);
  this->setPickRoot(node);

  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), THIS->viewport);
  if (THIS->grabber) {
    this->traverse(THIS->grabber);
  }
  if (!this->isHandled()) {
    this->traverse(node);
  }
  this->getState()->pop();
}

//////// Hidden private methods for //////////////////////////////////////
//////// SoHandleEventActionP (pimpl) ////////////////////////////////////

// Singleton pattern for the pick action instance.
SoRayPickAction *
SoHandleEventActionP::getPickAction(void)
{
  if (this->pickaction == NULL) {
    this->pickaction = new SoRayPickAction(this->viewport);
  }
  return this->pickaction;
}

void
SoHandleEventActionP::doPick(SoRayPickAction * ra)
{
  if (!this->event || !this->pickroot) return;

  SbBool didapply = FALSE;
  ra->setPoint(this->event->getPosition());
  if (this->owner->getWhatAppliedTo() == SoAction::PATH) {
    const SoPath * path = this->owner->getPathAppliedTo();
    if (path->getHead() == this->pickroot) {
      ra->apply((SoPath*)path);
      didapply = TRUE;
    }
    else { // make subpath if pickroot can be found in path
      int i, n = path->getLength();
      for (i = 1; i < n; i++) {
        if (path->getNode(i) == this->pickroot) break;
      }
      if (i < n) {
        SoPath * tmppath = path->copy(i);
        tmppath->ref();
        ra->apply(tmppath);
        tmppath->unref();
        didapply = TRUE;
      }
    }
  }
  if (!didapply) ra->apply(this->pickroot);
  this->didpickall = ra->isPickAll();
  this->pickvalid = TRUE;
}
