/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoKeyboardEvent SoKeyboardEvent.h Inventor/events/SoKeyboardEvent.h
  \brief The SoKeyboardEvent class contains information about
  keyboard interaction.
  \ingroup events

  When the user presses any keys on the keyboard, these will be translated
  from a system specific event into a Coin event and sent to the
  scenegraph by using instances of this class.

  NB! The key->ASCII value conversion is rather lame at the moment (more
  or less completely US-centric). This will be improved upon.

  \sa SoEvent, SoButtonEvent, SoMouseButtonEvent, SoSpaceballButtonEvent
  \sa SoEventCallback, SoHandleEventAction */

#include <Inventor/events/SoKeyboardEvent.h>
#include <assert.h>

/*¡
  The keycode handling in SoKeyboardEvent is really terrible -- could
  we improve it while still keeping Open Inventor API compatibility?
*/



/*!
  \enum SoKeyboardEvent::Key
  This enum contains all keys detected by Coin.
 */

/*!
  \def SO_KEY_PRESS_EVENT(EVENT, KEY)
  This macro evaluates to \c TRUE iff the \c EVENT represents a press on the
  given \c KEY.
*/
/*!
  \def SO_KEY_RELEASE_EVENT(EVENT, KEY)
  This macro evaluates to \c TRUE iff the \c EVENT represents a release of the
  given \c KEY.
*/


static char converttoprintable[] = {
//    ANY
  '.',
//    LEFT_SHIFT RIGHT_SHIFT LEFT_CONTROL RIGHT_CONTROL LEFT_ALT RIGHT_ALT
  '.', '.', '.', '.', '.', '.',
//      NUMBER_0 NUMBER_1 NUMBER_2 NUMBER_3 NUMBER_4 NUMBER_5 NUMBER_6
//      NUMBER_7 NUMBER_8 NUMBER_9
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
//      HOME LEFT_ARROW UP_ARROW RIGHT_ARROW DOWN_ARROW PAGE_UP PAGE_DOWN
//      PRIOR NEXT END
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      PAD_ENTER PAD_F1 PAD_F2 PAD_F3 PAD_F4 PAD_0 PAD_1 PAD_2 PAD_3
//      PAD_4 PAD_5 PAD_6 PAD_7 PAD_8 PAD_9 PAD_ADD PAD_SUBTRACT
//      PAD_MULTIPLY PAD_DIVIDE PAD_SPACE PAD_TAB PAD_INSERT PAD_DELETE
//      PAD_PERIOD
  '.', '.', '.', '.', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  '+', '-', '*', '/', ' ', '.', '.', '.', '.',
//      F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      BACKSPACE TAB RETURN ENTER PAUSE SCROLL_LOCK ESCAPE DELETE PRINT
//      INSERT NUM_LOCK CAPS_LOCK SHIFT_LOCK
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      SPACE APOSTROPHE COMMA MINUS PERIOD SLASH SEMICOLON EQUAL
//      BRACKETLEFT BACKSLASH BRACKETRIGHT GRAVE
  ' ', '\'', ',', '-', '.', '/', ';', '=', '[', '\\', ']', '~'
};

