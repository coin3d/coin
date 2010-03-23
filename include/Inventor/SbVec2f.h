#ifndef COIN_SBVEC2F_H
#define COIN_SBVEC2F_H

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
#include <Inventor/SbString.h>
#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec2d;
class SbVec2b;
class SbVec2s;
class SbVec2i32;

class COIN_DLL_API SbVec2f {
public:
  SbVec2f(void) { }
  SbVec2f(const float v[2]) { vec[0] = v[0]; vec[1] = v[1]; }
  SbVec2f(float x, float y) { vec[0] = x; vec[1] = y; }
  explicit SbVec2f(const SbVec2d & v) { setValue(v); }
  explicit SbVec2f(const SbVec2b & v) { setValue(v); }
  explicit SbVec2f(const SbVec2s & v) { setValue(v); }
  explicit SbVec2f(const SbVec2i32 & v) { setValue(v); }

  SbVec2f & setValue(const float v[2]) { vec[0] = v[0]; vec[1] = v[1]; return *this; }
  SbVec2f & setValue(float x, float y) { vec[0] = x; vec[1] = y; return *this; }
  SbVec2f & setValue(const SbVec2d & v);
  SbVec2f & setValue(const SbVec2b & v);
  SbVec2f & setValue(const SbVec2s & v);
  SbVec2f & setValue(const SbVec2i32 & v);

  const float * getValue(void) const { return vec; }
  void getValue(float & x, float & y) const { x = vec[0]; y = vec[1]; }

  float & operator [] (int i) { return vec[i]; }
  const float & operator [] (int i) const { return vec[i]; }

  float dot(const SbVec2f & v) const { return vec[0] * v[0] + vec[1] * v[1]; }
  SbBool equals(const SbVec2f & v, float tolerance) const;
  float length(void) const;
  float sqrLength(void) const { return vec[0] * vec[0] + vec[1] * vec[1]; }
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; }
  float normalize(void);

  SbVec2f & operator *= (float d) { vec[0] *= d; vec[1] *= d; return *this; }
  SbVec2f & operator /= (float d) { SbDividerChk("SbVec2f::operator/=(float)", d); return operator *= (1.0f / d); }
  SbVec2f & operator += (const SbVec2f & v) { vec[0] += v[0]; vec[1] += v[1]; return *this; }
  SbVec2f & operator -= (const SbVec2f & v) { vec[0] -= v[0]; vec[1] -= v[1]; return *this; }
  SbVec2f operator - (void) const { return SbVec2f(-vec[0], -vec[1]); }

  SbString toString() const;
  SbBool fromString(const SbString & str);

  void print(FILE * fp) const;

protected:
  float vec[2];

}; // SbVec2f

COIN_DLL_API inline SbVec2f operator * (const SbVec2f & v, float d) {
  SbVec2f val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2f operator * (float d, const SbVec2f & v) {
  SbVec2f val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2f operator / (const SbVec2f & v, float d) {
  SbDividerChk("operator/(SbVec2f,float)", d);
  SbVec2f val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2f operator + (const SbVec2f & v1, const SbVec2f & v2) {
  SbVec2f v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec2f operator - (const SbVec2f & v1, const SbVec2f & v2) {
  SbVec2f v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec2f & v1, const SbVec2f & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

COIN_DLL_API inline int operator != (const SbVec2f & v1, const SbVec2f & v2) {
  return !(v1 == v2);
}

// *************************************************************************

#endif // !COIN_SBVEC2F_H
