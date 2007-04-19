#ifndef COIN_SBVEC2I32_H
#define COIN_SBVEC2I32_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <stdio.h>
#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class COIN_DLL_API SbVec2i32 {
public:
  SbVec2i32(void);
  SbVec2i32(const int32_t v[2]);
  SbVec2i32(const int32_t x, const int32_t y);
  int32_t dot(const SbVec2i32& v) const;
  const int32_t * getValue(void) const;
  void getValue(int32_t & x, int32_t & y) const;
  void negate(void);
  SbVec2i32& setValue(const int32_t v[2]);
  SbVec2i32& setValue(int32_t x, int32_t y);
  int32_t & operator [](const int i);
  const int32_t & operator [](const int i) const;
  SbVec2i32& operator *=(int d);
  SbVec2i32& operator *=(double d);
  SbVec2i32& operator /=(int d);
  SbVec2i32& operator /=(double d);
  SbVec2i32& operator +=(const SbVec2i32& u);
  SbVec2i32& operator -=(const SbVec2i32& u);
  SbVec2i32 operator-(void) const;
  friend COIN_DLL_API SbVec2i32 operator *(const SbVec2i32& v, int d);
  friend COIN_DLL_API SbVec2i32 operator *(const SbVec2i32& v, double d);
  friend COIN_DLL_API SbVec2i32 operator *(int d, const SbVec2i32& v);
  friend COIN_DLL_API SbVec2i32 operator *(double d, const SbVec2i32& v);
  friend COIN_DLL_API SbVec2i32 operator /(const SbVec2i32& v, int d);
  friend COIN_DLL_API SbVec2i32 operator /(const SbVec2i32& v, double d);
  friend COIN_DLL_API SbVec2i32 operator +(const SbVec2i32& v1, const SbVec2i32& v2);
  friend COIN_DLL_API SbVec2i32 operator -(const SbVec2i32& v1, const SbVec2i32& v2);
  friend COIN_DLL_API int operator ==(const SbVec2i32& v1, const SbVec2i32& v2);
  friend COIN_DLL_API int operator !=(const SbVec2i32& v1, const SbVec2i32& v2);

  void print(FILE * fp) const;

protected:
  int32_t vec[2];
};

COIN_DLL_API SbVec2i32 operator *(const SbVec2i32& v, int d);
COIN_DLL_API SbVec2i32 operator *(const SbVec2i32& v, double d);
COIN_DLL_API SbVec2i32 operator *(int d, const SbVec2i32& v);
COIN_DLL_API SbVec2i32 operator *(double d, const SbVec2i32& v);
COIN_DLL_API SbVec2i32 operator /(const SbVec2i32& v, int d);
COIN_DLL_API SbVec2i32 operator /(const SbVec2i32& v, double d);
COIN_DLL_API SbVec2i32 operator +(const SbVec2i32& v1, const SbVec2i32& v2);
COIN_DLL_API SbVec2i32 operator -(const SbVec2i32& v1, const SbVec2i32& v2);
COIN_DLL_API int operator ==(const SbVec2i32& v1, const SbVec2i32& v2);
COIN_DLL_API int operator !=(const SbVec2i32& v1, const SbVec2i32& v2);

#endif // !COIN_SBVEC2I32_H
