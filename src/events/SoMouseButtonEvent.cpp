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
  \class SoMouseButtonEvent SoMouseButtonEvent.h Inventor/events/SoMouseButtonEvent.h
  \brief The SoMouseButtonEvent class contains information about
  mousebutton interaction.
  \ingroup events

  When the user presses any buttons on the mouse, these will be
  translated from a system specific event into a Coin event and sent
  to the scenegraph by using instances of this class.

  \sa SoEvent, SoButtonEvent, SoSpaceballButtonEvent, SoKeyboardEvent
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/SbName.h>
#include <assert.h>

/*!
  \enum SoMouseButtonEvent::Button
  This enum contains all mouse buttons detected by Coin.
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::ANY
  Unknown button.
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::BUTTON1
  First mouse button (usually the leftmost button).
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::BUTTON2
  Second mouse button (usually the middle button).
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::BUTTON3
  Third mouse button (usually the rightmost button).
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::BUTTON4
  Fourth mouse button (typically from a wheelmouse). This is
  \e not part of the original Open Inventor API.
*/
/*!
  \var SoMouseButtonEvent::Button SoMouseButtonEvent::BUTTON5
  Fifth mouse button (typically from a wheelmouse). This is
  \e not part of the original Open Inventor API.
*/

/*!
  \def SO_MOUSE_PRESS_EVENT(EVENT, BUTTON)

  This macro evaluates to \c TRUE iff the \a EVENT represents a press
  on the given \a BUTTON.
*/
/*!
  \def SO_MOUSE_RELEASE_EVENT(EVENT, BUTTON)

  This macro evaluates to \c TRUE iff the \a EVENT represents a
  release of the given \a BUTTON.
*/



SO_EVENT_SOURCE(SoMouseButtonEvent);

/*!
  Initialize the type information data.
 */
void
SoMouseButtonEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoMouseButtonEvent, SoButtonEvent);
}

/*!
  Constructor.
*/
SoMouseButtonEvent::SoMouseButtonEvent(void)
{
  this->button = SoMouseButtonEvent::ANY;
}

/*!
  Destructor.
*/
SoMouseButtonEvent::~SoMouseButtonEvent()
{
}

/*!
  Set the value of the button which the user interacted with.

  This method is used from the window specific device classes when
  translating events to the generic Coin library.

  \sa getButton()
 */
void
SoMouseButtonEvent::setButton(SoMouseButtonEvent::Button button)
{
  this->button = button;
}

/*!
  Returns the value of the button which was pressed or released.

  \sa getState()
  \sa wasShiftDown(), wasCtrlDown(), wasAltDown(), getPosition(), getTime()
 */
SoMouseButtonEvent::Button
SoMouseButtonEvent::getButton(void) const
{
  return this->button;
}

/*!
  Convenience method for quickly checking if the given event is a
  press on the given button, \a whichButton.

  \sa isButtonReleaseEvent(), isOfType(), getButton(), getState()
 */
SbBool
SoMouseButtonEvent::isButtonPressEvent(const SoEvent * e,
                                       SoMouseButtonEvent::Button whichButton)
{
  if (e->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    const SoMouseButtonEvent * me = (const SoMouseButtonEvent *)e;
    if ((me->getState() == SoButtonEvent::DOWN) &&
        ((whichButton == SoMouseButtonEvent::ANY) ||
         whichButton == me->getButton())) return TRUE;
  }
  return FALSE;
}

/*!
  Convenience method for quickly checking if the given event is a
  release of the given button, \a whichButton.

  \sa isButtonPressEvent(), isOfType(), getButton(), getState()
 */
SbBool
SoMouseButtonEvent::isButtonReleaseEvent(const SoEvent * e,
                                         SoMouseButtonEvent::Button
                                         whichButton)
{
  if (e->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    const SoMouseButtonEvent * me = (const SoMouseButtonEvent *)e;
    if ((me->getState() == SoButtonEvent::UP) &&
        ((whichButton == SoMouseButtonEvent::ANY) ||
         whichButton == me->getButton())) return TRUE;
  }
  return FALSE;
}
