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

#ifndef __SOKEYBOARDEVENT_H__
#define __SOKEYBOARDEVENT_H__

#include <Inventor/events/SoButtonEvent.h>


#define SO_KEY_PRESS_EVENT(EVENT, KEY) \
 (SoKeyboardEvent::isKeyPressEvent(EVENT, SoKeyboardEvent::KEY))

#define SO_KEY_RELEASE_EVENT(EVENT, KEY) \
 (SoKeyboardEvent::isKeyReleaseEvent(EVENT, SoKeyboardEvent::KEY))


class SoKeyboardEvent : public SoButtonEvent {
  typedef SoButtonEvent inherited;

//$ BEGIN TEMPLATE EventHeader()
protected:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
//$ END TEMPLATE EventHeader

public:
  enum Key {
    ANY,

    LEFT_SHIFT, RIGHT_SHIFT, LEFT_CONTROL, RIGHT_CONTROL, LEFT_ALT, RIGHT_ALT,

    NUMBER_0, NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4, NUMBER_5, NUMBER_6,
    NUMBER_7, NUMBER_8, NUMBER_9,

    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X,
    Y, Z,

    HOME, LEFT_ARROW, UP_ARROW, RIGHT_ARROW, DOWN_ARROW, PAGE_UP, PAGE_DOWN,
    PRIOR, NEXT, END,

    PAD_ENTER, PAD_F1, PAD_F2, PAD_F3, PAD_F4, PAD_0, PAD_1, PAD_2, PAD_3,
    PAD_4, PAD_5, PAD_6, PAD_7, PAD_8, PAD_9, PAD_ADD, PAD_SUBTRACT,
    PAD_MULTIPLY, PAD_DIVIDE, PAD_SPACE, PAD_TAB, PAD_INSERT, PAD_DELETE,
    PAD_PERIOD,

    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    BACKSPACE, TAB, RETURN, ENTER, PAUSE, SCROLL_LOCK, ESCAPE, DELETE, PRINT,
    INSERT, NUM_LOCK, CAPS_LOCK, SHIFT_LOCK,

    SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH, SEMICOLON, EQUAL,
    BRACKETLEFT, BACKSLASH, BRACKETRIGHT, GRAVE
  };


  SoKeyboardEvent(void);
  virtual ~SoKeyboardEvent();

  void setKey(SoKeyboardEvent::Key key);
  SoKeyboardEvent::Key getKey(void) const;

  char getPrintableCharacter(void) const;

  static SbBool isKeyPressEvent(const SoEvent * e,
                                SoKeyboardEvent::Key whichKey);
  static SbBool isKeyReleaseEvent(const SoEvent * e,
                                  SoKeyboardEvent::Key whichKey);

  static void initClass(void);

private:
  Key key;
};

#endif // !__SOKEYBOARDEVENT_H__
