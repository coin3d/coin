#ifndef COIN_SOEVENT_H
#define COIN_SOEVENT_H

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

#include <Inventor/SoType.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbTime.h>
#include <Inventor/events/SoSubEvent.h>

class SbViewportRegion;
class SbVec2f;

class COIN_DLL_API SoEvent {
  SO_EVENT_ABSTRACT_HEADER();
public:
  static void initClass(void);

  SoEvent(void);
  virtual ~SoEvent();

  SbBool isOfType(SoType type) const;

  void setTime(const SbTime t);
  SbTime getTime(void) const;

  void setPosition(const SbVec2s & p);
  const SbVec2s & getPosition(void) const;
  const SbVec2s getPosition(const SbViewportRegion & vpRgn) const;
  const SbVec2f getNormalizedPosition(const SbViewportRegion & vpRgn) const;

  void setShiftDown(SbBool isDown);
  SbBool wasShiftDown(void) const;
  void setCtrlDown(SbBool isDown);
  SbBool wasCtrlDown(void) const;
  void setAltDown(SbBool isDown);
  SbBool wasAltDown(void) const;

private:
  SbTime timeofevent;
  SbVec2s positionofevent;

  static void initEvents(void);

  struct {
    unsigned int shiftdown : 1;
    unsigned int ctrldown  : 1;
    unsigned int altdown   : 1;
  } modifiers;
};

#endif // !COIN_SOEVENT_H
