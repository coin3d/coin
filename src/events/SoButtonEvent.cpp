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
  \class SoButtonEvent SoButtonEvent.h Inventor/events/SoButtonEvent.h
  \brief The SoButtonEvent class is the base class for all button events.
  \ingroup events

  The event classes which results from the user pushing buttons on
  some device (keyboard, mouse or spaceball) all inherit this
  class. The SoButtonEvent class contains methods for setting and
  getting the state of the button(s).

  \sa SoEvent, SoKeyboardEvent, SoMouseButtonEvent, SoSpaceballButtonEvent
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoButtonEvent.h>
#include <Inventor/SbName.h>
#include <assert.h>

/*!
  \enum SoButtonEvent::State
  This gives the actual state of the button.
 */
/*!
  \var SoButtonEvent::State SoButtonEvent::UP
  Button has been released.
*/
/*!
  \var SoButtonEvent::State SoButtonEvent::DOWN
  Button has been pressed down.
*/
/*!
  \var SoButtonEvent::State SoButtonEvent::UNKNOWN
  The state of the button is unknown.
*/


//$ BEGIN TEMPLATE EventSource(SoButtonEvent)
/*!
  \var SoButtonEvent::classTypeId
  Unique type identification for the SoButtonEvent class type.
*/
SoType SoButtonEvent::classTypeId = SoType::badType();

/*!
  This is a virtual function overloaded in all subclasses which will
  return the type identificator of the class instance.

  \sa getClassTypeId(), isOfType()
*/
SoType
SoButtonEvent::getTypeId(void) const
{
  return SoButtonEvent::classTypeId;
}

/*!
  Returns the type identificator for the SoButtonEvent class.

  \sa getTypeId(), isOfType()
*/
SoType
SoButtonEvent::getClassTypeId(void)
{
  return SoButtonEvent::classTypeId;
}
//$ END TEMPLATE EventSource

/*!
  Initialize the type information data.
 */
void
SoButtonEvent::initClass(void)
{
//$ BEGIN TEMPLATE EventInitClass(ButtonEvent)
  // Make sure we only initialize once.
  assert(SoButtonEvent::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoButtonEvent::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "ButtonEvent");
//$ END TEMPLATE EventInitClass
}

/*!
  Constructor.
*/
SoButtonEvent::SoButtonEvent(void)
{
  this->buttonstate = SoButtonEvent::UNKNOWN;
}

/*!
  Destructor.
*/
SoButtonEvent::~SoButtonEvent()
{
}

/*!
  Set the state of the button which the user interacted with.

  This method is used from the window specific device classes when
  translating events to the generic Coin library.

  \sa getState()
 */
void
SoButtonEvent::setState(SoButtonEvent::State state)
{
  this->buttonstate = state;
}

/*!
  Returns the state of the button which is the cause of the event, i.e.
  whether it was pushed down or released.

  \sa wasShiftDown(), wasCtrlDown(), wasAltDown(), getPosition(), getTime()
 */
SoButtonEvent::State
SoButtonEvent::getState(void) const
{
  return this->buttonstate;
}
