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
#include <Inventor/SbName.h>
#include <assert.h>

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


//$ BEGIN TEMPLATE EventSource( SoSpaceballButtonEvent )
/*!
  \var SoSpaceballButtonEvent::classTypeId
  Unique type identification for the SoSpaceballButtonEvent class type.
*/
SoType SoSpaceballButtonEvent::classTypeId = SoType::badType();

/*!
  This is a virtual function overloaded in all subclasses which will
  return the type identificator of the class instance.

  \sa getClassTypeId(), isOfType()
*/
SoType
SoSpaceballButtonEvent::getTypeId(void) const
{
  return SoSpaceballButtonEvent::classTypeId;
}

/*!
  Returns the type identificator for the SoSpaceballButtonEvent class.

  \sa getTypeId(), isOfType()
*/
SoType
SoSpaceballButtonEvent::getClassTypeId(void)
{
  return SoSpaceballButtonEvent::classTypeId;
}
//$ END TEMPLATE EventSource

/*!
  Initialize the type information data.
 */
void
SoSpaceballButtonEvent::initClass(void)
{
//$ BEGIN TEMPLATE EventInitClass( SpaceballButtonEvent )
  // Make sure we only initialize once.
  assert(SoSpaceballButtonEvent::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSpaceballButtonEvent::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "SpaceballButtonEvent");
//$ END TEMPLATE EventInitClass
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
SoSpaceballButtonEvent::setButton(SoSpaceballButtonEvent::Button button)
{
  this->button = button;
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
	  ((SoSpaceballButtonEvent *)e)->getButton() == whichButton &&
	  ((SoButtonEvent *)e)->getState() == SoButtonEvent::DOWN);
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
	  ((SoSpaceballButtonEvent *)e)->getButton() == whichButton &&
	  ((SoButtonEvent *)e)->getState() == SoButtonEvent::UP);
}
