/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \brief The SoEventCallback class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoEventCallback.h>

#include <Inventor/events/SoEvent.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
#include <Inventor/actions/SoHandleEventAction.h>
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

// *************************************************************************

SO_NODE_SOURCE(SoEventCallback);

/*!
  Constructor.
*/
SoEventCallback::SoEventCallback()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoEventCallback);

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  this->heaction = NULL;
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  Destructor.
*/
SoEventCallback::~SoEventCallback()
{
}

/*!
  Does initialization common for all objects of the
  SoEventCallback class. This includes setting up the
  type system, among other things.
*/
void
SoEventCallback::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoEventCallback);
}

/*!
  FIXME: write function documentation
*/
void
SoEventCallback::setPath(SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
const SoPath *
SoEventCallback::getPath(void)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}


/*!
  FIXME: write function documentation
*/
void
SoEventCallback::addEventCallback(SoType eventType, SoEventCallbackCB * f,
                                  void * userData)
{
  this->callbackTypes.append(eventType);
  this->callbacks.append((void *)f);
  this->callbacks.append(userData);
}

/*!
  FIXME: write function documentation
*/
void
SoEventCallback::removeEventCallback(SoType /* eventType */,
                                     SoEventCallbackCB * f,
                                     void * /* userData */)
{
  int idx = this->callbacks.find((void *)f);
  if (idx != -1) {
    this->callbackTypes.remove(idx/2);
    this->callbacks.remove(idx);
    this->callbacks.remove(idx);
  }

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoEventCallback::removeEventCallback",
                              "tried to remove non-existant callback");
  }
#endif // COIN_DEBUG
}


#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write function documentation
*/
SoHandleEventAction *
SoEventCallback::getAction(void) const
{
  return this->heaction;
}

/*!
  FIXME: write function documentation
*/
const SoEvent *
SoEventCallback::getEvent(void) const
{
  return (this->heaction ? this->heaction->getEvent() : NULL);
}

/*!
  FIXME: write function documentation
*/
const SoPickedPoint *
SoEventCallback::getPickedPoint(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}


/*!
  FIXME: write function documentation
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
  FIXME: write function documentation
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
  FIXME: write function documentation
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
  FIXME: write function documentation
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
  FIXME: write function documentation
*/
void
SoEventCallback::handleEvent(SoHandleEventAction * action)
{
  // Make it possible to access the action object from the callback
  // code.
  this->heaction = action;

  // Invoke callbacks.
  for(int i=0; i < this->callbackTypes.getLength(); i++) {
    SoType type = this->callbackTypes[i];
    if (this->heaction->getEvent()->isOfType(type)) {
      SoEventCallbackCB * cb = (SoEventCallbackCB *)(this->callbacks[i*2+0]);
      void * userdata = this->callbacks[i*2+1];
      cb(userdata, this);
    }
  }

  // Reset.
  this->heaction = NULL;
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
