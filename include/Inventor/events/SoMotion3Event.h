#ifndef COIN_SOMOTION3EVENT_H
#define COIN_SOMOTION3EVENT_H

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
#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>

class COIN_DLL_API SoMotion3Event : public SoEvent {
  typedef SoEvent inherited;

  SO_EVENT_HEADER();

public:
  static void initClass(void);

  SoMotion3Event(void);
  virtual ~SoMotion3Event();

  void setTranslation(const SbVec3f & t);
  const SbVec3f & getTranslation(void) const;

  void setRotation(const SbRotation & r);
  const SbRotation & getRotation(void) const;

private:
  SbVec3f translation;
  SbRotation rotation;
};

#endif // !COIN_SOMOTION3EVENT_H
