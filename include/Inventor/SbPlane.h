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

#ifndef COIN_SBPLANE_H
#define COIN_SBPLANE_H

#include <stdio.h>

#include <Inventor/SbVec3f.h>

class SbLine;
class SbMatrix;

class COIN_DLL_EXPORT SbPlane {
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
  friend COIN_DLL_EXPORT int operator ==(const SbPlane& p1, const SbPlane& p2);
  friend COIN_DLL_EXPORT int operator !=(const SbPlane& p1, const SbPlane& p2);

  void print(FILE * file) const;

private:
  SbVec3f normal;
  float distance;
};

COIN_DLL_EXPORT int operator ==(const SbPlane& p1, const SbPlane& p2);
COIN_DLL_EXPORT int operator !=(const SbPlane& p1, const SbPlane& p2);

#endif // !COIN_SBPLANE_H
