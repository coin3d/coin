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

#ifndef COIN_SOSPACEBALLBUTTONEVENT_H
#define COIN_SOSPACEBALLBUTTONEVENT_H

#include <Inventor/events/SoButtonEvent.h>

#define SO_SPACEBALL_PRESS_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonPressEvent(EVENT, SoSpaceballButtonEvent::BUTTON))

#define SO_SPACEBALL_RELEASE_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonReleaseEvent(EVENT, SoSpaceballButtonEvent::BUTTON))


class SoSpaceballButtonEvent : public SoButtonEvent {
  typedef SoButtonEvent inherited;

  SO_EVENT_HEADER();

public:
  enum Button {
    ANY, BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7,
    BUTTON8, PICK
  };

  SoSpaceballButtonEvent(void);
  virtual ~SoSpaceballButtonEvent();

  void setButton(SoSpaceballButtonEvent::Button button);
  SoSpaceballButtonEvent::Button getButton(void) const;

  static SbBool isButtonPressEvent(const SoEvent * e,
                                   SoSpaceballButtonEvent::Button whichButton);
  static SbBool isButtonReleaseEvent(const SoEvent * e,
                                     SoSpaceballButtonEvent::Button whichButton);

  static void initClass(void);

private:
  Button button;
};

#endif // !COIN_SOSPACEBALLBUTTONEVENT_H
