/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOEVENT_H__
#define __SOEVENT_H__

#include <Inventor/SoType.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbTime.h>

#if defined(COIN_EXCLUDE_SOEVENT)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOEVENT

class SbViewportRegion;
class SbVec2f;


class SoEvent {
//$ BEGIN TEMPLATE EventHeader()
protected:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
//$ END TEMPLATE EventHeader

public:
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

  static void initClass(void);

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

#endif // !__SOEVENT_H__
