/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoEvent SoEvent.h Inventor/events/SoEvent.h
  \brief The SoEvent class is the base class for all Coin events.
  \ingroup events

  Coin contains its own set of event classes, independent of the underlying
  window system.

  Upon system specific events, a translation is done by the window
  specific device classes into one of the Coin event object classes
  listed below. The event is then typically sent by the render area
  to an SoSceneManager which will apply it to the scenegraph through
  an SoHandleEventAction.

  Events may be caught by the user by attaching an SoEventCallback
  node to the scenegraph, or it can automatically be handled by a
  dragger or manipulator in the graph.

  \sa SoButtonEvent, SoKeyboardEvent, SoLocation2Event, SoMotion3Event
  \sa SoMouseButtonEvent, SoSpaceballButtonEvent
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoSpaceballButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbName.h>
#include <assert.h>

SO_EVENT_SOURCE(SoEvent);

/*!
  \fn SoType SoEvent::getClassTypeId(void)

  This static method returns the SoType object associated with objects
  of this class.
 */


// FIXME: grab better version of getTypeId() doc from SoBase, SoAction
// and / or SoDetail. 20010913 mortene.
/*!
  \fn SoType SoEvent::getTypeId(void) const

  Returns the actual type id of an instantiated object.
*/


/*!
  Initialize SoEvent and all it's known subclasses (i.e. all subclasses
  which are part of the standard classes in the Coin library).

  This method is called from SoDB::init(), so it's very unlikely that
  you will have to call it explicitly.
 */
void
SoEvent::initClass(void)
{
  // Make sure we only initialize once.
  assert(SoEvent::classTypeId == SoType::badType());

  SoEvent::classTypeId = SoType::createType(SoType::badType(), "SoEvent");

  SoEvent::initEvents();
}

/*!
  \COININTERNAL

  Initialize all known subclasses.
 */
void
SoEvent::initEvents(void)
{
  SoButtonEvent::initClass();
    SoMouseButtonEvent::initClass();
    SoKeyboardEvent::initClass();
    SoSpaceballButtonEvent::initClass();
  SoLocation2Event::initClass();
  SoMotion3Event::initClass();
}

/*!
  Constructor, will set all modifiers to "off" state.
*/
SoEvent::SoEvent(void)
{
  this->modifiers.shiftdown = 0;
  this->modifiers.ctrldown = 0;
  this->modifiers.altdown = 0;
}

/*!
  Destructor.
 */
SoEvent::~SoEvent()
{
}

/*!
  Returns TRUE if this object either has the same type as the given
  \c type parameter, or if \c type belongs to a superclass of ourselves.
*/
SbBool
SoEvent::isOfType(SoType type) const
{
  const SoType myType = this->getTypeId();
  if (myType == type) return TRUE;
  if (myType.isDerivedFrom(type)) return TRUE;
  return FALSE;
}

/*!
  From a system specific device object, set the time the event occurred.

  \sa getTime()
 */
void
SoEvent::setTime(const SbTime t)
{
  this->timeofevent = t;
}

/*!
  Returns the time the event occurred.

  \sa getPosition(), wasShiftDown(), wasCtrlDown(), wasAltDown()
 */
SbTime
SoEvent::getTime(void) const
{
  return this->timeofevent;
}

/*!
  From a system specific device object, set the mouse pointer position
  when the event occurred.

  \sa getPosition(), getNormalizedPosition()
 */
void
SoEvent::setPosition(const SbVec2s & p)
{
  this->positionofevent = p;
}

/*!
  Returns the mouse pointer position when the event occurred. The
  coordinates are given relative to the window coordinates.

  \sa getNormalizedPosition(), getTime(), wasShiftDown(), wasCtrlDown(),
  \sa wasAltDown()
 */
const SbVec2s &
SoEvent::getPosition(void) const
{
  return this->positionofevent;
}

/*!
  Returns the mouse pointer position when the event occurred. The
  coordinates are given relative to the viewport coordinates.

  \sa getNormalizedPosition(), getTime(), wasShiftDown(), wasCtrlDown(),
  \sa wasAltDown()
 */
const SbVec2s
SoEvent::getPosition(const SbViewportRegion & vpRgn) const
{
  return SbVec2s(this->positionofevent - vpRgn.getViewportOriginPixels());
}

/*!
  Returns the mouse pointer position when the event occurred. The
  coordinates are given relative to the viewport coordinates,
  normalized according to the size of the viewport.

  \sa getPosition(), getTime(), wasShiftDown(), wasCtrlDown(), wasAltDown()
 */
const SbVec2f
SoEvent::getNormalizedPosition(const SbViewportRegion & vpRgn) const
{
  SbVec2s p = this->positionofevent - vpRgn.getViewportOriginPixels();
  SbVec2s s = vpRgn.getViewportSizePixels();

  return SbVec2f((float)p[0]/(float)s[0], (float)p[1]/(float)s[1]);
}

/*!
  From a system specific device object, set the state of the Shift key(s)
  when the event occurred.

  \sa wasShiftDown(), setCtrlDown(), setAltDown()
 */
void
SoEvent::setShiftDown(SbBool isDown)
{
  this->modifiers.shiftdown = isDown ? TRUE : FALSE;
}

/*!
  Returns state of Shift key(s) when the event occurred.

  \sa wasCtrlDown(), wasAltDown(), getPosition(), getTime()
 */
SbBool
SoEvent::wasShiftDown(void) const
{
  return this->modifiers.shiftdown;
}

/*!
  From a system specific device object, set the state of the Ctrl key(s)
  when the event occurred.

  \sa wasCtrlDown(), setShiftDown(), setAltDown()
 */
void
SoEvent::setCtrlDown(SbBool isDown)
{
  this->modifiers.ctrldown = isDown ? TRUE : FALSE;
}

/*!
  Returns state of Ctrl key(s) when the event occurred.

  \sa wasShiftDown(), wasAltDown(), getPosition(), getTime()
 */
SbBool
SoEvent::wasCtrlDown(void) const
{
  return this->modifiers.ctrldown;
}

/*!
  From a system specific device object, set the state of the Alt key(s)
  when the event occurred.

  \sa wasAltDown(), setCtrlDown(), setShiftDown()
 */
void
SoEvent::setAltDown(SbBool isDown)
{
  this->modifiers.altdown = isDown ? TRUE : FALSE;
}

/*!
  Returns state of Alt key(s) when the event occurred.

  \sa wasShiftDown(), wasCtrlDown(), getPosition(), getTime()
 */
SbBool
SoEvent::wasAltDown(void) const
{
  return this->modifiers.altdown;
}
