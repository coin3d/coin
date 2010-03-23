#ifndef COIN_SBVEC3UB_H
#define COIN_SBVEC3UB_H

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

class SbVec3b;
class SbVec3us;
class SbVec3ui32;

class COIN_DLL_API SbVec3ub {
public:
  SbVec3ub(void) { }
  SbVec3ub(const uint8_t v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3ub(uint8_t x, uint8_t y, uint8_t z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3ub(const SbVec3b & v) { setValue(v); }
  explicit SbVec3ub(const SbVec3us & v) { setValue(v); }
  explicit SbVec3ub(const SbVec3ui32 & v) { setValue(v); }

  SbVec3ub & setValue(const uint8_t v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3ub & setValue(uint8_t x, uint8_t y, uint8_t z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3ub & setValue(const SbVec3b & v);
  SbVec3ub & setValue(const SbVec3us & v);
  SbVec3ub & setValue(const SbVec3ui32 & v);

  const uint8_t * getValue(void) const { return vec; }
  void getValue(uint8_t & x, uint8_t & y, uint8_t & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  uint8_t & operator [] (int i) { return vec[i]; }
  const uint8_t & operator [] (int i) const { return vec[i]; }

  int32_t dot(SbVec3ub v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  void negate(void);

  SbVec3ub & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3ub & operator *= (double d);
  SbVec3ub & operator /= (int d) { SbDividerChk("SbVec3ub::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; return *this; }
  SbVec3ub & operator /= (double d) { SbDividerChk("SbVec3ub::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec3ub & operator += (SbVec3ub v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3ub & operator -= (SbVec3ub v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3ub operator - (void) const { SbVec3ub v(*this); v.negate(); return v; }

protected:
  uint8_t vec[3];

}; // SbVec3ub

COIN_DLL_API inline SbVec3ub operator * (SbVec3ub v, int d) {
  SbVec3ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ub operator * (SbVec3ub v, double d) {
  SbVec3ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ub operator * (int d, SbVec3ub v) {
  SbVec3ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ub operator * (double d, SbVec3ub v) {
  SbVec3ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3ub operator / (SbVec3ub v, int d) {
  SbDividerChk("operator/(SbVec3ub,int)", d);
  SbVec3ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3ub operator / (SbVec3ub v, double d) {
  SbDividerChk("operator/(SbVec3ub,double)", d);
  SbVec3ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3ub operator + (SbVec3ub v1, SbVec3ub v2) {
  SbVec3ub v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3ub operator - (SbVec3ub v1, SbVec3ub v2) {
  SbVec3ub v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (SbVec3ub v1, SbVec3ub v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (SbVec3ub v1, SbVec3ub v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3UB_H
