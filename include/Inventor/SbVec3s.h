#ifndef COIN_SBVEC3S_H
#define COIN_SBVEC3S_H

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

class COIN_DLL_API SbVec3s {
public:
  SbVec3s(void);
  SbVec3s(const short v[3]);
  SbVec3s(const short x, const short y, const short z);

  int32_t dot(const SbVec3s & v) const;
  const short * getValue(void) const;
  void getValue(short & x, short & y, short & z) const;
  void negate(void);
  SbVec3s & setValue(const short v[3]);
  SbVec3s & setValue(const short x, const short y, const short z);

  short & operator [](const int i);
  const short & operator [](const int i) const;
  SbVec3s & operator *=(int d);
  SbVec3s & operator *=(double  d);
  SbVec3s & operator /=(int d);
  SbVec3s & operator /=(double d);
  SbVec3s & operator +=(const SbVec3s & u);
  SbVec3s & operator -=(const SbVec3s & u);
  SbVec3s operator-(void) const;
  friend COIN_DLL_API SbVec3s operator *(const SbVec3s & v, int d);
  friend COIN_DLL_API SbVec3s operator *(const SbVec3s & v, double d);
  friend COIN_DLL_API SbVec3s operator *(int d, const SbVec3s & v);
  friend COIN_DLL_API SbVec3s operator *(double d, const SbVec3s & v);
  friend COIN_DLL_API SbVec3s operator /(const SbVec3s & v, int d);
  friend COIN_DLL_API SbVec3s operator /(const SbVec3s & v, double d);
  friend COIN_DLL_API SbVec3s operator +(const SbVec3s & v1, const SbVec3s & v2);
  friend COIN_DLL_API SbVec3s operator -(const SbVec3s & v1, const SbVec3s & v2);
  friend COIN_DLL_API int operator ==(const SbVec3s & v1, const SbVec3s & v2);
  friend COIN_DLL_API int operator !=(const SbVec3s & v1, const SbVec3s & v2);

  void print(FILE * fp) const;

private:
  short vec[3];
};

COIN_DLL_API SbVec3s operator *(const SbVec3s & v, int d);
COIN_DLL_API SbVec3s operator *(const SbVec3s & v, double d);
COIN_DLL_API SbVec3s operator *(int d, const SbVec3s & v);
COIN_DLL_API SbVec3s operator *(double d, const SbVec3s & v);
COIN_DLL_API SbVec3s operator /(const SbVec3s & v, int d);
COIN_DLL_API SbVec3s operator /(const SbVec3s & v, double d);
COIN_DLL_API SbVec3s operator +(const SbVec3s & v1, const SbVec3s & v2);
COIN_DLL_API SbVec3s operator -(const SbVec3s & v1, const SbVec3s & v2);
COIN_DLL_API int operator ==(const SbVec3s & v1, const SbVec3s & v2);
COIN_DLL_API int operator !=(const SbVec3s & v1, const SbVec3s & v2);

/* inlined methods ********************************************************/

inline short &
SbVec3s::operator [](const int i)
{
  return this->vec[i];
}

inline const short &
SbVec3s::operator [](const int i) const
{
  return this->vec[i];
}

#endif // !COIN_SBVEC3S_H
