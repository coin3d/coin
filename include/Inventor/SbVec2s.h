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

#ifndef COIN_SBVEC2S_H
#define COIN_SBVEC2S_H

#include <stdio.h>

#include <Inventor/system/inttypes.h>

class SbVec2s {
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
  SbVec2s operator -(void) const;
  friend SbVec2s operator *(const SbVec2s& v, int d);
  friend SbVec2s operator *(const SbVec2s& v, double d);
  friend SbVec2s operator *(int d, const SbVec2s& v);
  friend SbVec2s operator *(double d, const SbVec2s& v);
  friend SbVec2s operator /(const SbVec2s& v, int d);
  friend SbVec2s operator /(const SbVec2s& v, double d);
  friend SbVec2s operator +(const SbVec2s& v1, const SbVec2s& v2);
  friend SbVec2s operator -(const SbVec2s& v1, const SbVec2s& v2);
  friend int operator ==(const SbVec2s& v1, const SbVec2s& v2);
  friend int operator !=(const SbVec2s& v1, const SbVec2s& v2);

  void print(FILE * fp) const;

private:
  short vec[2];
};

#endif // !COIN_SBVEC2S_H
