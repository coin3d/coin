#ifndef COIN_SBDPPLANE_H
#define COIN_SBDPPLANE_H

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

#include <Inventor/SbVec3d.h>

class SbDPLine;
class SbDPMatrix;

class COIN_DLL_API SbDPPlane {
public:
  SbDPPlane(void);
  SbDPPlane(const SbVec3d & normal, const double D);
  SbDPPlane(const SbVec3d & p0, const SbVec3d & p1, const SbVec3d & p2);
  SbDPPlane(const SbVec3d & normal, const SbVec3d & point);

  void offset(const double d);
  SbBool intersect(const SbDPLine & l, SbVec3d & intersection) const;
  void transform(const SbDPMatrix & matrix);
  SbBool isInHalfSpace(const SbVec3d & point) const;
  double getDistance(const SbVec3d & point) const;
  const SbVec3d & getNormal(void) const;
  double getDistanceFromOrigin(void) const;
  friend COIN_DLL_API int operator ==(const SbDPPlane & p1, const SbDPPlane & p2);
  friend COIN_DLL_API int operator !=(const SbDPPlane & p1, const SbDPPlane & p2);

  void print(FILE * file) const;

  SbBool intersect(const SbDPPlane & pl, SbDPLine & line);

private:
  SbVec3d normal;
  double distance;
};

COIN_DLL_API int operator ==(const SbDPPlane & p1, const SbDPPlane & p2);
COIN_DLL_API int operator !=(const SbDPPlane & p1, const SbDPPlane & p2);

#endif // !COIN_SBPLANE_H
