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

#ifndef COIN_SBSPHERE_H
#define COIN_SBSPHERE_H

#include <stdio.h>
#include <Inventor/SbVec3f.h>

class SbLine;
class SbBox3f;

class COIN_DLL_API SbSphere {
public:
  SbSphere(void);
  SbSphere(const SbVec3f & center, const float radius);
  void setValue(const SbVec3f & center, const float radius);
  void setCenter(const SbVec3f & center);
  void setRadius(const float radius);
  const SbVec3f &  getCenter(void) const;
  float getRadius(void) const;
  void circumscribe(const SbBox3f & box);
  SbBool intersect(const SbLine & l, SbVec3f & intersection) const;
  SbBool intersect(const SbLine & l, SbVec3f & enter, SbVec3f & exit) const;
  SbBool pointInside(const SbVec3f & p) const;

  void print(FILE * file) const;

private:
  SbVec3f center;
  float radius;
};

#endif // !COIN_SBSPHERE_H
