#ifndef COIN_SBROTATION_H
#define COIN_SBROTATION_H

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

#include <stdio.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbByteBuffer.h>
#include <Inventor/SbString.h>

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
  SbRotation & operator*=(const SbRotation & q);
  SbRotation & operator*=(const float s);
  friend COIN_DLL_API int operator==(const SbRotation & q1, const SbRotation & q2);
  friend COIN_DLL_API int operator!=(const SbRotation & q1, const SbRotation & q2);
  float operator[] (size_t n) const;

  SbBool equals(const SbRotation & r, float tolerance) const;
  friend COIN_DLL_API SbRotation operator *(const SbRotation & q1, const SbRotation & q2);
  void multVec(const SbVec3f & src, SbVec3f & dst) const;

  void scaleAngle(const float scaleFactor);
  static SbRotation slerp(const SbRotation & rot0, const SbRotation & rot1,
                          float t);
  static SbRotation identity(void);

  SbString toString() const;
  SbBool fromString(const SbString & str);

  void print(FILE * fp) const;

private:
  SbVec4f quat;
};

COIN_DLL_API int operator ==(const SbRotation & q1, const SbRotation & q2);
COIN_DLL_API int operator !=(const SbRotation & q1, const SbRotation & q2);
COIN_DLL_API SbRotation operator *(const SbRotation & q1, const SbRotation & q2);

inline float SbRotation::operator[](size_t n) const
{
  //Any limit checking is delegated to SbVec4f
  return quat[n];
}

#endif // !COIN_SBROTATION_H
