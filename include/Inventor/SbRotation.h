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

#ifndef COIN_SBROTATION_H
#define COIN_SBROTATION_H

#include <stdio.h>
#include <Inventor/SbVec4f.h>

class SbMatrix;
class SbVec3f;

class COIN_DLL_API SbRotation {
public:
  SbRotation(void);
  SbRotation(const SbVec3f & axis, const float radians);
  SbRotation(const float q[4]);
  SbRotation(const float q0, const float q1, const float q2, const float q3);
  SbRotation(const SbMatrix & m);
  SbRotation(const SbVec3f & rotateFrom, const SbVec3f & rotateTo);
  const float * getValue(void) const;
  void getValue(float & q0, float & q1, float & q2, float & q3) const;
  SbRotation & setValue(const float q0, const float q1,
                        const float q2, const float q3);
  void getValue(SbVec3f & axis, float & radians) const;
  void getValue(SbMatrix & matrix) const;
  SbRotation & invert(void);
  SbRotation inverse(void) const;
  SbRotation & setValue(const float q[4]);
  SbRotation & setValue(const SbMatrix & m);
  SbRotation & setValue(const SbVec3f & axis, const float radians);
  SbRotation & setValue(const SbVec3f & rotateFrom, const SbVec3f & rotateTo);
  SbRotation & operator *=(const SbRotation & q);
  friend COIN_DLL_API int operator ==(const SbRotation & q1, const SbRotation & q2);
  friend COIN_DLL_API int operator !=(const SbRotation & q1, const SbRotation & q2);
  SbBool equals(const SbRotation & r, const float tolerance) const;
  friend COIN_DLL_API SbRotation operator *(const SbRotation & q1, const SbRotation & q2);
  void multVec(const SbVec3f & src, SbVec3f & dst) const;

  void scaleAngle(const float scaleFactor);
  static SbRotation slerp(const SbRotation & rot0, const SbRotation & rot1,
                          float t);
  static SbRotation identity(void);

  void print(FILE * fp) const;

private:
  SbVec4f quat;
};

COIN_DLL_API int operator ==(const SbRotation & q1, const SbRotation & q2);
COIN_DLL_API int operator !=(const SbRotation & q1, const SbRotation & q2);
COIN_DLL_API SbRotation operator *(const SbRotation & q1, const SbRotation & q2);

#endif // !COIN_SBROTATION_H