static char converttoprintable_shift[] = {
//    ANY
  '.',
//    LEFT_SHIFT RIGHT_SHIFT LEFT_CONTROL RIGHT_CONTROL LEFT_ALT RIGHT_ALT
  '.', '.', '.', '.', '.', '.',
//      NUMBER_0 NUMBER_1 NUMBER_2 NUMBER_3 NUMBER_4 NUMBER_5 NUMBER_6
//      NUMBER_7 NUMBER_8 NUMBER_9
  ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
//      A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
//      HOME LEFT_ARROW UP_ARROW RIGHT_ARROW DOWN_ARROW PAGE_UP PAGE_DOWN
//      PRIOR NEXT END
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      PAD_ENTER PAD_F1 PAD_F2 PAD_F3 PAD_F4 PAD_0 PAD_1 PAD_2 PAD_3
//      PAD_4 PAD_5 PAD_6 PAD_7 PAD_8 PAD_9 PAD_ADD PAD_SUBTRACT
//      PAD_MULTIPLY PAD_DIVIDE PAD_SPACE PAD_TAB PAD_INSERT PAD_DELETE
//      PAD_PERIOD
  '.', '.', '.', '.', '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  '+', '-', '*', '/', ' ', '.', '.', '.', '.',
//      F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      BACKSPACE TAB RETURN ENTER PAUSE SCROLL_LOCK ESCAPE DELETE PRINT
//      INSERT NUM_LOCK CAPS_LOCK SHIFT_LOCK
  '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
//      SPACE APOSTROPHE COMMA MINUS PERIOD SLASH SEMICOLON EQUAL
//      BRACKETLEFT BACKSLASH BRACKETRIGHT GRAVE
  ' ', '\'', ',', '-', '.', '/', ';', '=', '[', '\\', ']', '~'
};

SO_EVENT_SOURCE(SoKeyboardEvent);

/*!
  Initialize the type information data.
 */
void
SoKeyboardEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(SoKeyboardEvent, SoButtonEvent);
}

/*!
  Constructor.
 */
SoKeyboardEvent::SoKeyboardEvent(void)
{
  this->key = SoKeyboardEvent::ANY;
}

/*!
  Destructor.
*/
SoKeyboardEvent::~SoKeyboardEvent()
{
}

/*!
  Set the value of the key which the user interacted with.

  This method is used from the window specific device classes when
  translating events to the generic Coin library.

  \sa getKey()
 */
void
SoKeyboardEvent::setKey(SoKeyboardEvent::Key key)
{
  this->key = key;
}

/*!
  Returns the value of the key which was pressed or released.

  \sa getPrintableCharacter(), getState()
  \sa wasShiftDown(), wasCtrlDown(), wasAltDown(), getPosition(), getTime()
 */
SoKeyboardEvent::Key
SoKeyboardEvent::getKey(void) const
{
  return this->key;
}

/*!
  Convenience method for quickly checking if the given event is a
  keypress on the given key, \c whichKey.

  \sa isKeyReleaseEvent(), isOfType(), getKey(), getState()
 */
SbBool
SoKeyboardEvent::isKeyPressEvent(const SoEvent * e,
                                 SoKeyboardEvent::Key whichKey)
{
  return (e->isOfType(SoKeyboardEvent::getClassTypeId()) &&
          (whichKey == SoKeyboardEvent::ANY ||
           ((SoKeyboardEvent *)e)->getKey() == whichKey) &&
          ((SoButtonEvent *)e)->getState() == SoButtonEvent::DOWN);
}

/*!
  Convenience method for quickly checking if the given event is a
  key release of the given key, \c whichKey.

  \sa isKeyPressEvent(), isOfType(), getKey(), getState()
 */
SbBool
SoKeyboardEvent::isKeyReleaseEvent(const SoEvent * e,
                                   SoKeyboardEvent::Key whichKey)
{
  return (e->isOfType(SoKeyboardEvent::getClassTypeId()) &&
          (whichKey == SoKeyboardEvent::ANY ||
           ((SoKeyboardEvent *)e)->getKey() == whichKey) &&
          ((SoButtonEvent *)e)->getState() == SoButtonEvent::UP);
}

/*!
  Return ASCII value which would be generated by the key and
  modifier combination.

  NB! Does not always work as expected, particularly not on non-US
  keyboards.

  \sa getKey(), wasShiftDown(), wasCtrlDown(), wasAltDown()
 */
char
SoKeyboardEvent::getPrintableCharacter(void) const
{
  // FIXME: this method is complete and utter bollocks in OIV. Should
  // make a proper one for Coin. 19990213 mortene.

  char * ca =
    this->wasShiftDown() ? converttoprintable_shift : converttoprintable;
  return ca[this->getKey()];
}
