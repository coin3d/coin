#ifndef COIN_SBVEC3S_H
#define COIN_SBVEC3S_H

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

class SbVec3us;
class SbVec3b;
class SbVec3i32;
class SbVec3f;
class SbVec3d;

class COIN_DLL_API SbVec3s {
public:
  SbVec3s(void) { }
  SbVec3s(const short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3s(short x, short y, short z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3s(const SbVec3us & v) { setValue(v); }
  explicit SbVec3s(const SbVec3b & v) { setValue(v); }
  explicit SbVec3s(const SbVec3i32 & v) { setValue(v); }
  explicit SbVec3s(const SbVec3f & v) { setValue(v); }
  explicit SbVec3s(const SbVec3d & v) { setValue(v); }

  SbVec3s & setValue(const short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3s & setValue(short x, short y, short z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3s & setValue(const SbVec3us & v);
  SbVec3s & setValue(const SbVec3b & v);
  SbVec3s & setValue(const SbVec3i32 & v);
  SbVec3s & setValue(const SbVec3f & v);
  SbVec3s & setValue(const SbVec3d & v);

  const short * getValue(void) const { return vec; }
  void getValue(short & x, short & y, short & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  short & operator [] (int i) { return vec[i]; }
  const short & operator [] (int i) const { return vec[i]; }

  int32_t dot(const SbVec3s & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; vec[2] = -vec[2]; }

  SbVec3s & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3s & operator *= (double  d);
  SbVec3s & operator /= (int d) { SbDividerChk("SbVec3s::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; return *this; }
  SbVec3s & operator /= (double d) { SbDividerChk("SbVec3s::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec3s & operator += (const SbVec3s & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3s & operator -= (const SbVec3s & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3s operator - (void) const { return SbVec3s(-vec[0], -vec[1], -vec[2]); }

  SbString toString() const;
  SbBool fromString(const SbString & str);
  void print(FILE * fp) const;

protected:
  short vec[3];

}; // SbVec3s

COIN_DLL_API inline SbVec3s operator * (const SbVec3s & v, int d) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (const SbVec3s & v, double d) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (int d, const SbVec3s & v) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (double d, const SbVec3s & v) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator / (const SbVec3s & v, int d) {
  SbDividerChk("operator/(SbVec3s,int)", d);
  SbVec3s val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3s operator / (const SbVec3s & v, double d) {
  SbDividerChk("operator/(SbVec3s,double)", d);
  SbVec3s val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3s operator + (const SbVec3s & v1, const SbVec3s & v2) {
  SbVec3s v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3s operator - (const SbVec3s & v1, const SbVec3s & v2) {
  SbVec3s v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec3s & v1, const SbVec3s & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (const SbVec3s & v1, const SbVec3s & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3S_H
