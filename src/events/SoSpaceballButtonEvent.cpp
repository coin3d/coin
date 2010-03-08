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
  \class SoSpaceballButtonEvent SoSpaceballButtonEvent.h Inventor/events/SoSpaceballButtonEvent.h
  \brief The SoSpaceballButtonEvent class contains information about
  spaceball button interaction.
  \ingroup events

  When the user presses any buttons on a spaceball device, these will
  be translated from a system specific event into a Coin event and
  sent to the scenegraph by using instances of this class.

  \sa SoEvent, SoButtonEvent, SoMouseButtonEvent, SoKeyboardEvent
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoSpaceballButtonEvent.h>

#include "SbBasicP.h"

#include <Inventor/SbName.h>
#include <cassert>

/*!
  \enum SoSpaceballButtonEvent::Button
  This enum contains all spaceball buttons detected by Coin.
 */

/*!
  \def SO_SPACEBALL_PRESS_EVENT(EVENT, BUTTON)
  This macro evaluates to \c TRUE iff the \c EVENT represents a press on the
  given \c BUTTON.
*/
/*!
  \def SO_SPACEBALL_RELEASE_EVENT(EVENT, BUTTON)
  This macro evaluates to \c TRUE iff the \c EVENT represents a release of the
  given \c BUTTON.
*/

SO_EVENT_SOURCE(SoSpaceballButtonEvent);

/*!
  Initialize the type information data.
 */
void
SoSpaceballButtonEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoSpaceballButtonEvent, SoButtonEvent);
}

/*!
  Constructor.
 */
SoSpaceballButtonEvent::SoSpaceballButtonEvent(void)
{
  this->button = SoSpaceballButtonEvent::ANY;
}

/*!
  Destructor.
 */
SoSpaceballButtonEvent::~SoSpaceballButtonEvent()
{
}

/*!
  Set the value of the button which the user interacted with.

  This method is used from the window specific device classes when
  translating events to the generic Coin library.

  \sa getButton()
 */
void
SoSpaceballButtonEvent::setButton(SoSpaceballButtonEvent::Button buttonarg)
{
  this->button = buttonarg;
}

/*!
  Returns the value of the button which was pressed or released.

  \sa getState()
  \sa wasShiftDown(), wasCtrlDown(), wasAltDown(), getPosition(), getTime()
 */
SoSpaceballButtonEvent::Button
SoSpaceballButtonEvent::getButton(void) const
{
  return this->button;
}

/*!
  Convenience method for quickly checking if the given event is a
  press on the given button, \c whichButton.

  \sa isButtonReleaseEvent(), isOfType(), getButton(), getState()
 */
SbBool
SoSpaceballButtonEvent::isButtonPressEvent(const SoEvent * e,
                                           SoSpaceballButtonEvent::Button
                                           whichButton)
{
  return (e->isOfType(SoSpaceballButtonEvent::getClassTypeId()) &&
          (
          whichButton == SoSpaceballButtonEvent::ANY ||
           coin_assert_cast<const SoSpaceballButtonEvent *>(e)->getButton()
          == whichButton
          )
         &&
          coin_assert_cast<const SoButtonEvent *>(e)->getState() == SoButtonEvent::DOWN
         );
}

/*!
  Convenience method for quickly checking if the given event is a
  release of the given button, \c whichButton.

  \sa isButtonPressEvent(), isOfType(), getButton(), getState()
 */
SbBool
SoSpaceballButtonEvent::isButtonReleaseEvent(const SoEvent * e,
                                             SoSpaceballButtonEvent::Button
                                             whichButton)
{
  return (e->isOfType(SoSpaceballButtonEvent::getClassTypeId()) &&
          (
          whichButton == SoSpaceballButtonEvent::ANY ||
           coin_assert_cast<const SoSpaceballButtonEvent *>(e)->getButton()
          == whichButton
          ) &&
          coin_assert_cast<const SoButtonEvent *>(e)->getState() == SoButtonEvent::UP
         );
}


/*!
  Converts from an enum value of type SoMouseButtonEvent::Button to a
  string containing the enum symbol.

  \COIN_FUNCTION_EXTENSION
  \since Coin 3.0
*/
// Should we add stringToEnum as well perhaps?
SbBool
SoSpaceballButtonEvent::enumToString(Button enumval, SbString & stringrep)
{
  switch (enumval) {
  case SoSpaceballButtonEvent::ANY:
    stringrep = "ANY";
    break;
  case SoSpaceballButtonEvent::BUTTON1:
    stringrep = "BUTTON1";
    break;
  case SoSpaceballButtonEvent::BUTTON2:
    stringrep = "BUTTON2";
    break;
  case SoSpaceballButtonEvent::BUTTON3:
    stringrep = "BUTTON3";
    break;
  case SoSpaceballButtonEvent::BUTTON4:
    stringrep = "BUTTON4";
    break;
  case SoSpaceballButtonEvent::BUTTON5:
    stringrep = "BUTTON5";
    break;
  case SoSpaceballButtonEvent::BUTTON6:
    stringrep = "BUTTON6";
    break;
  case SoSpaceballButtonEvent::BUTTON7:
    stringrep = "BUTTON7";
    break;
  case SoSpaceballButtonEvent::BUTTON8:
    stringrep = "BUTTON8";
    break;
  case SoSpaceballButtonEvent::PICK:
    stringrep = "PICK";
    break;
  default:
    return FALSE;
  }
  return TRUE;
}
