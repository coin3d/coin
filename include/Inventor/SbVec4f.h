/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SBVEC4F_H__
#define __SBVEC4F_H__

#include <Inventor/SbBasic.h>

class SbVec3f;
class ostream;


class SbVec4f {
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
  SbVec4f operator -(void) const;
  friend SbVec4f operator *(const SbVec4f& v, const float d);
  friend SbVec4f operator *(const float d, const SbVec4f& v);
  friend SbVec4f operator /(const SbVec4f& v, const float d);
  friend SbVec4f operator +(const SbVec4f& v1, const SbVec4f& v2);
  friend SbVec4f operator -(const SbVec4f& v1, const SbVec4f& v2);
  friend int operator ==(const SbVec4f& v1, const SbVec4f& v2);
  friend int operator !=(const SbVec4f& v1, const SbVec4f& v2);

  void print(ostream & file) const;

private:
  float vec[4];
};

#endif // !__SBVEC4F_H__
