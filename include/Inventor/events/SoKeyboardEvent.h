#ifndef COIN_SOKEYBOARDEVENT_H
#define COIN_SOKEYBOARDEVENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/events/SoButtonEvent.h>

// Avoid problem with Microsoft Win32 API headers (yes, they actually
// #define DELETE somewhere in their header files).
#if defined(DELETE)
#define SOKEYBOARDEVENT_STORE_DELETE_DEF DELETE
#undef DELETE
#endif /* DELETE */


#define SO_KEY_PRESS_EVENT(EVENT, KEY) \
 (SoKeyboardEvent::isKeyPressEvent(EVENT, SoKeyboardEvent::KEY))

#define SO_KEY_RELEASE_EVENT(EVENT, KEY) \
 (SoKeyboardEvent::isKeyReleaseEvent(EVENT, SoKeyboardEvent::KEY))


class COIN_DLL_API SoKeyboardEvent : public SoButtonEvent {
  typedef SoButtonEvent inherited;

  SO_EVENT_HEADER();

public:
  enum Key {
    ANY = 0,
    UNDEFINED = 1,

    LEFT_SHIFT = 0xffe1, RIGHT_SHIFT, LEFT_CONTROL, RIGHT_CONTROL,
    LEFT_ALT = 0xffe9, RIGHT_ALT,

    NUMBER_0 = 0x0030, NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4, NUMBER_5,
    NUMBER_6, NUMBER_7, NUMBER_8, NUMBER_9,

    A = 0x0061, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T,
    U, V, W, X, Y, Z,

    HOME = 0xff50, LEFT_ARROW, UP_ARROW, RIGHT_ARROW, DOWN_ARROW,
    PAGE_UP, PAGE_DOWN, END,
    PRIOR = 0xff55, NEXT,

    PAD_ENTER = 0xff8d,
    PAD_F1 = 0xff91, PAD_F2, PAD_F3, PAD_F4,
    PAD_0 = 0xff9e, PAD_1 = 0xff9c, PAD_2 = 0xff99, PAD_3 = 0xff9b,
    PAD_4 = 0xff96, PAD_5 = 0xff9d, PAD_6 = 0xff98, PAD_7 = 0xff95,
    PAD_8 = 0xff97, PAD_9 = 0xff9a,
    PAD_ADD = 0xffab, PAD_SUBTRACT = 0xffad,
    PAD_MULTIPLY = 0xffaa, PAD_DIVIDE = 0xffaf,
    PAD_SPACE = 0xff8d, PAD_TAB = 0xff89,
    PAD_INSERT = 0xff9e, PAD_DELETE = 0xff9f, PAD_PERIOD = 0xff9f,

    F1 = 0xffbe, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    BACKSPACE = 0xff08, TAB = 0xff09,
    RETURN = 0xff0d, ENTER = 0xff0d,
    PAUSE = 0xff13, SCROLL_LOCK = 0xff14,
    ESCAPE = 0xff1b, DELETE = 0xffff, KEY_DELETE = DELETE,
    PRINT = 0xff61, INSERT = 0xff63,
    NUM_LOCK = 0xff7f, CAPS_LOCK = 0xffe5, SHIFT_LOCK = 0xffe6,

    SPACE = 0x0020, APOSTROPHE = 0x0027,
    COMMA = 0x002c, MINUS = 0x002d, PERIOD = 0x002e, SLASH = 0x002f,
    SEMICOLON = 0x003b, EQUAL = 0x003d,
    BRACKETLEFT = 0x005b, BACKSLASH = 0x005c,
    BRACKETRIGHT = 0x005d, GRAVE = 0x0060
  };

  SoKeyboardEvent(void);
  virtual ~SoKeyboardEvent();

  void setKey(SoKeyboardEvent::Key key);
  SoKeyboardEvent::Key getKey(void) const;

  void setPrintableCharacter(const char c);
  char getPrintableCharacter(void) const;

  static SbBool isKeyPressEvent(const SoEvent * e,
                                SoKeyboardEvent::Key whichKey);
  static SbBool isKeyReleaseEvent(const SoEvent * e,
                                  SoKeyboardEvent::Key whichKey);

  static void initClass(void);

private:
  Key key;
  char printable;
  char isprintableset;
};


// Avoid problem with Microsoft Win32 API headers (see above).
#if defined(SOKEYBOARDEVENT_STORE_DELETE_DEF)
#define DELETE SOKEYBOARDEVENT_STORE_DELETE_DEF
#undef SOKEYBOARDEVENT_STORE_DELETE_DEF
#endif /* SOKEYBOARDEVENT_STORE_DELETE_DEF */

#endif // !COIN_SOKEYBOARDEVENT_H
