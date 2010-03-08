#ifndef COIN_SBVEC3UI32_H
#define COIN_SBVEC3UI32_H

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

class SbVec3i32;
class SbVec3ub;
class SbVec3us;

class COIN_DLL_API SbVec3ui32 {
public:
  SbVec3ui32(void) { }
  SbVec3ui32(const uint32_t v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3ui32(uint32_t x, uint32_t y, uint32_t z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3ui32(const SbVec3i32 & v) { setValue(v); }
  explicit SbVec3ui32(const SbVec3ub & v) { setValue(v); }
  explicit SbVec3ui32(const SbVec3us & v) { setValue(v); }

  SbVec3ui32 & setValue(const uint32_t v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3ui32 & setValue(uint32_t x, uint32_t y, uint32_t z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3ui32 & setValue(const SbVec3i32 & v);
  SbVec3ui32 & setValue(const SbVec3ub & v);
  SbVec3ui32 & setValue(const SbVec3us & v);

  const uint32_t * getValue(void) const { return vec; }
  void getValue(uint32_t & x, uint32_t & y, uint32_t & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  uint32_t & operator [] (int i) { return vec[i]; }
  const uint32_t & operator [] (int i) const { return vec[i]; }

  int32_t dot(const SbVec3ui32 & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  void negate(void);

  SbVec3ui32 & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3ui32 & operator *= (double d);
  SbVec3ui32 & operator /= (int d) { SbDividerChk("SbVec3ui32::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; return *this; }
  SbVec3ui32 & operator /= (double d) { SbDividerChk("SbVec3ui32::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec3ui32 & operator += (const SbVec3ui32 & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3ui32 & operator -= (const SbVec3ui32 & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3ui32 operator - (void) const { SbVec3ui32 v(*this); v.negate(); return v; }

protected:
  uint32_t vec[3];

}; // SbVec3ui32

COIN_DLL_API inline SbVec3ui32 operator * (const SbVec3ui32 & v, int d) {
  SbVec3ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator * (const SbVec3ui32 & v, double d) {
  SbVec3ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator * (int d, const SbVec3ui32 & v) {
  SbVec3ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator * (double d, const SbVec3ui32 & v) {
  SbVec3ui32 val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator / (const SbVec3ui32 & v, int d) {
  SbDividerChk("operator/(SbVec3ui32,int)", d);
  SbVec3ui32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator / (const SbVec3ui32 & v, double d) {
  SbDividerChk("operator/(SbVec3ui32,double)", d);
  SbVec3ui32 val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3ui32 operator + (const SbVec3ui32 & v1, const SbVec3ui32 & v2) {
  SbVec3ui32 v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3ui32 operator - (const SbVec3ui32 & v1, const SbVec3ui32 & v2) {
  SbVec3ui32 v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec3ui32 & v1, const SbVec3ui32 & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (const SbVec3ui32 & v1, const SbVec3ui32 & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3UI32_H
