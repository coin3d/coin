/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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
#include <cassert>

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


SO_EVENT_SOURCE(SoButtonEvent);

/*!
  Initialize the type information data.
 */
void
SoButtonEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoButtonEvent, SoEvent);
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

/*!
  Converts from an enum value of type SoButtonEvent::State to a
  string containing the enum symbol.

  \COIN_FUNCTION_EXTENSION
  \since Coin 3.0
*/
// Should we add stringToEnum as well perhaps?
SbBool
SoButtonEvent::enumToString(State enumval, SbString & stringrep)
{
  switch (enumval) {
  case SoButtonEvent::UP:
    stringrep = "UP";
    break;
  case SoButtonEvent::DOWN:
    stringrep = "DOWN";
    break;
  case SoButtonEvent::UNKNOWN:
    stringrep = "UNKNOWN";
    break;
  default:
    return FALSE;
  }
  return TRUE;
}
