#ifndef COIN_SBVEC4UI32_H
#define COIN_SBVEC4UI32_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>
#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec4i32;
class SbVec4ub;
class SbVec4us;

class COIN_DLL_API SbVec4ui32 {
public:
  SbVec4ui32(void) { }
  SbVec4ui32(const uint32_t v[4]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; vec[3] = v[3]; }
  SbVec4ui32(uint32_t x, uint32_t y, uint32_t z, uint32_t w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; }
  explicit SbVec4ui32(const SbVec4i32 & v) { setValue(v); }
  explicit SbVec4ui32(const SbVec4ub & v) { setValue(v); }
  explicit SbVec4ui32(const SbVec4us & v) { setValue(v); }

  SbVec4ui32 & setValue(const uint32_t v[4]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; vec[3] = v[3]; return *this; }
  SbVec4ui32 & setValue(uint32_t x, uint32_t y, uint32_t z, uint32_t w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; return *this; }
  SbVec4ui32 & setValue(const SbVec4i32 & v);
  SbVec4ui32 & setValue(const SbVec4ub & v);
  SbVec4ui32 & setValue(const SbVec4us & v);

  const uint32_t * getValue(void) const { return vec; }
  void getValue(uint32_t & x, uint32_t & y, uint32_t & z, uint32_t & w) const { x = vec[0]; y = vec[1]; z = vec[2]; w = vec[3]; }

  uint32_t & operator [] (int i) { return vec[i]; }
  const uint32_t & operator [] (int i) const { return vec[i]; }

  int32_t dot(const SbVec4ui32 & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2] + vec[3] * v[3]; }
  void negate(void);

  SbVec4ui32 & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; vec[3] *= d; return *this; }
  SbVec4ui32 & operator *= (double d);
  SbVec4ui32 & operator /= (int d) { SbDividerChk("SbVec4ui32::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; vec[3] /= d; return *this; }
  SbVec4ui32 & operator /= (double d) { SbDividerChk("SbVec4ui32::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec4ui32 & operator += (const SbVec4ui32 & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; vec[3] += v[3]; return *this; }
  SbVec4ui32 & operator -= (const SbVec4ui32 & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; vec[3] -= v[3]; return *this; }
  SbVec4ui32 operator - (void) const { SbVec4ui32 v(*this); v.negate(); return v; }

protected:
  uint32_t vec[4];

}; // SbVec4ui32

COIN_DLL_API inline SbVec4ui32 operator * (const SbVec4ui32 & v, int d) {
  SbVec4ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator * (const SbVec4ui32 & v, double d) {
  SbVec4ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator * (int d, const SbVec4ui32 & v) {
  SbVec4ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator * (double d, const SbVec4ui32 & v) {
  SbVec4ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator / (const SbVec4ui32 & v, int d) {
  SbDividerChk("operator/(SbVec4ui32,int)", d);
  SbVec4ui32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator / (const SbVec4ui32 & v, double d) {
  SbDividerChk("operator/(SbVec4ui32,double)", d);
  SbVec4ui32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec4ui32 operator + (const SbVec4ui32 & v1, const SbVec4ui32 & v2) {
  SbVec4ui32 v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec4ui32 operator - (const SbVec4ui32 & v1, const SbVec4ui32 & v2) {
  SbVec4ui32 v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec4ui32 & v1, const SbVec4ui32 & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]) && (v1[3] == v2[3]));
}

COIN_DLL_API inline int operator != (const SbVec4ui32 & v1, const SbVec4ui32 & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC4UI32_H
