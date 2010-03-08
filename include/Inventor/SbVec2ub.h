#ifndef COIN_SBVEC2UB_H
#define COIN_SBVEC2UB_H

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

class SbVec2b;
class SbVec2us;
class SbVec2ui32;

class COIN_DLL_API SbVec2ub {
public:
  SbVec2ub(void) { }
  SbVec2ub(const uint8_t v[2]) { vec[0] = v[0]; vec[1] = v[1]; }
  SbVec2ub(uint8_t x, uint8_t y) { vec[0] = x; vec[1] = y; }
  explicit SbVec2ub(const SbVec2b & v) { setValue(v); }
  explicit SbVec2ub(const SbVec2us & v) { setValue(v); }
  explicit SbVec2ub(const SbVec2ui32 & v) { setValue(v); }

  SbVec2ub & setValue(const uint8_t v[2]) { vec[0] = v[0]; vec[1] = v[1]; return *this; }
  SbVec2ub & setValue(uint8_t x, uint8_t y) { vec[0] = x; vec[1] = y; return *this; }
  SbVec2ub & setValue(const SbVec2b & v);
  SbVec2ub & setValue(const SbVec2us & v);
  SbVec2ub & setValue(const SbVec2ui32 & v);

  const uint8_t * getValue(void) const { return vec; }
  void getValue(uint8_t & x, uint8_t & y) const { x = vec[0]; y = vec[1]; }

  uint8_t & operator [] (int i) { return vec[i]; }
  const uint8_t & operator [] (int i) const { return vec[i]; }

  int32_t dot(SbVec2ub v) const { return vec[0] * v[0] + vec[1] * v[1]; }
  void negate(void);

  SbVec2ub & operator *= (int d) { vec[0] *= d; vec[1] *= d; return *this; }
  SbVec2ub & operator *= (double d);
  SbVec2ub & operator /= (int d) { SbDividerChk("SbVec2ub::operator/=(int)", d); vec[0] /= d; vec[1] /= d; return *this; }
  SbVec2ub & operator /= (double d) { SbDividerChk("SbVec2ub::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec2ub & operator += (SbVec2ub v) { vec[0] += v[0]; vec[1] += v[1]; return *this; }
  SbVec2ub & operator -= (SbVec2ub v) { vec[0] -= v[0]; vec[1] -= v[1]; return *this; }
  SbVec2ub operator - (void) const { SbVec2ub v(*this); v.negate(); return v; }

protected:
  uint8_t vec[2];

}; // SbVec2ub

COIN_DLL_API inline SbVec2ub operator * (SbVec2ub v, int d) {
  SbVec2ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2ub operator * (SbVec2ub v, double d) {
  SbVec2ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2ub operator * (int d, SbVec2ub v) {
  SbVec2ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2ub operator * (double d, SbVec2ub v) {
  SbVec2ub val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2ub operator / (SbVec2ub v, int d) {
  SbDividerChk("operator/(SbVec2ub,int)", d);
  SbVec2ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2ub operator / (SbVec2ub v, double d) {
  SbDividerChk("operator/(SbVec2ub,double)", d);
  SbVec2ub val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2ub operator + (SbVec2ub v1, SbVec2ub v2) {
  SbVec2ub v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec2ub operator - (SbVec2ub v1, SbVec2ub v2) {
  SbVec2ub v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (SbVec2ub v1, SbVec2ub v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

COIN_DLL_API inline int operator != (SbVec2ub v1, SbVec2ub v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC2UB_H
