#ifndef COIN_SBVEC4UB_H
#define COIN_SBVEC4UB_H

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

class SbVec4b;
class SbVec4us;
class SbVec4ui32;

class COIN_DLL_API SbVec4ub {
public:
  SbVec4ub(void) { }
  SbVec4ub(const uint8_t v[4]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; vec[3] = v[3]; }
  SbVec4ub(uint8_t x, uint8_t y, uint8_t z, uint8_t w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; }
  explicit SbVec4ub(const SbVec4b & v) { setValue(v); }
  explicit SbVec4ub(const SbVec4us & v) { setValue(v); }
  explicit SbVec4ub(const SbVec4ui32 & v) { setValue(v); }

  SbVec4ub & setValue(const uint8_t v[4]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; vec[3] = v[3]; return *this; } 
  SbVec4ub & setValue(uint8_t x, uint8_t y, uint8_t z, uint8_t w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; return *this; } 
  SbVec4ub & setValue(const SbVec4b & v);
  SbVec4ub & setValue(const SbVec4us & v);
  SbVec4ub & setValue(const SbVec4ui32 & v);

  const uint8_t * getValue(void) const { return vec; }
  void getValue(uint8_t & x, uint8_t & y, uint8_t & z, uint8_t & w) const { x = vec[0]; y = vec[1]; z = vec[2]; w = vec[3]; }

  uint8_t & operator [] (int i) { return vec[i]; }
  const uint8_t & operator [] (int i) const { return vec[i]; }

  int32_t dot(SbVec4ub v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2] + vec[3] * v[3]; }
  void negate(void);

  SbVec4ub & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; vec[3] *= d; return *this; }
  SbVec4ub & operator *= (double d);
  SbVec4ub & operator /= (int d) { SbDividerChk("SbVec4ub::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; vec[3] /= d; return *this; }
  SbVec4ub & operator /= (double d) { SbDividerChk("SbVec4ub::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec4ub & operator += (SbVec4ub v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; vec[3] += v[3]; return *this; }
  SbVec4ub & operator -= (SbVec4ub v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; vec[3] -= v[3]; return *this; }
  SbVec4ub operator - (void) const { SbVec4ub v(*this); v.negate(); return v; }

protected:
  uint8_t vec[4];

}; // SbVec4ub

COIN_DLL_API inline SbVec4ub operator * (SbVec4ub v, int d) {
  SbVec4ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ub operator * (SbVec4ub v, double d) {
  SbVec4ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ub operator * (int d, SbVec4ub v) {
  SbVec4ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ub operator * (double d, SbVec4ub v) {
  SbVec4ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec4ub operator / (SbVec4ub v, int d) {
  SbDividerChk("operator/(SbVec4ub,int)", d);
  SbVec4ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec4ub operator / (SbVec4ub v, double d) {
  SbDividerChk("operator/(SbVec4ub,double)", d);
  SbVec4ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec4ub operator + (SbVec4ub v1, SbVec4ub v2) {
  SbVec4ub v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec4ub operator - (SbVec4ub v1, SbVec4ub v2) {
  SbVec4ub v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (SbVec4ub v1, SbVec4ub v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]) && (v1[3] == v2[3]));
}

COIN_DLL_API inline int operator != (SbVec4ub v1, SbVec4ub v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC4UB_H
