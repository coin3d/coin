#ifndef COIN_SBVEC2D_H
#define COIN_SBVEC2D_H

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

class COIN_DLL_API SbVec2d {
public:
  SbVec2d(void);
  SbVec2d(const double v[2]);
  SbVec2d(const double x, const double y);
  double dot(const SbVec2d & v) const;
  SbBool equals(const SbVec2d & v, const double tolerance) const;
  const double * getValue(void) const;
  void getValue(double & x, double & y) const;
  double length(void) const;
  void negate(void);
  double normalize(void);
  SbVec2d & setValue(const double v[2]);
  SbVec2d & setValue(const double x, const double y);
  double & operator [] (const int i);
  const double & operator [] (const int i) const;
  SbVec2d & operator *= (const double d);
  SbVec2d & operator /= (const double d);
  SbVec2d & operator += (const SbVec2d & u);
  SbVec2d & operator -= (const SbVec2d & u);
  SbVec2d operator -(void) const;

  friend COIN_DLL_API SbVec2d operator * (const SbVec2d & v, const double d);
  friend COIN_DLL_API SbVec2d operator * (const double d, const SbVec2d & v);
  friend COIN_DLL_API SbVec2d operator / (const SbVec2d & v, const double d);
  friend COIN_DLL_API SbVec2d operator + (const SbVec2d & v1, const SbVec2d & v2);
  friend COIN_DLL_API SbVec2d operator - (const SbVec2d & v1, const SbVec2d & v2);
  friend COIN_DLL_API int operator == (const SbVec2d & v1, const SbVec2d & v2);
  friend COIN_DLL_API int operator != (const SbVec2d & v1, const SbVec2d & v2);

  void print(FILE * fp) const;

private:
  double vec[2];
};

COIN_DLL_API SbVec2d operator * (const SbVec2d & v, const double d);
COIN_DLL_API SbVec2d operator * (const double d, const SbVec2d & v);
COIN_DLL_API SbVec2d operator / (const SbVec2d & v, const double d);
COIN_DLL_API SbVec2d operator + (const SbVec2d & v1, const SbVec2d & v2);
COIN_DLL_API SbVec2d operator - (const SbVec2d & v1, const SbVec2d & v2);
COIN_DLL_API int operator == (const SbVec2d & v1, const SbVec2d & v2);
COIN_DLL_API int operator != (const SbVec2d & v1, const SbVec2d & v2);


// *************************************************************************

//$ IMPORT INLINE ../../src/SbVec2d.cpp

// *************************************************************************

#endif // !COIN_SBVEC2D_H
