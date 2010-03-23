#ifndef COIN_SOMOUSEBUTTONEVENT_H
#define COIN_SOMOUSEBUTTONEVENT_H

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

#define SO_MOUSE_PRESS_EVENT(EVENT, BUTTON) \
 (SoMouseButtonEvent::isButtonPressEvent(EVENT, SoMouseButtonEvent::BUTTON))

#define SO_MOUSE_RELEASE_EVENT(EVENT, BUTTON) \
 (SoMouseButtonEvent::isButtonReleaseEvent(EVENT, SoMouseButtonEvent::BUTTON))

class COIN_DLL_API SoMouseButtonEvent : public SoButtonEvent {
  typedef SoButtonEvent inherited;

  SO_EVENT_HEADER();

public:
  static void initClass(void);

  SoMouseButtonEvent(void);
  virtual ~SoMouseButtonEvent();

  enum Button {
    ANY, BUTTON1, BUTTON2, BUTTON3, BUTTON4, BUTTON5
  };

  void setButton(Button button);
  Button getButton(void) const;

  static SbBool isButtonPressEvent(const SoEvent * e, Button whichButton);
  static SbBool isButtonReleaseEvent(const SoEvent * e, Button whichButton);

  static SbBool enumToString(Button enumval, SbString & stringrep);

private:
  Button button;

}; // SoMouseButtonEvent

#endif // !COIN_SOMOUSEBUTTONEVENT_H
