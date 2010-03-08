#ifndef COIN_SBVEC2US_H
#define COIN_SBVEC2US_H

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

class SbVec2s;
class SbVec2ub;
class SbVec2ui32;

class COIN_DLL_API SbVec2us {
public:
  SbVec2us(void) { }
  SbVec2us(const unsigned short v[2]) { vec[0] = v[0]; vec[1] = v[1]; }
  SbVec2us(unsigned short x, unsigned short y) { vec[0] = x; vec[1] = y; }
  explicit SbVec2us(const SbVec2s & v) { setValue(v); }
  explicit SbVec2us(const SbVec2ub & v) { setValue(v); }
  explicit SbVec2us(const SbVec2ui32 & v) { setValue(v); }

  SbVec2us & setValue(const unsigned short v[2]) { vec[0] = v[0]; vec[1] = v[1]; return *this; }
  SbVec2us & setValue(unsigned short x, unsigned short y) { vec[0] = x; vec[1] = y; return *this; }
  SbVec2us & setValue(const SbVec2s & v);
  SbVec2us & setValue(const SbVec2ub & v);
  SbVec2us & setValue(const SbVec2ui32 & v);

  const unsigned short * getValue(void) const { return vec; }
  void getValue(unsigned short & x, unsigned short & y) const { x = vec[0]; y = vec[1]; }

  unsigned short & operator [] (int i) { return vec[i]; }
  const unsigned short & operator [] (int i) const { return vec[i]; }

  int32_t dot(SbVec2us v) const { return vec[0] * v[0] + vec[1] * v[1]; }
  void negate(void);

  SbVec2us & operator *= (int d) { vec[0] *= d; vec[1] *= d; return *this; }
  SbVec2us & operator *= (double d);
  SbVec2us & operator /= (int d) { SbDividerChk("SbVec2us::operator/=(int)", d); vec[0] /= d; vec[1] /= d; return *this; }
  SbVec2us & operator /= (double d) { SbDividerChk("SbVec2us::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec2us & operator += (SbVec2us v) { vec[0] += v[0]; vec[1] += v[1]; return *this; }
  SbVec2us & operator -= (SbVec2us v) { vec[0] -= v[0]; vec[1] -= v[1]; return *this; }
  SbVec2us operator - (void) const { SbVec2us v(*this); v.negate(); return v; }

protected:
  unsigned short vec[2];

}; // SbVec2us

COIN_DLL_API inline SbVec2us operator * (SbVec2us v, int d) {
  SbVec2us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2us operator * (SbVec2us v, double d) {
  SbVec2us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2us operator * (int d, SbVec2us v) {
  SbVec2us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2us operator * (double d, SbVec2us v) {
  SbVec2us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2us operator / (SbVec2us v, int d) {
  SbDividerChk("operator/(SbVec2us,int)", d);
  SbVec2us val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2us operator / (SbVec2us v, double d) {
  SbDividerChk("operator/(SbVec2us,double)", d);
  SbVec2us val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2us operator + (SbVec2us v1, SbVec2us v2) {
  SbVec2us v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec2us operator - (SbVec2us v1, SbVec2us v2) {
  SbVec2us v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (SbVec2us v1, SbVec2us v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

COIN_DLL_API inline int operator != (SbVec2us v1, SbVec2us v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC2US_H
