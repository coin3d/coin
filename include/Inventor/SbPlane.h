#ifndef COIN_SBPLANE_H
#define COIN_SBPLANE_H

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

#include <stdio.h>

#include <Inventor/SbVec3f.h>

class SbLine;
class SbMatrix;

class COIN_DLL_API SbPlane {
public:
  SbPlane(void);
  SbPlane(const SbVec3f& normal, const float D);
  SbPlane(const SbVec3f& p0, const SbVec3f& p1, const SbVec3f& p2);
  SbPlane(const SbVec3f& normal, const SbVec3f& point);

  void offset(const float d);
  SbBool intersect(const SbLine& l, SbVec3f& intersection) const;
  void transform(const SbMatrix& matrix);
  SbBool isInHalfSpace(const SbVec3f& point) const;
  float getDistance(const SbVec3f &point) const;
  const SbVec3f& getNormal(void) const;
  float getDistanceFromOrigin(void) const;
  friend COIN_DLL_API int operator ==(const SbPlane& p1, const SbPlane& p2);
  friend COIN_DLL_API int operator !=(const SbPlane& p1, const SbPlane& p2);

  void print(FILE * file) const;

  SbBool intersect(const SbPlane & pl, SbLine & line);

private:
  SbVec3f normal;
  float distance;
};

COIN_DLL_API int operator ==(const SbPlane& p1, const SbPlane& p2);
COIN_DLL_API int operator !=(const SbPlane& p1, const SbPlane& p2);

#endif // !COIN_SBPLANE_H
