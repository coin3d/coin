#ifndef COIN_SBMATRIX_H
#define COIN_SBMATRIX_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <stdio.h>
#include <Inventor/SbBasic.h>

class SbLine;
class SbRotation;
class SbVec3f;
class SbVec4f;

typedef float SbMat[4][4];

class COIN_DLL_API SbMatrix {
public:
  SbMatrix(void);
  SbMatrix(const float a11, const float a12, const float a13, const float a14,
           const float a21, const float a22, const float a23, const float a24,
           const float a31, const float a32, const float a33, const float a34,
           const float a41, const float a42, const float a43, const float a44);
  SbMatrix(const SbMat & matrix);
  SbMatrix(const SbMat * matrix);
  ~SbMatrix(void);

  SbMatrix & operator =(const SbMat & m);

  operator float*(void);
  SbMatrix & operator =(const SbMatrix & m);
  void setValue(const SbMat & m);
  const SbMat & getValue(void) const;

  void makeIdentity(void);
  void setRotate(const SbRotation & q);
  SbMatrix inverse(void) const;
  float det3(int r1, int r2, int r3,
             int c1, int c2, int c3) const;
  float det3(void) const;
  float det4(void) const;

  SbBool equals(const SbMatrix & m, float tolerance) const;


  operator SbMat&(void);
  float * operator [](int i);
  const float * operator [](int i) const;
  SbMatrix & operator =(const SbRotation & q);
  SbMatrix & operator *=(const SbMatrix & m);
  friend COIN_DLL_API SbMatrix operator *(const SbMatrix & m1, const SbMatrix & m2);
  friend COIN_DLL_API int operator ==(const SbMatrix & m1, const SbMatrix & m2);
  friend COIN_DLL_API int operator !=(const SbMatrix & m1, const SbMatrix & m2);
  void getValue(SbMat & m) const;
  static SbMatrix identity(void);
  void setScale(const float s);
  void setScale(const SbVec3f & s);
  void setTranslate(const SbVec3f & t);
  void setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s);
  void setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s,
                    const SbRotation & so);
  void setTransform(const SbVec3f & translation,
                    const SbRotation & rotation, const SbVec3f & scaleFactor,
                    const SbRotation & scaleOrientation, const SbVec3f & center);
  void getTransform(SbVec3f & t, SbRotation & r,
                    SbVec3f & s, SbRotation & so) const;
  void getTransform(SbVec3f & translation, SbRotation & rotation,
                    SbVec3f & scaleFactor, SbRotation & scaleOrientation,
                    const SbVec3f & center) const;
  SbBool factor(SbMatrix & r, SbVec3f & s, SbMatrix & u, SbVec3f & t,
                SbMatrix & proj);
  SbBool LUDecomposition(int index[4], float & d);
  void LUBackSubstitution(int index[4], float b[4]) const;
  SbMatrix transpose(void) const;
  SbMatrix & multRight(const SbMatrix & m);
  SbMatrix & multLeft(const SbMatrix & m);
  void multMatrixVec(const SbVec3f & src, SbVec3f & dst) const;
  void multVecMatrix(const SbVec3f & src, SbVec3f & dst) const;
  void multDirMatrix(const SbVec3f & src, SbVec3f & dst) const;
  void multLineMatrix(const SbLine & src, SbLine & dst) const;
  void multVecMatrix(const SbVec4f & src, SbVec4f & dst) const;

  void print(FILE * fp) const;

private:
  float matrix[4][4];

  void operator /=(const float v);
  void operator *=(const float v);
};

COIN_DLL_API SbMatrix operator *(const SbMatrix & m1, const SbMatrix & m2);
COIN_DLL_API int operator ==(const SbMatrix & m1, const SbMatrix & m2);
COIN_DLL_API int operator !=(const SbMatrix & m1, const SbMatrix & m2);

#endif // !COIN_SBMATRIX_H
