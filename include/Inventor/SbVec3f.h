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

#ifndef __SBVEC3F_H__
#define __SBVEC3F_H__

#include <Inventor/SbBasic.h>

class SbPlane;
class ostream;


class SbVec3f
{ 
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
  friend SbVec3f operator *(const SbVec3f & v, const float d);
  friend SbVec3f operator *(const float d, const SbVec3f & v);
  friend SbVec3f operator /(const SbVec3f & v, const float d);
  friend SbVec3f operator +(const SbVec3f & v1, const SbVec3f & v2);
  friend SbVec3f operator -(const SbVec3f & v1, const SbVec3f & v2);
  friend int operator ==(const SbVec3f & v1, const SbVec3f & v2);
  friend int operator !=(const SbVec3f & v1, const SbVec3f & v2);

  void print(ostream & file) const;

private:
  float vec[3];
};

#endif // !__SBVEC3F_H__
