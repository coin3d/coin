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

#ifndef COIN_SOMOTION3EVENT_H
#define COIN_SOMOTION3EVENT_H

#include <Inventor/events/SoSubEvent.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>


class SoMotion3Event : public SoEvent {
  typedef SoEvent inherited;

  SO_EVENT_HEADER();

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

#endif // !COIN_SOMOTION3EVENT_H
