#ifndef COIN_SBVEC2S_H
#define COIN_SBVEC2S_H

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
#include <Inventor/system/inttypes.h>


class COIN_DLL_API SbVec2s {
public:
  SbVec2s(void);
  SbVec2s(const short v[2]);
  SbVec2s(const short x, const short y);
  int32_t dot(const SbVec2s& v) const;
  const short * getValue(void) const;
  void getValue(short& x, short& y) const;
  void negate(void);
  SbVec2s& setValue(const short v[2]);
  SbVec2s& setValue(short x, short y);
  short& operator [](const int i);
  const short& operator [](const int i) const;
  SbVec2s& operator *=(int d);
  SbVec2s& operator *=(double d);
  SbVec2s& operator /=(int d);
  SbVec2s& operator /=(double d);
  SbVec2s& operator +=(const SbVec2s& u);
  SbVec2s& operator -=(const SbVec2s& u);
  SbVec2s operator-(void) const;
  friend COIN_DLL_API SbVec2s operator *(const SbVec2s& v, int d);
  friend COIN_DLL_API SbVec2s operator *(const SbVec2s& v, double d);
  friend COIN_DLL_API SbVec2s operator *(int d, const SbVec2s& v);
  friend COIN_DLL_API SbVec2s operator *(double d, const SbVec2s& v);
  friend COIN_DLL_API SbVec2s operator /(const SbVec2s& v, int d);
  friend COIN_DLL_API SbVec2s operator /(const SbVec2s& v, double d);
  friend COIN_DLL_API SbVec2s operator +(const SbVec2s& v1, const SbVec2s& v2);
  friend COIN_DLL_API SbVec2s operator -(const SbVec2s& v1, const SbVec2s& v2);
  friend COIN_DLL_API int operator ==(const SbVec2s& v1, const SbVec2s& v2);
  friend COIN_DLL_API int operator !=(const SbVec2s& v1, const SbVec2s& v2);

  void print(FILE * fp) const;

private:
  short vec[2];
};

COIN_DLL_API SbVec2s operator *(const SbVec2s& v, int d);
COIN_DLL_API SbVec2s operator *(const SbVec2s& v, double d);
COIN_DLL_API SbVec2s operator *(int d, const SbVec2s& v);
COIN_DLL_API SbVec2s operator *(double d, const SbVec2s& v);
COIN_DLL_API SbVec2s operator /(const SbVec2s& v, int d);
COIN_DLL_API SbVec2s operator /(const SbVec2s& v, double d);
COIN_DLL_API SbVec2s operator +(const SbVec2s& v1, const SbVec2s& v2);
COIN_DLL_API SbVec2s operator -(const SbVec2s& v1, const SbVec2s& v2);
COIN_DLL_API int operator ==(const SbVec2s& v1, const SbVec2s& v2);
COIN_DLL_API int operator !=(const SbVec2s& v1, const SbVec2s& v2);

#endif // !COIN_SBVEC2S_H
