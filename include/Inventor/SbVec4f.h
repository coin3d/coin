#ifndef COIN_SBVEC4F_H
#define COIN_SBVEC4F_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <stdio.h>

#include <Inventor/SbBasic.h>

class SbVec3f;

class COIN_DLL_API SbVec4f {
public:
  SbVec4f(void);
  SbVec4f(const float v[4]);
  SbVec4f(const float x, const float y, const float z, const float w);
  float dot(const SbVec4f& v) const;
  SbBool equals(const SbVec4f& v, const float tolerance) const;
  void getReal(SbVec3f& v) const;
  const float* getValue(void) const;
  void getValue(float& x, float& y, float& z, float& w) const;
  float length(void) const;
  void negate(void);
  float normalize(void);
  SbVec4f& setValue(const float v[4]);
  SbVec4f& setValue(const float x, const float y, const float z,
                    const float w);
  float& operator [](const int i);
  const float& operator [](const int i) const;
  SbVec4f& operator *=(const float d);
  SbVec4f& operator /=(const float d);
  SbVec4f& operator +=(const SbVec4f& u);
  SbVec4f& operator -=(const SbVec4f& u);
  SbVec4f operator-(void) const;
  friend COIN_DLL_API SbVec4f operator *(const SbVec4f& v, const float d);
  friend COIN_DLL_API SbVec4f operator *(const float d, const SbVec4f& v);
  friend COIN_DLL_API SbVec4f operator /(const SbVec4f& v, const float d);
  friend COIN_DLL_API SbVec4f operator +(const SbVec4f& v1, const SbVec4f& v2);
  friend COIN_DLL_API SbVec4f operator -(const SbVec4f& v1, const SbVec4f& v2);
  friend COIN_DLL_API int operator ==(const SbVec4f& v1, const SbVec4f& v2);
  friend COIN_DLL_API int operator !=(const SbVec4f& v1, const SbVec4f& v2);

  void print(FILE * fp) const;

private:
  float vec[4];
};

COIN_DLL_API SbVec4f operator *(const SbVec4f& v, const float d);
COIN_DLL_API SbVec4f operator *(const float d, const SbVec4f& v);
COIN_DLL_API SbVec4f operator /(const SbVec4f& v, const float d);
COIN_DLL_API SbVec4f operator +(const SbVec4f& v1, const SbVec4f& v2);
COIN_DLL_API SbVec4f operator -(const SbVec4f& v1, const SbVec4f& v2);
COIN_DLL_API int operator ==(const SbVec4f& v1, const SbVec4f& v2);
COIN_DLL_API int operator !=(const SbVec4f& v1, const SbVec4f& v2);

#endif // !COIN_SBVEC4F_H
