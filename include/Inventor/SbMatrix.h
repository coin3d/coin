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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SBMATRIX_H__
#define __SBMATRIX_H__

#include <stdio.h>
#include <Inventor/SbBasic.h>

class SbLine;
class SbRotation;
class SbVec3f;
class ostream;

typedef float SbMat[4][4];


class SbMatrix {
public:
  SbMatrix(void);
  SbMatrix(const float a11, const float a12, const float a13, const float a14,
	   const float a21, const float a22, const float a23, const float a24,
	   const float a31, const float a32, const float a33, const float a34,
	   const float a41, const float a42, const float a43, const float a44);
  SbMatrix(const SbMat& matrix);
  ~SbMatrix(void);

  SbMatrix& operator =(const SbMat& m);

  operator float*(void);
  SbMatrix& operator =(const SbMatrix& m);
  void setValue(const SbMat& m);
  const SbMat& getValue(void) const;

  void makeIdentity(void);
  void setRotate(const SbRotation& q);
  SbMatrix inverse(void) const;
  float det3(int r1, int r2, int r3,
	     int c1, int c2, int c3) const;
  float det3(void) const;
  float det4(void) const;

  SbBool equals(const SbMatrix& m, float tolerance) const;


  operator SbMat&(void);
  float * operator [](int i);
  const float * operator [](int i) const;
  SbMatrix& operator =(const SbRotation& q);
  SbMatrix& operator *=(const SbMatrix& m);
  friend SbMatrix operator *(const SbMatrix& m1, const SbMatrix& m2);
  friend int operator ==(const SbMatrix& m1, const SbMatrix& m2);
  friend int operator !=(const SbMatrix& m1, const SbMatrix& m2);
  void getValue(SbMat& m) const;
  static SbMatrix identity(void);
  void setScale(const float s);
  void setScale(const SbVec3f& s);
  void setTranslate(const SbVec3f& t);
  void setTransform(const SbVec3f& t, const SbRotation& r, const SbVec3f& s);
  void setTransform(const SbVec3f& t, const SbRotation& r, const SbVec3f& s,
		    const SbRotation& so);
  void setTransform(const SbVec3f& translation,
		    const SbRotation& rotation, const SbVec3f& scaleFactor,
		    const SbRotation& scaleOrientation, const SbVec3f& center);
  void getTransform(SbVec3f& t, SbRotation& r,
		    SbVec3f& s, SbRotation& so) const;
  void getTransform(SbVec3f& translation, SbRotation& rotation,
		    SbVec3f& scaleFactor, SbRotation& scaleOrientation,
		    const SbVec3f& center) const;
  SbBool factor(SbMatrix& r, SbVec3f& s, SbMatrix& u, SbVec3f& t,
		SbMatrix& proj);
  const SbBool LUDecomposition(int index[4], float& d);
  void LUBackSubstitution(int index[4], float b[4]) const;
  SbMatrix transpose(void) const;
  SbMatrix& multRight(const SbMatrix& m);
  SbMatrix& multLeft(const SbMatrix& m);
  void multMatrixVec(const SbVec3f& src, SbVec3f& dst) const;
  void multVecMatrix(const SbVec3f& src, SbVec3f& dst) const;
  void multDirMatrix(const SbVec3f& src, SbVec3f& dst) const;
  void multLineMatrix(const SbLine& src, SbLine& dst) const;

  void print(FILE * fp) const;
  void print(ostream & file) const;

private:
  float matrix[4][4];

  void operator /=(const float v);
  void operator *=(const float v);
};

#endif // !__SBMATRIX_H__
