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

#ifndef __SOMOTION3EVENT_H__
#define __SOMOTION3EVENT_H__

#include <Inventor/events/SoEvent.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>

#if defined(COIN_EXCLUDE_SOMOTION3EVENT)
#error "Configuration settings not respected, should not include this file!"
#endif // COIN_EXCLUDE_SOMOTION3EVENT


class SoMotion3Event : public SoEvent {
  typedef SoEvent inherited;

//$ BEGIN TEMPLATE EventHeader()
protected:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
//$ END TEMPLATE EventHeader

public:
  SoMotion3Event(void);
  virtual ~SoMotion3Event();

  void setTranslation(const SbVec3f & t);
  const SbVec3f & getTranslation(void) const;

  void setRotation(const SbRotation & r);
  const SbRotation & getRotation(void) const;

  static void initClass(void);

private:
  SbVec3f translation;
  SbRotation rotation;
};

#endif // !__SOMOTION3EVENT_H__
