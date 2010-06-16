#ifndef COIN_SBVEC2D_H
#define COIN_SBVEC2D_H

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
#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec2f;
class SbVec2b;
class SbVec2s;
class SbVec2i32;

class COIN_DLL_API SbVec2d {
public:
  SbVec2d(void) { }
  SbVec2d(const double v[2]) { vec[0] = v[0]; vec[1] = v[1]; }
  SbVec2d(double x, double y) { vec[0] = x; vec[1] = y; }
  explicit SbVec2d(const SbVec2f & v) { setValue(v); }
  explicit SbVec2d(const SbVec2b & v) { setValue(v); }
  explicit SbVec2d(const SbVec2s & v) { setValue(v); }
  explicit SbVec2d(const SbVec2i32 & v) { setValue(v); }

  SbVec2d & setValue(const double v[2]) { vec[0] = v[0]; vec[1] = v[1]; return *this; }
  SbVec2d & setValue(double x, double y) { vec[0] = x; vec[1] = y; return *this; }
  SbVec2d & setValue(const SbVec2f & v);
  SbVec2d & setValue(const SbVec2b & v);
  SbVec2d & setValue(const SbVec2s & v);
  SbVec2d & setValue(const SbVec2i32 & v);

  const double * getValue(void) const { return vec; }
  void getValue(double & x, double & y) const { x = vec[0]; y = vec[1]; }

  double & operator [] (int i) { return vec[i]; } 
  const double & operator [] (int i) const { return vec[i]; }

  SbBool equals(const SbVec2d & v, double tolerance) const;
  double dot(const SbVec2d & v) const { return vec[0] * v[0] + vec[1] * v[1]; }
  double length(void) const;
  double sqrLength(void) const { return vec[0]*vec[0] + vec[1]*vec[1]; }
  double normalize(void);
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; }

  SbVec2d & operator *= (double d) { vec[0] *= d; vec[1] *= d; return *this; }
  SbVec2d & operator /= (double d) { SbDividerChk("SbVec2d::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec2d & operator += (const SbVec2d & v) { vec[0] += v[0]; vec[1] += v[1]; return *this; }
  SbVec2d & operator -= (const SbVec2d & v) { vec[0] -= v[0]; vec[1] -= v[1]; return *this; }
  SbVec2d operator - (void) const { return SbVec2d(-vec[0], -vec[1]); }

  void print(FILE * fp) const;

protected:
  double vec[2];

}; // SbVec2d

COIN_DLL_API inline SbVec2d operator * (const SbVec2d & v, const double d) {
  SbVec2d val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2d operator * (const double d, const SbVec2d & v) {
  SbVec2d val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec2d operator / (const SbVec2d & v, const double d) {
  SbDividerChk("operator/(SbVec2d,double)", d);
  SbVec2d val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec2d operator + (const SbVec2d & v1, const SbVec2d & v2) {
  SbVec2d v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec2d operator - (const SbVec2d & v1, const SbVec2d & v2) {
  SbVec2d v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec2d & v1, const SbVec2d & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]));
}

COIN_DLL_API inline int operator != (const SbVec2d & v1, const SbVec2d & v2) {
  return !(v1 == v2);
}

// *************************************************************************

#endif // !COIN_SBVEC2D_H
