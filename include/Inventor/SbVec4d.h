#ifndef COIN_SBVEC4D_H
#define COIN_SBVEC4D_H

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

class SbVec3d;

class COIN_DLL_API SbVec4d {
public:
  SbVec4d(void);
  SbVec4d(const double v[4]);
  SbVec4d(const double x, const double y, const double z, const double w);
  double dot(const SbVec4d& v) const;
  SbBool equals(const SbVec4d& v, const double tolerance) const;
  void getReal(SbVec3d& v) const;
  const double* getValue(void) const;
  void getValue(double& x, double& y, double& z, double& w) const;
  double length(void) const;
  void negate(void);
  double normalize(void);
  SbVec4d& setValue(const double v[4]);
  SbVec4d& setValue(const double x, const double y, const double z,
                    const double w);
  double& operator [](const int i);
  const double& operator [](const int i) const;
  SbVec4d& operator *=(const double d);
  SbVec4d& operator /=(const double d);
  SbVec4d& operator +=(const SbVec4d& u);
  SbVec4d& operator -=(const SbVec4d& u);
  SbVec4d operator -(void) const;
  friend COIN_DLL_API SbVec4d operator *(const SbVec4d& v, const double d);
  friend COIN_DLL_API SbVec4d operator *(const double d, const SbVec4d& v);
  friend COIN_DLL_API SbVec4d operator /(const SbVec4d& v, const double d);
  friend COIN_DLL_API SbVec4d operator +(const SbVec4d& v1, const SbVec4d& v2);
  friend COIN_DLL_API SbVec4d operator -(const SbVec4d& v1, const SbVec4d& v2);
  friend COIN_DLL_API int operator ==(const SbVec4d& v1, const SbVec4d& v2);
  friend COIN_DLL_API int operator !=(const SbVec4d& v1, const SbVec4d& v2);

  void print(FILE * fp) const;

private:
  double vec[4];
};

COIN_DLL_API SbVec4d operator *(const SbVec4d& v, const double d);
COIN_DLL_API SbVec4d operator *(const double d, const SbVec4d& v);
COIN_DLL_API SbVec4d operator /(const SbVec4d& v, const double d);
COIN_DLL_API SbVec4d operator +(const SbVec4d& v1, const SbVec4d& v2);
COIN_DLL_API SbVec4d operator -(const SbVec4d& v1, const SbVec4d& v2);
COIN_DLL_API int operator ==(const SbVec4d& v1, const SbVec4d& v2);
COIN_DLL_API int operator !=(const SbVec4d& v1, const SbVec4d& v2);

#endif // !COIN_SBVEC4D_H
