/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOMOTION3EVENT_H
#define COIN_SOMOTION3EVENT_H

#include <Inventor/events/SoSubEvent.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>


class COIN_DLL_EXPORT SoMotion3Event : public SoEvent {
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
