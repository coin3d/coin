#ifndef COIN_SOEVENT_H
#define COIN_SOEVENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SoType.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbTime.h>

class SbViewportRegion;
class SbVec2f;

class COIN_DLL_API SoEvent {

public:
  SoEvent(void);
  virtual ~SoEvent();

  SbBool isOfType(SoType type) const;
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

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

  static void initClass(void);

private:
  static SoType classTypeId;

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
