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

#ifndef COIN_SOSPACEBALLBUTTONEVENT_H
#define COIN_SOSPACEBALLBUTTONEVENT_H

#include <Inventor/events/SoButtonEvent.h>

#define SO_SPACEBALL_PRESS_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonPressEvent(EVENT, SoSpaceballButtonEvent::BUTTON))

#define SO_SPACEBALL_RELEASE_EVENT(EVENT, BUTTON) \
 (SoSpaceballButtonEvent::isButtonReleaseEvent(EVENT, SoSpaceballButtonEvent::BUTTON))


class COIN_DLL_API SoSpaceballButtonEvent : public SoButtonEvent {
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
