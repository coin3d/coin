#ifndef COIN_SBVEC3D_H
#define COIN_SBVEC3D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <stdio.h>
#include <Inventor/SbBasic.h>

class SbVec3f;

class COIN_DLL_API SbVec3d {
public:
  SbVec3d(void);
  SbVec3d(const double v[3]);
  SbVec3d(const double x, const double y, const double z);
  SbVec3d(const SbVec3f & v);
  SbVec3d cross(const SbVec3d & v) const;
  double dot(const SbVec3d & v) const;
  SbBool equals(const SbVec3d & v, const double tolerance) const;
  SbVec3d getClosestAxis(void) const;
  const double * getValue(void) const;
  void getValue(double & x, double & y, double & z) const;
  double length(void) const;
  double sqrLength() const;
  void negate(void);
  double normalize(void);
  SbVec3d & setValue(const double v[3]);
  SbVec3d & setValue(const double x, const double y, const double z);
  SbVec3d & setValue(const SbVec3d & barycentric,
                     const SbVec3d & v0,
                     const SbVec3d & v1,
                     const SbVec3d & v2);
  SbVec3d & setValue(const SbVec3f & v);
  double & operator [](const int i);
  const double & operator [](const int i) const;
  SbVec3d & operator *=(const double d);
  SbVec3d & operator /=(const double d);
  SbVec3d & operator +=(const SbVec3d & u);
  SbVec3d & operator -=(const SbVec3d & u);
  SbVec3d operator -(void) const;
  friend COIN_DLL_API SbVec3d operator *(const SbVec3d & v, const double d);
  friend COIN_DLL_API SbVec3d operator *(const double d, const SbVec3d & v);
  friend COIN_DLL_API SbVec3d operator /(const SbVec3d & v, const double d);
  friend COIN_DLL_API SbVec3d operator +(const SbVec3d & v1, const SbVec3d & v2);
  friend COIN_DLL_API SbVec3d operator -(const SbVec3d & v1, const SbVec3d & v2);
  friend COIN_DLL_API int operator ==(const SbVec3d & v1, const SbVec3d & v2);
  friend COIN_DLL_API int operator !=(const SbVec3d & v1, const SbVec3d & v2);

  void print(FILE * fp) const;

private:
  double vec[3];
};

COIN_DLL_API SbVec3d operator *(const SbVec3d & v, const double d);
COIN_DLL_API SbVec3d operator *(const double d, const SbVec3d & v);
COIN_DLL_API SbVec3d operator /(const SbVec3d & v, const double d);
COIN_DLL_API SbVec3d operator +(const SbVec3d & v1, const SbVec3d & v2);
COIN_DLL_API SbVec3d operator -(const SbVec3d & v1, const SbVec3d & v2);
COIN_DLL_API int operator ==(const SbVec3d & v1, const SbVec3d & v2);
COIN_DLL_API int operator !=(const SbVec3d & v1, const SbVec3d & v2);


/* inlined methods ********************************************************/

inline double &
SbVec3d::operator [](const int i)
{
  return this->vec[i];
}

inline const double &
SbVec3d::operator [](const int i) const
{
  return this->vec[i];
}

#endif // !COIN_SBVEC3D_H
