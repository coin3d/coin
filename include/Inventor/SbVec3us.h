#ifndef COIN_SBVEC3US_H
#define COIN_SBVEC3US_H

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

#include <cstdio>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>
#include <Inventor/SbByteBuffer.h>
#include <Inventor/SbString.h>

#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec3s;
class SbVec3ub;
class SbVec3ui32;

class COIN_DLL_API SbVec3us {
public:
  SbVec3us(void) { }
  SbVec3us(const unsigned short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3us(unsigned short x, unsigned short y, unsigned short z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3us(const SbVec3s & v) { setValue(v); }
  explicit SbVec3us(const SbVec3ub & v) { setValue(v); }
  explicit SbVec3us(const SbVec3ui32 & v) { setValue(v); }

  SbVec3us & setValue(const unsigned short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3us & setValue(unsigned short x, unsigned short y, unsigned short z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3us & setValue(const SbVec3s & v);
  SbVec3us & setValue(const SbVec3ub & v);
  SbVec3us & setValue(const SbVec3ui32 & v);

  const unsigned short * getValue(void) const { return vec; }
  void getValue(unsigned short & x, unsigned short & y, unsigned short & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  unsigned short & operator [] (int i) { return vec[i]; }
  const unsigned short & operator [] (int i) const { return vec[i]; }

  int32_t dot(const SbVec3us & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  void negate(void);

  SbVec3us & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3us & operator *= (double d);
  SbVec3us & operator /= (int d) { SbDividerChk("SbVec3us::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; return *this; }
  SbVec3us & operator /= (double d) { SbDividerChk("SbVec3us::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec3us & operator += (const SbVec3us & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3us & operator -= (const SbVec3us & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3us operator - (void) const { SbVec3us v(*this); v.negate(); return v; }

protected:
  unsigned short vec[3];

}; // SbVec3us

COIN_DLL_API inline SbVec3us operator * (const SbVec3us & v, int d) {
  SbVec3us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3us operator * (const SbVec3us & v, double d) {
  SbVec3us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3us operator * (int d, const SbVec3us & v) {
  SbVec3us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3us operator * (double d, const SbVec3us & v) {
  SbVec3us val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3us operator / (const SbVec3us & v, int d) {
  SbDividerChk("operator/(SbVec3us,int)", d);
  SbVec3us val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3us operator / (const SbVec3us & v, double d) {
  SbDividerChk("operator/(SbVec3us,double)", d);
  SbVec3us val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3us operator + (const SbVec3us & v1, const SbVec3us & v2) {
  SbVec3us v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3us operator - (const SbVec3us & v1, const SbVec3us & v2) {
  SbVec3us v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec3us & v1, const SbVec3us & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (const SbVec3us & v1, const SbVec3us & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3US_H
