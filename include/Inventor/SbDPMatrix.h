#ifndef COIN_SBDPMATRIX_H
#define COIN_SBDPMATRIX_H

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

class SbDPLine;
class SbDPRotation;
class SbVec3d;
class SbVec4d;
class SbMatrix;

typedef double SbDPMat[4][4];

class COIN_DLL_API SbDPMatrix {
public:
  SbDPMatrix(void);
  SbDPMatrix(const double a11, const double a12, const double a13, const double a14,
             const double a21, const double a22, const double a23, const double a24,
             const double a31, const double a32, const double a33, const double a34,
             const double a41, const double a42, const double a43, const double a44);
  SbDPMatrix(const SbDPMat & matrix);
  SbDPMatrix(const SbDPMat * matrix);
  SbDPMatrix(const SbMatrix & matrix);
  ~SbDPMatrix(void);

  SbDPMatrix & operator =(const SbDPMat & m);

  operator double*(void);
  SbDPMatrix & operator =(const SbDPMatrix & m);
  void setValue(const SbDPMat & m);
  const SbDPMat & getValue(void) const;

  void makeIdentity(void);
  void setRotate(const SbDPRotation & q);
  SbDPMatrix inverse(void) const;
  double det3(int r1, int r2, int r3,
             int c1, int c2, int c3) const;
  double det3(void) const;
  double det4(void) const;

  SbBool equals(const SbDPMatrix & m, double tolerance) const;


  operator SbDPMat&(void);
  double * operator [](int i);
  const double * operator [](int i) const;
  SbDPMatrix & operator =(const SbDPRotation & q);
  SbDPMatrix & operator *=(const SbDPMatrix & m);
  friend COIN_DLL_API SbDPMatrix operator *(const SbDPMatrix & m1, const SbDPMatrix & m2);
  friend COIN_DLL_API int operator ==(const SbDPMatrix & m1, const SbDPMatrix & m2);
  friend COIN_DLL_API int operator !=(const SbDPMatrix & m1, const SbDPMatrix & m2);
  void getValue(SbDPMat & m) const;
  static SbDPMatrix identity(void);
  void setScale(const double s);
  void setScale(const SbVec3d & s);
  void setTranslate(const SbVec3d & t);
  void setTransform(const SbVec3d & t, const SbDPRotation & r, const SbVec3d & s);
  void setTransform(const SbVec3d & t, const SbDPRotation & r, const SbVec3d & s,
                    const SbDPRotation & so);
  void setTransform(const SbVec3d & translation,
                    const SbDPRotation & rotation, const SbVec3d & scaleFactor,
                    const SbDPRotation & scaleOrientation, const SbVec3d & center);
  void getTransform(SbVec3d & t, SbDPRotation & r,
                    SbVec3d & s, SbDPRotation & so) const;
  void getTransform(SbVec3d & translation, SbDPRotation & rotation,
                    SbVec3d & scaleFactor, SbDPRotation & scaleOrientation,
                    const SbVec3d & center) const;
  SbBool factor(SbDPMatrix & r, SbVec3d & s, SbDPMatrix & u, SbVec3d & t,
                SbDPMatrix & proj);
  SbBool LUDecomposition(int index[4], double & d);
  void LUBackSubstitution(int index[4], double b[4]) const;
  SbDPMatrix transpose(void) const;
  SbDPMatrix & multRight(const SbDPMatrix & m);
  SbDPMatrix & multLeft(const SbDPMatrix & m);
  void multMatrixVec(const SbVec3d & src, SbVec3d & dst) const;
  void multVecMatrix(const SbVec3d & src, SbVec3d & dst) const;
  void multDirMatrix(const SbVec3d & src, SbVec3d & dst) const;
  void multLineMatrix(const SbDPLine & src, SbDPLine & dst) const;
  void multVecMatrix(const SbVec4d & src, SbVec4d & dst) const;

  void print(FILE * fp) const;

private:
  double matrix[4][4];

  void operator /=(const double v);
  void operator *=(const double v);
};

COIN_DLL_API SbDPMatrix operator *(const SbDPMatrix & m1, const SbDPMatrix & m2);
COIN_DLL_API int operator ==(const SbDPMatrix & m1, const SbDPMatrix & m2);
COIN_DLL_API int operator !=(const SbDPMatrix & m1, const SbDPMatrix & m2);

#endif // !COIN_SBDPMATRIX_H
