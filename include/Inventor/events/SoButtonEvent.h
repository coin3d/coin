#ifndef COIN_SOBUTTONEVENT_H
#define COIN_SOBUTTONEVENT_H

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

#include <Inventor/events/SoEvent.h>

class SbString;

class COIN_DLL_API SoButtonEvent : public SoEvent {
  typedef SoEvent inherited;

  SO_EVENT_HEADER();

public:
  static void initClass(void);

  SoButtonEvent(void);
  virtual ~SoButtonEvent();

  enum State {
    UP, DOWN, UNKNOWN
  };

  void setState(State state);
  State getState(void) const;

  static SbBool enumToString(State enumval, SbString & stringrep);

private:
  State buttonstate;

}; // SoButtonEvent

#endif // !COIN_SOBUTTONEVENT_H
