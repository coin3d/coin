/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBVEC3F_H
#define COIN_SBVEC3F_H

#include <stdio.h>

#include <Inventor/SbBasic.h>

class SbPlane;

class COIN_DLL_EXPORT SbVec3f {
public:
  SbVec3f(void);
  SbVec3f(const float v[3]);
  SbVec3f(const float x, const float y, const float z);
  SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);
  SbVec3f cross(const SbVec3f & v) const;
  float dot(const SbVec3f & v) const;
  SbBool equals(const SbVec3f & v, const float tolerance) const;
  SbVec3f getClosestAxis(void) const;
  const float * getValue(void) const;
  void getValue(float & x, float & y, float & z) const;
  float length(void) const;
  float sqrLength() const;
  void negate(void);
  float normalize(void);
  SbVec3f & setValue(const float v[3]);
  SbVec3f & setValue(const float x, const float y, const float z);
  SbVec3f & setValue(const SbVec3f & barycentic,
                     const SbVec3f & v0,
                     const SbVec3f & v1,
                     const SbVec3f & v2);
  float & operator [](const int i);
  const float & operator [](const int i) const;
  SbVec3f & operator *=(const float d);
  SbVec3f & operator /=(const float d);
  SbVec3f & operator +=(const SbVec3f & u);
  SbVec3f & operator -=(const SbVec3f & u);
  SbVec3f operator -(void) const;
  friend COIN_DLL_EXPORT SbVec3f operator *(const SbVec3f & v, const float d);
  friend COIN_DLL_EXPORT SbVec3f operator *(const float d, const SbVec3f & v);
  friend COIN_DLL_EXPORT SbVec3f operator /(const SbVec3f & v, const float d);
  friend COIN_DLL_EXPORT SbVec3f operator +(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_EXPORT SbVec3f operator -(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_EXPORT int operator ==(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_EXPORT int operator !=(const SbVec3f & v1, const SbVec3f & v2);

  void print(FILE * fp) const;

private:
  float vec[3];
};

COIN_DLL_EXPORT SbVec3f operator *(const SbVec3f & v, const float d);
COIN_DLL_EXPORT SbVec3f operator *(const float d, const SbVec3f & v);
COIN_DLL_EXPORT SbVec3f operator /(const SbVec3f & v, const float d);
COIN_DLL_EXPORT SbVec3f operator +(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_EXPORT SbVec3f operator -(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_EXPORT int operator ==(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_EXPORT int operator !=(const SbVec3f & v1, const SbVec3f & v2);

#endif // !COIN_SBVEC3F_H
