#ifndef COIN_SBVEC3F_H
#define COIN_SBVEC3F_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbBasic.h>

class SbPlane;
class SbVec3d;

class COIN_DLL_API SbVec3f {
public:
  SbVec3f(void) { }
  SbVec3f(const float v[3]);
  SbVec3f(const float x, const float y, const float z);
  SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);
  SbVec3f(const SbVec3d & v);
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
  SbVec3f & setValue(const SbVec3f & barycentric,
                     const SbVec3f & v0,
                     const SbVec3f & v1,
                     const SbVec3f & v2);
  SbVec3f & setValue(const SbVec3d & v);
  float & operator [](const int i);
  const float & operator [](const int i) const;
  SbVec3f & operator *=(const float d);
  SbVec3f & operator /=(const float d);
  SbVec3f & operator +=(const SbVec3f & u);
  SbVec3f & operator -=(const SbVec3f & u);
  SbVec3f operator-(void) const;
  friend COIN_DLL_API SbVec3f operator *(const SbVec3f & v, const float d);
  friend COIN_DLL_API SbVec3f operator *(const float d, const SbVec3f & v);
  friend COIN_DLL_API SbVec3f operator /(const SbVec3f & v, const float d);
  friend COIN_DLL_API SbVec3f operator +(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_API SbVec3f operator -(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_API int operator ==(const SbVec3f & v1, const SbVec3f & v2);
  friend COIN_DLL_API int operator !=(const SbVec3f & v1, const SbVec3f & v2);

  void print(FILE * fp) const;

private:
  float vec[3];
};

COIN_DLL_API SbVec3f operator *(const SbVec3f & v, const float d);
COIN_DLL_API SbVec3f operator *(const float d, const SbVec3f & v);
COIN_DLL_API SbVec3f operator /(const SbVec3f & v, const float d);
COIN_DLL_API SbVec3f operator +(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_API SbVec3f operator -(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_API int operator ==(const SbVec3f & v1, const SbVec3f & v2);
COIN_DLL_API int operator !=(const SbVec3f & v1, const SbVec3f & v2);


/* inlined methods ********************************************************/

inline float &
SbVec3f::operator [](const int i)
{
  return this->vec[i];
}

inline const float &
SbVec3f::operator [](const int i) const
{
  return this->vec[i];
}

#endif // !COIN_SBVEC3F_H
