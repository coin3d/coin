#ifndef COIN_SBSPHERE_H
#define COIN_SBSPHERE_H

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
