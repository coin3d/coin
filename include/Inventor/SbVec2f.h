/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SBVEC2F_H
#define COIN_SBVEC2F_H

#include <stdio.h>

#include <Inventor/SbBasic.h>

class COIN_DLL_EXPORT SbVec2f {
public:
  SbVec2f(void);
  SbVec2f(const float v[2]);
  SbVec2f(const float x, const float y);
  float dot(const SbVec2f & v) const;
  SbBool equals(const SbVec2f & v, const float tolerance) const;
  const float * getValue(void) const;
  void getValue(float & x, float & y) const;
  float length(void) const;
  void negate(void);
  float normalize(void);
  SbVec2f & setValue(const float v[2]);
  SbVec2f & setValue(const float x, const float y);
  float & operator [] (const int i);
  const float & operator [] (const int i) const;
  SbVec2f & operator *= (const float d);
  SbVec2f & operator /= (const float d);
  SbVec2f & operator += (const SbVec2f & u);
  SbVec2f & operator -= (const SbVec2f & u);
  SbVec2f operator -(void) const;

  friend COIN_DLL_EXPORT SbVec2f operator * (const SbVec2f & v, const float d);
  friend COIN_DLL_EXPORT SbVec2f operator * (const float d, const SbVec2f & v);
  friend COIN_DLL_EXPORT SbVec2f operator / (const SbVec2f & v, const float d);
  friend COIN_DLL_EXPORT SbVec2f operator + (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_EXPORT SbVec2f operator - (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_EXPORT int operator == (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_EXPORT int operator != (const SbVec2f & v1, const SbVec2f & v2);

  void print(FILE * fp) const;

private:
  float vec[2];
};

COIN_DLL_EXPORT SbVec2f operator * (const SbVec2f & v, const float d);
COIN_DLL_EXPORT SbVec2f operator * (const float d, const SbVec2f & v);
COIN_DLL_EXPORT SbVec2f operator / (const SbVec2f & v, const float d);
COIN_DLL_EXPORT SbVec2f operator + (const SbVec2f & v1, const SbVec2f & v2);
COIN_DLL_EXPORT SbVec2f operator - (const SbVec2f & v1, const SbVec2f & v2);
COIN_DLL_EXPORT int operator == (const SbVec2f & v1, const SbVec2f & v2);
COIN_DLL_EXPORT int operator != (const SbVec2f & v1, const SbVec2f & v2);


// *************************************************************************

//$ IMPORT INLINE ../../src/SbVec2f.cpp

// *************************************************************************

#endif // !COIN_SBVEC2F_H
