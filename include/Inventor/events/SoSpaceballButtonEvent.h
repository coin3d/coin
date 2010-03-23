#ifndef COIN_SOSPACEBALLBUTTONEVENT_H
#define COIN_SOSPACEBALLBUTTONEVENT_H

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

#include <Inventor/events/SoButtonEvent.h>

#define SO_SPACEBALL_PRESS_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonPressEvent(EVENT, SoSpaceballButtonEvent::BUTTON))

#define SO_SPACEBALL_RELEASE_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonReleaseEvent(EVENT, SoSpaceballButtonEvent::BUTTON))


class COIN_DLL_API SoSpaceballButtonEvent : public SoButtonEvent {
  typedef SoButtonEvent inherited;

  SO_EVENT_HEADER();

public:
  static void initClass(void);

  SoSpaceballButtonEvent(void);
  virtual ~SoSpaceballButtonEvent();

  enum Button {
    ANY, BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5, BUTTON6, BUTTON7,
    BUTTON8, PICK
  };

  void setButton(Button button);
  Button getButton(void) const;

  static SbBool isButtonPressEvent(const SoEvent * e, Button whichButton);
  static SbBool isButtonReleaseEvent(const SoEvent * e, Button whichButton);

  static SbBool enumToString(Button enumval, SbString & stringrep);

private:
  Button button;

}; // SoSpaceballButtonEvent

#endif // !COIN_SOSPACEBALLBUTTONEVENT_H
