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
  \class SoEventCallback SoEventCallback.h Inventor/nodes/SoEventCallback.h
  \brief The SoEventCallback class provides a method for handling events.
  \ingroup nodes

  Use SoEventCallback nodes for grabbing events during
  SoHandleEventAction traversal.
*/

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_NODE_SOURCE(SoEventCallback);


/*!
  Constructor.
*/
SoEventCallback::SoEventCallback(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoEventCallback);

  this->heaction = NULL;
  this->path = NULL;
}

/*!
  Destructor.
*/
SoEventCallback::~SoEventCallback()
{
  if (this->path) this->path->unref();
}

// Doc from superclass.
void
SoEventCallback::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoEventCallback);
}

/*!
  Sets the path that must be picked before the registered callbacks
  are invoked. If \c NULL, callbacks will be invoked for every event
  that matches the callback event type.

  \sa getPath()
*/
void
SoEventCallback::setPath(SoPath * path)
{
  if (this->path) {
    this->path->unref();
    this->path = NULL;
  }
  if (path) {
    this->path = path->copy();
    this->path->ref();
  }
}

/*!
  Returns the path that must be picked before callbacks are invoked.

  \sa setPath()
*/
const SoPath *
SoEventCallback::getPath(void)
{
  return this->path;
}


/*!
  Set up a callback function \a f which will be invoked for the given
  \a eventtype. \a userdata will be given as the first argument to the
  function.
*/
void
SoEventCallback::addEventCallback(SoType eventtype, SoEventCallbackCB * f,
                                  void * userdata)
{
  struct CallbackInfo cb;
  cb.func = f;
  cb.eventtype = eventtype;
  cb.userdata = userdata;

  this->callbacks.append(cb);
}

/*!
  Unregister the given callback function \a f.
*/
void
SoEventCallback::removeEventCallback(SoType eventtype, SoEventCallbackCB * f,
                                     void * userdata)
{
  for (int i = 0; i < this->callbacks.getLength(); i++) {
    if (this->callbacks[i].func == f &&
        this->callbacks[i].eventtype == eventtype &&
        this->callbacks[i].userdata == userdata) {
      this->callbacks.remove(i);
      return;
    }
  }

#if COIN_DEBUG
  SoDebugError::postWarning("SoEventCallback::removeEventCallback",
                            "tried to remove non-existent callback function");
#endif // COIN_DEBUG
}


/*!
  Returns the SoHandleEventAction instance currently traversing the
  scene graph.
*/
SoHandleEventAction *
SoEventCallback::getAction(void) const
{
  return this->heaction;
}

/*!
  Returns a pointer to the event object which is currently being
  processed.
*/
const SoEvent *
SoEventCallback::getEvent(void) const
{
  return (this->heaction ? this->heaction->getEvent() : NULL);
}

/*!
  Returns the picked point for the current handle event traversal.
*/
const SoPickedPoint *
SoEventCallback::getPickedPoint(void) const
{
  return this->heaction ? this->heaction->getPickedPoint() : NULL;
}


/*!
  Use this method from a callback function to notify the node that the
  event has been handled.

  The rest of the callbacks registered with the node will still be
  called, but further SoEventCallback nodes in the scene will not be
  used.

  Since callbacks registered within the same SoEventCallback node will
  still be invoked after the event has been handled, it is likely that
  you should use isHandled() to check for this condition from your
  callback functions.
*/
void
SoEventCallback::setHandled(void)
{
#if COIN_DEBUG
  if (!this->heaction) {
    SoDebugError::postWarning("SoEventCallback::setHandled",
                              "should only be called from event callbacks");
    return;
  }
#endif // COIN_DEBUG

  this->heaction->setHandled();
}

/*!
  Check whether or not the event from the SoHandleEventAction has been
  handled.
*/
SbBool
SoEventCallback::isHandled(void) const
{
#if COIN_DEBUG
  if (!this->heaction) {
    SoDebugError::postWarning("SoEventCallback::isHandled",
                              "should only be called from event callbacks");
    return TRUE;
  }
#endif // COIN_DEBUG

  return this->heaction->isHandled();
}


/*!
  Set up the node so all future events (until releaseEvents() is
  called) in Coin will be directly forwarded to this node.
*/
void
SoEventCallback::grabEvents(void)
{
#if COIN_DEBUG
  if (!this->heaction) {
    SoDebugError::postWarning("SoEventCallback::grabEvents",
                              "should only be called from event callbacks");
    return;
  }
#endif // COIN_DEBUG

  this->heaction->setGrabber(this);
}

/*!
  Do not grab event handling any more.

  \sa grabEvents()
*/
void
SoEventCallback::releaseEvents(void)
{
#if COIN_DEBUG
  if (!this->heaction) {
    SoDebugError::postWarning("SoEventCallback::releaseEvents",
                              "should only be called from event callbacks");
    return;
  }
#endif // COIN_DEBUG

  this->heaction->releaseGrabber();
}

/*!
  Invokes the registered callback functions.
 */
void
SoEventCallback::handleEvent(SoHandleEventAction * action)
{
  // check if correct path is picked
  if (this->path) {
    const SoPickedPoint * pp = action->getPickedPoint();
    if (pp && pp->getPath()->containsPath(this->path)) return;
  }

  // Make it possible to access the action object from the callback
  // code.
  this->heaction = action;

  SoType eventtype = this->heaction->getEvent()->getTypeId();
  
  // Invoke callbacks.
  for (int i = 0; i < this->callbacks.getLength(); i++) {
    if (eventtype.isDerivedFrom(this->callbacks[i].eventtype)) {
      SoEventCallbackCB * cb = this->callbacks[i].func;
      cb(this->callbacks[i].userdata, this);
    }
  }

  // Reset.
  this->heaction = NULL;
}
