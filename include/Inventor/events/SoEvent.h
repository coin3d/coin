#ifndef COIN_SOEVENT_H
#define COIN_SOEVENT_H

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

  // FIXME: this is to avoid a complaint from Doxygen, which has a bug
  // that makes it spit out a warning on undocumented *private*
  // structs and classes. Bug has been reported to
  // <doxygen-develop@lists.sourceforge.net> at 2002-07-30 by mortene.
#ifndef DOXYGEN_SKIP_THIS
  struct {
    unsigned int shiftdown : 1;
    unsigned int ctrldown  : 1;
    unsigned int altdown   : 1;
  } modifiers;
#endif // DOXYGEN_SKIP_THIS
};

#endif // !COIN_SOEVENT_H
