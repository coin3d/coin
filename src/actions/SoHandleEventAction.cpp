/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
  : viewport(viewportregion), event(NULL), grabber(NULL), pickroot(NULL),
    pickvalid(FALSE), pickaction(NULL), applynode(NULL)
{
  SO_ACTION_CONSTRUCTOR(SoHandleEventAction);

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::handleEventS);
}

/*!
  Destructor.
*/
SoHandleEventAction::~SoHandleEventAction()
{
  delete this->pickaction;
}

/*!
  Set a new viewport region, replacing the one passed in the
  constructor.
*/
void
SoHandleEventAction::setViewportRegion(const SbViewportRegion & newregion)
{
  this->viewport = newregion;
  delete this->pickaction;
  this->pickaction = NULL;
}

/*!
  Returns the viewport region this action instance is using.
*/
const SbViewportRegion &
SoHandleEventAction::getViewportRegion(void) const
{
  return this->viewport;
}

/*!
  Set the event to distribute to the nodes of the scene.
*/
void
SoHandleEventAction::setEvent(const SoEvent * ev)
{
  this->event = ev;
}

/*!
  Returns the event this action is handling.
*/
const SoEvent *
SoHandleEventAction::getEvent(void) const
{
  return this->event;
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

  if (node != this->grabber) {
    this->releaseGrabber();
    this->grabber = node;
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

  SoNode * old = this->grabber;
  this->grabber = NULL;
  if (old) old->grabEventsCleanup();
}

/*!
  Returns the grabber node, or \c NULL if no grabber is active.
*/
SoNode *
SoHandleEventAction::getGrabber(void) const
{
  return this->grabber;
}

/*!
  Sets the root \a node that is used for the pick action tracking the
  cursor.
*/
void
SoHandleEventAction::setPickRoot(SoNode * node)
{
  this->pickroot = node;
}

/*!
  Returns the root node that is used by nodes that is tracking the
  cursor.
*/
SoNode *
SoHandleEventAction::getPickRoot(void) const
{
  return this->pickroot;
}

/*!
  Sets the pick radius for cursor tracking.
*/
void
SoHandleEventAction::setPickRadius(const float radiusinpixels)
{
  this->getPickAction()->setRadius(radiusinpixels);
}

/*!
  Returns the SoPickedPoint information for the intersection point
  below the cursor.
*/
const SoPickedPoint *
SoHandleEventAction::getPickedPoint(void)
{
  if (this->event && (this->pickroot || this->applynode) &&
      (!this->pickvalid || this->didpickall)) {
    this->getPickAction()->setPoint(this->event->getPosition());
    this->getPickAction()->setPickAll(FALSE);
    this->getPickAction()->apply(this->pickroot ? this->pickroot : this->applynode);
    this->pickvalid = TRUE;
    this->didpickall = FALSE;
  }
  return this->getPickAction()->getPickedPoint();
}

/*!
  Returns a list of all intersection points below the mouse cursor.
*/
const SoPickedPointList &
SoHandleEventAction::getPickedPointList(void)
{

//   Maybe it is A Good Thing to make an extension to SoRayPickAction,
//   to always make it store all picked points, but also store the
//   closest picked point?  pederb, 19991214

  if (this->event && (this->pickroot || this->applynode) &&
      (!this->pickvalid || this->didpickall)) {
    this->getPickAction()->setPickAll(TRUE);
    this->getPickAction()->apply(this->pickroot ? this->pickroot : this->applynode);
    this->pickvalid = TRUE;
    this->didpickall = TRUE;
  }
  return this->getPickAction()->getPickedPointList();
}

/*!
  Overloaded to initialize local data members before executing the
  scene traversal.
*/
void
SoHandleEventAction::beginTraversal(SoNode * node)
{
  assert(this->event);
  this->pickvalid = FALSE;
  this->applynode = node;

  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->viewport);
  if (this->grabber) {
    // ?? is this correct ?? pederb, 19991214
    this->traverse(this->grabber);
  }
  else {
    inherited::beginTraversal(node);
  }
  this->getState()->pop();
}

// Singleton pattern for the pick action instance.
SoRayPickAction *
SoHandleEventAction::getPickAction(void)
{
  if (this->pickaction == NULL) {
    this->pickaction = new SoRayPickAction(this->viewport);
  }
  return this->pickaction;
}
