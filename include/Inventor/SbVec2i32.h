#ifndef COIN_SBVEC2I32_H
#define COIN_SBVEC2I32_H

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
#include <Inventor/system/inttypes.h>
#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec2ui32;
class SbVec2b;
class SbVec2s;
class SbVec2f;
class SbVec2d;

class COIN_DLL_API SbVec2i32 {
public:
  SbVec2i32(void) { }
  SbVec2i32(const int32_t v[2]) { vec[0] = v[0]; vec[1] = v[1]; }
  SbVec2i32(int32_t x, int32_t y) { vec[0] = x; vec[1] = y; }
  explicit SbVec2i32(const SbVec2ui32 & v) { setValue(v); }
  explicit SbVec2i32(const SbVec2b & v) { setValue(v); }
  explicit SbVec2i32(const SbVec2s & v) { setValue(v); }
  explicit SbVec2i32(const SbVec2f & v) { setValue(v); }
  explicit SbVec2i32(const SbVec2d & v) { setValue(v); }

  SbVec2i32 & setValue(const int32_t v[2]) { vec[0] = v[0]; vec[1] = v[1]; return *this; }
  SbVec2i32 & setValue(int32_t x, int32_t y) { vec[0] = x; vec[1] = y; return *this; }
  SbVec2i32 & setValue(const SbVec2ui32 & v);
  SbVec2i32 & setValue(const SbVec2b & v);
  SbVec2i32 & setValue(const SbVec2s & v);
  SbVec2i32 & setValue(const SbVec2f & v);
  SbVec2i32 & setValue(const SbVec2d & v);

  const int32_t * getValue(void) const { return vec; }
  void getValue(int32_t & x, int32_t & y) const { x = vec[0]; y = vec[1]; }

  int32_t & operator [] (const int i) { return vec[i]; }
  const int32_t & operator [] (const int i) const { return vec[i]; }

  int32_t dot(const SbVec2i32 & v) const { return vec[0] * v[0] + vec[1] * v[1]; }
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; }

  SbVec2i32 & operator *= (int d) { vec[0] *= d; vec[1] *= d; return *this; }
  SbVec2i32 & operator *= (double d);
  SbVec2i32 & operator /= (int d) { SbDividerChk("SbVec2i32::operator/=(int)", d); vec[0] /= d; vec[1] /= d; return *this; }
  SbVec2i32 & operator /= (double d) { SbDividerChk("SbVec2i32::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec2i32 & operator += (const SbVec2i32 & v) { vec[0] += v[0]; vec[1] += v[1]; return *this; }
  SbVec2i32 & operator -= (const SbVec2i32 & v) { vec[0] -= v[0]; vec[1] -= v[1]; return *this; }
  SbVec2i32 operator - (void) const { return SbVec2i32(-vec[0], -vec[1]); }

  void print(FILE * fp) const;

protected:
  int32_t vec[2];

}; // SbVec2i32

COIN_DLL_API inline SbVec2i32 operator * (const SbVec2i32 & v, int d) {
  SbVec2i32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator * (const SbVec2i32 & v, double d) {
  SbVec2i32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator * (int d, const SbVec2i32 & v) {
  SbVec2i32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator * (double d, const SbVec2i32 & v) {
  SbVec2i32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator / (const SbVec2i32 & v, int d) {
  SbDividerChk("operator/(SbVec2i32,int)", d);
  SbVec2i32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator / (const SbVec2i32 & v, double d) {
  SbDividerChk("operator/(SbVec2i32,double)", d);
  SbVec2i32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2i32 operator + (const SbVec2i32 & v1, const SbVec2i32 & v2) {
  SbVec2i32 v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec2i32 operator - (const SbVec2i32 & v1, const SbVec2i32 & v2) {
  SbVec2i32 v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec2i32 & v1, const SbVec2i32 & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

COIN_DLL_API inline int operator != (const SbVec2i32& v1, const SbVec2i32& v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC2I32_H
