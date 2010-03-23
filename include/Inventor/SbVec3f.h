#ifndef COIN_SBVEC3F_H
#define COIN_SBVEC3F_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbByteBuffer.h>
#include <Inventor/SbString.h>
#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbPlane;
class SbVec3d;
class SbVec3b;
class SbVec3s;
class SbVec3i32;

class COIN_DLL_API SbVec3f {
public:
  SbVec3f(void) { }
  SbVec3f(const float v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3f(float x, float y, float z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3f(const SbVec3d & v) { setValue(v); }
  explicit SbVec3f(const SbVec3b & v) { setValue(v); }
  explicit SbVec3f(const SbVec3s & v) { setValue(v); }
  explicit SbVec3f(const SbVec3i32 & v) { setValue(v); }
  SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);

  SbVec3f & setValue(const float v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3f & setValue(float x, float y, float z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3f & setValue(const SbVec3f & barycentric,
                     const SbVec3f & v0,
                     const SbVec3f & v1,
                     const SbVec3f & v2);
  SbVec3f & setValue(const SbVec3d & v);
  SbVec3f & setValue(const SbVec3b & v);
  SbVec3f & setValue(const SbVec3s & v);
  SbVec3f & setValue(const SbVec3i32 & v);

  const float * getValue(void) const { return vec; }
  void getValue(float & x, float & y, float & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  float & operator [] (int i) { return vec[i]; }
  const float & operator [] (int i) const { return vec[i]; }

  SbBool equals(const SbVec3f & v, float tolerance) const;
  SbVec3f cross(const SbVec3f & v) const;
  float dot(const SbVec3f & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  SbVec3f getClosestAxis(void) const;
  float length(void) const;
  float sqrLength(void) const { return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]; }
  float normalize(void);
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; vec[2] = -vec[2]; }

  SbVec3f & operator *= (float d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3f & operator /= (float d) { SbDividerChk("SbVec3f::operator/=(float)", d); return operator *= (1.0f / d); }
  SbVec3f & operator += (const SbVec3f & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3f & operator -= (const SbVec3f & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3f operator - (void) const { return SbVec3f(-vec[0], -vec[1], -vec[2]); }

  SbString toString() const;
  SbBool fromString(const SbString & str);

  void print(FILE * fp) const;

protected:
  float vec[3];

}; // SbVec3f

COIN_DLL_API inline SbVec3f operator * (const SbVec3f & v, float d) {
  SbVec3f val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3f operator * (float d, const SbVec3f & v) {
  SbVec3f val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3f operator / (const SbVec3f & v, float d) {
  SbDividerChk("operator/(SbVec3f,float)", d);
  SbVec3f val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3f operator + (const SbVec3f & v1, const SbVec3f & v2) {
  SbVec3f v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3f operator - (const SbVec3f & v1, const SbVec3f & v2) {
  SbVec3f v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec3f & v1, const SbVec3f & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (const SbVec3f & v1, const SbVec3f & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3F_H
