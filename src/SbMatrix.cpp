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

/*!
  \class SbMatrix SbLinear.h Inventor/SbLinear.h
  \brief The SbMatrix class is a 4x4 dimensional representation of a matrix.
  \ingroup base

  SbMatrix is used by many other classes in Coin.
  It provides storage for a 4x4 matrix in row-major mode. Many common
  geometrical operations which involves matrix calculations are implemented
  as methods on this class.
*/

/*¡
  These methods are still not implemented:
  <UL>
  <LI>SbMatrix::getTransform() (x2)</LI>
  <LI>SbMatrix::factor()</LI>
  </UL>

  Optimizations are not done yet, so there's a <B>lot</B> of room for
  improvements.
*/


#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbLine.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <iostream.h> // For print(ostream)
#endif // COIN_DEBUG


/*!
  The default constructor does nothing. The matrix will be uninitialized.
 */
SbMatrix::SbMatrix(void)
{
}


/*!
  Constructs a matrix instance with the given initial elements.
 */
SbMatrix::SbMatrix(const float a11, const float a12,
		   const float a13, const float a14,
		   const float a21, const float a22,
		   const float a23, const float a24,
		   const float a31, const float a32,
		   const float a33, const float a34,
		   const float a41, const float a42,
		   const float a43, const float a44)
{
  this->matrix[0][0] = a11;
  this->matrix[0][1] = a12;
  this->matrix[0][2] = a13;
  this->matrix[0][3] = a14;

  this->matrix[1][0] = a21;
  this->matrix[1][1] = a22;
  this->matrix[1][2] = a23;
  this->matrix[1][3] = a24;

  this->matrix[2][0] = a31;
  this->matrix[2][1] = a32;
  this->matrix[2][2] = a33;
  this->matrix[2][3] = a34;

  this->matrix[3][0] = a41;
  this->matrix[3][1] = a42;
  this->matrix[3][2] = a43;
  this->matrix[3][3] = a44;
}

/*!
  Constructs a matrix instance with the initial elements from the
  \a matrix argument.
 */
SbMatrix::SbMatrix(const SbMat& matrix)
{
  this->setValue(matrix);
}


/*!
  Default destructor does nothing.
 */
SbMatrix::~SbMatrix(void)
{
}

/*!
  Returns a pointer to the 2 dimensional float array with the matrix
  elements.

  \sa setValue().
 */
const SbMat&
SbMatrix::getValue(void) const
{
  return this->matrix;
}

/*!
  Copies the elements from \a m into the matrix.

  \sa getValue().
 */
void
SbMatrix::setValue(const SbMat& m)
{
  memcpy(this->matrix, m, sizeof(float)*4*4);
}

/*!
  Assignment operator. Copies the elements from \a m to the matrix.
 */
SbMatrix&
SbMatrix::operator =(const SbMat& m)
{
  this->setValue(m);
  return *this;
}

/*!
  Assignment operator. Copies the elements from \a m to the matrix.
 */
SbMatrix&
SbMatrix::operator =(const SbMatrix& m)
{
  this->setValue(m.matrix);
  return *this;
}

/*!
  Set the matrix to be the identity matrix.

  \sa identity().
 */
void
SbMatrix::makeIdentity(void)
{
  this->matrix[0][0]=this->matrix[1][1]=
    this->matrix[2][2]=this->matrix[3][3] = 1.0f;

  this->matrix[0][1]=this->matrix[0][2]=this->matrix[0][3]=
    this->matrix[1][0]=this->matrix[1][2]=this->matrix[1][3]=
    this->matrix[2][0]=this->matrix[2][1]=this->matrix[2][3]=
    this->matrix[3][0]=this->matrix[3][1]=this->matrix[3][2] = 0.0f;
}

/*!
  Set matrix to be a rotation matrix with the given rotation.

  \sa setTranslate(), setScale().
*/
void
SbMatrix::setRotate(const SbRotation& q)
{
  SbVec3f u;
  float t,cost,sint;

  q.getValue(u, t);
  cost=cos(t);
  sint=sin(t);

  // pederb, 19990423
  // added tests for special cases ==> quite a lot faster
  // since most rotations are around one of the axes
  // FIXME: I think I have seen a faster way to do these
  // tests somewhere...
  if (u == SbVec3f(1.0f, 0.0f, 0.0f)) {
    this->makeIdentity();
    this->matrix[1][1] = cost;
    this->matrix[2][1] = -sint;
    this->matrix[2][1] = sint;
    this->matrix[2][2] = cost;
  }
  else if (u == SbVec3f(0.0f, 1.0f, 0.0f)) {
    this->makeIdentity();
    this->matrix[0][0] = cost;
    this->matrix[2][0] = sint;
    this->matrix[0][2] = -sint;
    this->matrix[2][2] = cost;
  }
  else if (u == SbVec3f(0.0f, 0.0f, 1.0f)) {
    this->makeIdentity();
    this->matrix[0][0] = cost;
    this->matrix[1][0] = -sint;
    this->matrix[0][1] = sint;
    this->matrix[1][1] = cost;
  }
  else {
    this->matrix[3][0]=this->matrix[3][1]=this->matrix[3][2]=
      this->matrix[0][3]=this->matrix[1][3]=this->matrix[2][3]=0.0f;
    this->matrix[3][3]=1.0f;
    
    float u0_2 = u[0]*u[0];
    float u1_2 = u[1]*u[1];
    float u2_2 = u[2]*u[2];
    
    this->matrix[0][0]=u[0]*u[0]+cost*(1-u[0]*u[0]);
    this->matrix[1][0]=u[0]*u[1]*(1-cost)-u[2]*sint;
    this->matrix[2][0]=u[2]*u[0]*(1-cost)+u[1]*sint;
    this->matrix[0][1]=u[0]*u[1]*(1-cost)+u[2]*sint;
    this->matrix[1][1]=u[1]*u[1]+cost*(1-u[1]*u[1]);
    this->matrix[2][1]=u[1]*u[2]*(1-cost)-u[0]*sint;
    this->matrix[0][2]=u[2]*u[0]*(1-cost)-u[1]*sint;
    this->matrix[1][2]=u[1]*u[2]*(1-cost)+u[0]*sint;
    this->matrix[2][2]=u[2]*u[2]+cost*(1-u[2]*u[2]);
  }
}

/*!
  Multiply all element values in the matrix with \a v.
 */
void
SbMatrix::operator *=(const float v)
{
  for(int i=0; i < 4; i++) {
    for(int j=0; j < 4; j++) {
      this->matrix[i][j] *= v;
    }
  }
}

/*!
  Divide all element values in the matrix on \a v.
 */
void
SbMatrix::operator /=(const float v)
{
#if COIN_DEBUG
  if (v==0.0f)
    SoDebugError::postWarning("SbMatrix::operator/=",
			      "Division by zero.");
#endif // COIN_DEBUG

  operator*=(1.0f/v);
}

/*!
  Returns the determinant of the 3x3 submatrix specified by the row and
  column indices.
 */
float
SbMatrix::det3(int r1, int r2, int r3,
	       int c1, int c2, int c3) const
{
#if COIN_DEBUG
  if (r1<0 || r1>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "r1 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", r1);
    if (r1<0) r1=0;
    else if (r1>3) r1=3;
  }
  if (r2<0 || r2>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "r2 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", r2);
    if (r2<0) r2=0;
    else if (r2>3) r2=3;
  }
  if (r3<0 || r3>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "r3 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", r3);
    if (r3<0) r3=0;
    else if (r3>3) r3=3;
  }
  if (c1<0 || c1>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "c1 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", c1);
    if (c1<0) c1=0;
    else if (c1>3) c1=3;
  }
  if (c2<0 || c2>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "c2 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", c2);
    if (c2<0) c2=0;
    else if (c2>3) c2=3;
  }
  if (c3<0 || c3>3) {
    SoDebugError::postWarning("SbMatrix::det3",
			      "c3 parameter (%f) out of bounds [0,3]. "
			      "Clamping to bounds.", c3);
    if (c3<0) c3=0;
    else if (c3>3) c3=3;
  }
#endif // COIN_DEBUG

#if COIN_DEBUG
  if (r1==r2 || r1==r3 || r2==r3) 
    SoDebugError::postWarning("SbMatrix::det3",
			      "Row indices (%d,%d,%d) should be distinct.",
			      r1, r2, r3);
  if (c1==c2 || c1==c3 || c2==c3) 
    SoDebugError::postWarning("SbMatrix::det3",
			      "Column indices (%d,%d,%d) should be distinct.",
			      c1, c2, c3);
#endif // COIN_DEBUG

  // More or less directly from "Advanced Engineering Mathematics"
  // (E. Kreyszig), 6th edition.

  float a11 = this->matrix[r1][c1];
  float a12 = this->matrix[r1][c2];
  float a13 = this->matrix[r1][c3];
  float a21 = this->matrix[r2][c1];
  float a22 = this->matrix[r2][c2];
  float a23 = this->matrix[r2][c3];
  float a31 = this->matrix[r3][c1];
  float a32 = this->matrix[r3][c2];
  float a33 = this->matrix[r3][c3];

  float M11 = a22*a33 - a32*a23;
  float M21 = -(a12*a33 - a32*a13);
  float M31 = a12*a23 - a22*a13;

  return (a11*M11 + a21*M21 + a31*M31);
}

/*!
  Returns the determinant of the upper left 3x3 submatrix.
 */
float
SbMatrix::det3(void) const
{
  return this->det3(0, 1, 2, 0, 1, 2);
}

/*!
  Returns the determinant of the matrix.
 */
float
SbMatrix::det4(void) const
{
  // TODO: erraneous? this code gives the same results in the regression
  // test as described in Kreyszig, but OI returns other values (except
  // for 1 of the testmatrices!).

  float det = 0.0f;
  det += this->matrix[0][0] * det3(1,2,3,1,2,3);
  det -= this->matrix[1][0] * det3(0,2,3,1,2,3);
  det += this->matrix[2][0] * det3(0,1,3,1,2,3);
  det -= this->matrix[3][0] * det3(0,1,2,1,2,3);
  return det;
}

/*!
  Return a new matrix which is the inverse matrix of this.

  The user is responsible for checking that this is a valid operation
  to execute, by first making sure that the result of SbMatrix::det4()
  is not equal to zero.
 */
SbMatrix
SbMatrix::inverse(void) const
{
  float det = this->det4();
#if COIN_DEBUG
  if (det == 0.0f) {
    SoDebugError::postWarning("SbMatrix::inverse",
			      "Determinant of matrix is zero.");
    return *this;
  }
#endif // COIN_DEBUG

  SbMatrix result;

  result.matrix[0][0] = this->det3(1,2,3,1,2,3);
  result.matrix[1][0] = -this->det3(1,2,3,0,2,3);
  result.matrix[2][0] = this->det3(1,2,3,0,1,3);
  result.matrix[3][0] = -this->det3(1,2,3,0,1,2);
  result.matrix[0][1] = -this->det3(0,2,3,1,2,3);
  result.matrix[1][1] = this->det3(0,2,3,0,2,3);
  result.matrix[2][1] = -this->det3(0,2,3,0,1,3);
  result.matrix[3][1] = this->det3(0,2,3,0,1,2);
  result.matrix[0][2] = this->det3(0,1,3,1,2,3);
  result.matrix[1][2] = -this->det3(0,1,3,0,2,3);
  result.matrix[2][2] = this->det3(0,1,3,0,1,3);
  result.matrix[3][2] = -this->det3(0,1,3,0,1,2);
  result.matrix[0][3] = -this->det3(0,1,2,1,2,3);
  result.matrix[1][3] = this->det3(0,1,2,0,2,3);
  result.matrix[2][3] = -this->det3(0,1,2,0,1,3);
  result.matrix[3][3] = this->det3(0,1,2,0,1,2);

  result /= det;
  return result;
}

/*!
  Check if the \a m matrix is equal to this one, within the given tolerance
  value. The tolerance value is applied in the comparison on a component by
  component basis.
 */
SbBool
SbMatrix::equals(const SbMatrix& m, float tolerance) const
{
#if COIN_DEBUG
  if (tolerance<0.0f)
    SoDebugError::postWarning("SbMatrix::equals",
			      "tolerance should be >=0.0f.");
#endif // COIN_DEBUG

  for(int i=0; i < 4; i++) {
    for(int j=0;  j< 4; j++) {
      if(fabs(this->matrix[i][j] - m.matrix[i][j]) > tolerance) return FALSE;
    }
  }

  return TRUE;
}


/*!
  Return pointer to the matrix' 4x4 float array.
 */
SbMatrix::operator float*(void)
{
  return &(this->matrix[0][0]);
}


/*!
  Return pointer to the matrix' 4x4 float array.
 */
SbMatrix::operator SbMat&(void)
{
  return this->matrix;
}

/*!
  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
 */
float *
SbMatrix::operator [](int i)
{
#if COIN_DEBUG
  if (i<0 || i>3) {
    SoDebugError::postWarning("SbMatrix::operator[]",
			      "Index (%d) is out of bounds. "
			      "Clamping to bounds.");
    if (i<0) i=0;
    else if (i>3) i=3;
  }
#endif // COIN_DEBUG

   return matrix[i];
}

/*!
  Returns pointer to the 4 element array representing a matrix row.
  \a i should be within [0, 3].

  \sa getValue(), setValue().
 */
const float *
SbMatrix::operator [](int i) const
{
#if COIN_DEBUG
  if (i<0 || i>3) {
    SoDebugError::postWarning("SbMatrix::operator[]",
			      "Index (%d) is out of bounds. "
			      "Clamping to bounds.");
    if (i<0) i=0;
    else if (i>3) i=3;
  }
#endif // COIN_DEBUG

   return matrix[i];
}

/*!
  Set matrix to be a rotation matrix with the given rotation.

  \sa setRotate().
*/
SbMatrix&
SbMatrix::operator =(const SbRotation& q)
{
  this->setRotate(q);
  return *this;
}

/*!
  Right-multiply with the \a m matrix.

  \sa multRight().
 */
SbMatrix&
SbMatrix::operator *=(const SbMatrix& m)
{
  return this->multRight(m);
}

/*!
  \relates SbMatrix

  Multiplies matrix \a m1 with matrix \a m2 and returns the resultant
  matrix.
*/
SbMatrix
operator *(const SbMatrix& m1, const SbMatrix& m2)
{
  SbMatrix result = m1;
  result *= m2;
  return result;
}

/*!
  \relates SbMatrix

  Compare matrices to see if they are equal. For two matrices to be equal,
  all their individual elements must be equal.

  \sa equals().
*/
int
operator ==(const SbMatrix& m1, const SbMatrix& m2)
{
  for(int i=0; i < 4; i++) {
    for(int j=0; j < 4; j++) {
      if(m1[i][j] != m2[i][j]) return FALSE;
    }
  }

  return TRUE;
}

/*!
  \relates SbMatrix

  Compare matrices to see if they are not equal. For two matrices to not be
  equal, it is enough that at least one of their elements are unequal.

  \sa equals().
*/
int
operator !=(const SbMatrix& m1, const SbMatrix& m2)
{
  return !(m1 == m2);
}

/*!
  Return matrix components in the SbMat structure.

  \sa setValue().
 */
void
SbMatrix::getValue(SbMat& m) const
{
  memcpy(&m[0][0], &(this->matrix[0][0]), sizeof(float)*4*4);
}

/*!
  Return the identity matrix.

  \sa makeIdentity().
 */
SbMatrix
SbMatrix::identity(void)
{
  return SbMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		  0.0f, 1.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f,
		  0.0f, 0.0f, 0.0f, 1.0f);
}

/*!
  Set matrix to be a pure scaling matrix. Scale factors are specified
  by \a s.

  \sa setRotate(), setTranslate().
 */
void
SbMatrix::setScale(const float s)
{
  this->makeIdentity();
  this->matrix[0][0] = s;
  this->matrix[1][1] = s;
  this->matrix[2][2] = s;
}

/*!
  Set matrix to be a pure scaling matrix. Scale factors in x, y and z
  is specified by the \a s vector.

  \sa setRotate(), setTranslate().
 */
void
SbMatrix::setScale(const SbVec3f& s)
{
  this->makeIdentity();
  this->matrix[0][0] = s[0];
  this->matrix[1][1] = s[1];
  this->matrix[2][2] = s[2];
}

/*!
  Make this matrix into a pure translation matrix (no scale or rotation
  components) with the given vector \t as the translation.

  \sa setRotate(), setScale().
 */
void
SbMatrix::setTranslate(const SbVec3f& t)
{
  this->makeIdentity();

  this->matrix[3][0] = t[0];
  this->matrix[3][1] = t[1];
  this->matrix[3][2] = t[2];
}

/*!
  Set translation, rotation and scaling all at once. The resulting matrix
  gets calculated like this:

  M = S * R * T,

  where \a S, \a R and \a T is scaling, rotation and translation matrices.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
 */
void
SbMatrix::setTransform(const SbVec3f& t, const SbRotation& r, const SbVec3f& s)
{
  SbMatrix tmp;
  
  this->setScale(s);

  tmp.setRotate(r);
  this->multRight(tmp);

  tmp.setTranslate(t);
  this->multRight(tmp);
}

/*!
  Set translation, rotation and scaling all at once with a specified
  scale orientation. The resulting matrix gets calculated like this:

  M = Ro-¹ * S * Ro * R * T,

  where \a Ro is the scale orientation, and \a S, \a R
  and \a T is scaling, rotation and translation.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
 */
void
SbMatrix::setTransform(const SbVec3f& t, const SbRotation& r, const SbVec3f& s,
		       const SbRotation& so)
{
  SbMatrix tmp;
  
  this->setRotate(so.inverse());

  tmp.setScale(s);
  this->multRight(tmp);

  tmp.setRotate(so);
  this->multRight(tmp);

  tmp.setRotate(r);
  this->multRight(tmp);

  tmp.setTranslate(t);
  this->multRight(tmp);
}

/*!
  Set translation, rotation and scaling all at once with a specified
  scale orientation and center point. The resulting matrix gets calculated
  like this:

  M = -Tc * Ro-¹ * S * Ro * R * T * Tc,

  where \a Tc is the center point, \a Ro the scale orientation, \a S, \a R
  and \a T is scaling, rotation and translation.

  \sa setTranslate(), setRotate(), setScale() and getTransform().
 */
void
SbMatrix::setTransform(const SbVec3f& translation,
		       const SbRotation& rotation,
		       const SbVec3f& scaleFactor,
		       const SbRotation& scaleOrientation,
		       const SbVec3f& center)
{
#if 0 // replaced with optimized version, 19990423 pederb
  SbMatrix tmp;
  
  this->setTranslate(-center);

  tmp.setRotate(scaleOrientation.inverse());
  this->multRight(tmp);

  tmp.setScale(scaleFactor);
  this->multRight(tmp);

  tmp.setRotate(scaleOrientation);
  this->multRight(tmp);

  tmp.setRotate(rotation);
  this->multRight(tmp);

  tmp.setTranslate(translation);
  this->multRight(tmp);

  tmp.setTranslate(center);
  this->multRight(tmp);

#else // optimized version
  // should be quite a lot faster. Matrix multiplication is 
  // expensive, and it rarely happens that all arguments
  // are used in a Transform node. For VRML2Transform this
  // is very important, since the Transform node has replaced
  // the Separator node for that file format.
  //  pederb, 19990423

  SbMatrix tmp;
  this->setTranslate(-center);
  SbBool doorient = scaleOrientation.getValue()[3] != 0.0f;
  if (doorient) {
    tmp.setRotate(scaleOrientation.inverse());
    this->multRight(tmp);
  }
  if (scaleFactor != SbVec3f(1.0f, 1.0f, 1.0f)) {
    tmp.setScale(scaleFactor);
    this->multRight(tmp);
  } 
  if (doorient) {
    tmp.setRotate(scaleOrientation);
    this->multRight(tmp);
  }
  if (rotation.getValue()[3] != 0.0f) {
    tmp.setRotate(rotation);
    this->multRight(tmp);
  }
  if (translation + center != SbVec3f(0.0f, 0.0f, 0.0f)) {
    tmp.setTranslate(translation+center);
    this->multRight(tmp);
  }
#endif // end of optimized version
}

/*!
  Factor the matrix back into its translation, rotation, scale and
  scaleorientation components.

  TODO: not implemented

  \sa factor()
 */
void
SbMatrix::getTransform(SbVec3f& t, SbRotation& r, SbVec3f& s,
		       SbRotation& so) const
{
  // TODO: not implemented.
  assert(0);
}

/*!
  Factor the matrix back into its translation, rotation, scale and
  scaleorientation components. Will translate from \a center to
  origo before the factorization takes place.

  TODO: not implemented

  \sa factor()
 */
void
SbMatrix::getTransform(SbVec3f& translation,
		       SbRotation& rotation,
		       SbVec3f& scaleFactor,
		       SbRotation& scaleOrientation,
		       const SbVec3f& center) const
{
  // TODO: not implemented.
  assert(0);
}

/*!
  TODO: not implemented, not documented

  \sa getTransform()
 */
SbBool
SbMatrix::factor(SbMatrix& r, SbVec3f& s, SbMatrix& u, SbVec3f& t,
		 SbMatrix& proj)
{
  // TODO: not implemented.
  assert(0);
  return FALSE;
}

/*!
  This function produces a permuted LU decomposition of the matrix.  It
  uses the common single-row-pivoting strategy.

  \a FALSE is returned if the matrix is singular, which it never is, because
  of small adjustment values inserted if a singularity is found (as Open
  Inventor does too).

  The parity argument is always set to 1.0 or -1.0.  Don't really know what
  it's for, so it's not checked for correctness.

  The index[] argument returns the permutation that was done on the matrix
  to LU-decompose it.  index[i] is the row that row i was swapped with at
  step i in the decomposition, so index[] is not the actual permutation of
  the row indexes!

  BUGS:  The function does not produce results that are numerically identical
  with those produced by Open Inventor for the same matrices, because the
  pivoting strategy in OI was never fully understood.

  \sa SbMatrix::LUBackSubstitution
*/


const SbBool
SbMatrix::LUDecomposition(
    int index[4],
    float & d )
{
    int i;
    for ( i = 0; i < 4; i++ ) index[i] = i;
    d = 1.0f;

    const float MINIMUM_PIVOT = 1e-6f; // Inventor fix...

    for ( int row = 1; row < 4; row++ ) {
        int swap_row = row;
        float max_pivot = 0.0f;
        for ( int test_row = row; test_row < 4; test_row++ ) {
            const float test_pivot = SbAbs( matrix[test_row][row] );
            if ( test_pivot > max_pivot ) {
                swap_row = test_row;
                max_pivot = test_pivot;
            }
        }
             
        // swap rows
        if ( swap_row != row ) {
            d = -d;
            index[row] = swap_row;
            for ( i = 0; i < 4; i++ )
                SbSwap( matrix[row][i], matrix[swap_row][i] );
        }

        float pivot = matrix[row][row];
        if ( matrix[row][row] == 0.0f ) {
//            return FALSE;
            // instead of returning FALSE on singulars...
            matrix[row][row] = pivot = MINIMUM_PIVOT;
        }

        // the factorization
        for ( i = row + 1; i < 4; i++ ) {
            const float factor = (matrix[i][row] /= pivot);
            for ( int j = row + 1; j < 4; j++ )
                matrix[i][j] -= factor * matrix[row][j];
        }
    }
    return TRUE;
} // LUDecomposition()

/*!
  This function does a solve on the "Ax = b" system, given that the matrix
  is LU-decomposed in advance.  First, a forward substitution is done on the
  lower system (Ly = b), and then a backwards substitution is done on the
  upper triangular system (Ux = y).

  The index[] argument is the one returned from
  SbMatrix::LUDecomposition(), so see that function for an explanation.

  The b[] argument must contain the b vector in "Ax = b" when calling the
  function.  After the function has solved the system, the b[] vector contains
  the x vector.

  BUGS:  As is done by Open Inventor, unsolvable x values will not return
  NaN but 0.
*/

void
SbMatrix::LUBackSubstitution(
    int index[4],
    float b[4] ) const
{
    int i;

    // permute b[] the way matrix[][] is permuted
    for ( i = 0; i < 4; i++ )
        if ( i != index[i] )
            SbSwap( b[i], b[index[i]] );

    // forward substitution on L (Ly = b)
    float y[4];
    for ( i = 0; i < 4; i++ ) {
        float sum = 0.0f;
        for ( int j = 0; j < i; j++ )
            sum += matrix[i][j] * y[j];
        y[i] = b[i] - sum;
    }

    // backwards substitution on U (Ux = y)
    float x[4];
    for ( i = 3; i >= 0; i-- ) {
        float sum = 0.0f;
        for ( int j = i + 1; j < 4; j++ )
             sum += matrix[i][j] * x[j];
        if ( matrix[i][i] != 0.0f )
            x[i] = ( y[i] - sum ) / matrix[i][i];
        else
            x[i] = 0.0f;
    }

    // de-permute x[]?  doesn't look like it
//    for ( i = 3; i >= 0; i-- )
//        if ( i != index[i] )
//            SbSwap( x[i], x[index[i]] );

    // copy x[] into b[] for "return to sender"
    for ( i = 0; i < 4; i++ ) b[i] = x[i];
} // LUBackSubstitution()

/*!
  Returns the transpose of this matrix.
*/ 

SbMatrix
SbMatrix::transpose(void) const
{
  SbMatrix trans = (*this);

  for(int i=0; i < 3; i++) {
    for(int j=i+1; j < 4; j++) {
      SbSwap(trans[i][j], trans[j][i]);
    }
  }

  return trans;
}

/*!
  Let this matrix to be right-multiplied by \a m. Returns reference to
  self.

  \sa multLeft()
*/
SbMatrix &
SbMatrix::multRight(const SbMatrix & m)
{
  SbMatrix tmp;
  const float *m0 = m[0];
  const float *m1 = m[1];
  const float *m2 = m[2];
  const float *m3 = m[3];
  const float *t0 = (*this)[0];
  const float *t1 = (*this)[1];
  const float *t2 = (*this)[2];
  const float *t3 = (*this)[3];

  tmp[0][0]=m0[0]*t0[0]+m1[0]*t0[1]+m2[0]*t0[2]+m3[0]*t0[3];
  tmp[0][1]=m0[1]*t0[0]+m1[1]*t0[1]+m2[1]*t0[2]+m3[1]*t0[3];
  tmp[0][2]=m0[2]*t0[0]+m1[2]*t0[1]+m2[2]*t0[2]+m3[2]*t0[3];
  tmp[0][3]=m0[3]*t0[0]+m1[3]*t0[1]+m2[3]*t0[2]+m3[3]*t0[3];
  tmp[1][0]=m0[0]*t1[0]+m1[0]*t1[1]+m2[0]*t1[2]+m3[0]*t1[3];
  tmp[1][1]=m0[1]*t1[0]+m1[1]*t1[1]+m2[1]*t1[2]+m3[1]*t1[3];
  tmp[1][2]=m0[2]*t1[0]+m1[2]*t1[1]+m2[2]*t1[2]+m3[2]*t1[3];
  tmp[1][3]=m0[3]*t1[0]+m1[3]*t1[1]+m2[3]*t1[2]+m3[3]*t1[3];
  tmp[2][0]=m0[0]*t2[0]+m1[0]*t2[1]+m2[0]*t2[2]+m3[0]*t2[3];
  tmp[2][1]=m0[1]*t2[0]+m1[1]*t2[1]+m2[1]*t2[2]+m3[1]*t2[3];
  tmp[2][2]=m0[2]*t2[0]+m1[2]*t2[1]+m2[2]*t2[2]+m3[2]*t2[3];
  tmp[2][3]=m0[3]*t2[0]+m1[3]*t2[1]+m2[3]*t2[2]+m3[3]*t2[3];
  tmp[3][0]=m0[0]*t3[0]+m1[0]*t3[1]+m2[0]*t3[2]+m3[0]*t3[3];
  tmp[3][1]=m0[1]*t3[0]+m1[1]*t3[1]+m2[1]*t3[2]+m3[1]*t3[3];
  tmp[3][2]=m0[2]*t3[0]+m1[2]*t3[1]+m2[2]*t3[2]+m3[2]*t3[3];
  tmp[3][3]=m0[3]*t3[0]+m1[3]*t3[1]+m2[3]*t3[2]+m3[3]*t3[3];

  memcpy(this->matrix, tmp.matrix, sizeof(float)*4*4);
  return *this;
}

/*!
  Let this matrix to be left-multiplied by \a m. Returns reference
  to self.

  \sa multRight().
*/
SbMatrix&
SbMatrix::multLeft(const SbMatrix& m)
{
  SbMatrix tmp;
  const float *m0 = m[0];
  const float *m1 = m[1];
  const float *m2 = m[2];
  const float *m3 = m[3];
  const float *t0 = (*this)[0];
  const float *t1 = (*this)[1];
  const float *t2 = (*this)[2];
  const float *t3 = (*this)[3];

  tmp[0][0]=m0[0]*t0[0]+m0[1]*t1[0]+m0[2]*t2[0]+m0[3]*t3[0];
  tmp[0][1]=m0[0]*t0[1]+m0[1]*t1[1]+m0[2]*t2[1]+m0[3]*t3[1];
  tmp[0][2]=m0[0]*t0[2]+m0[1]*t1[2]+m0[2]*t2[2]+m0[3]*t3[2];
  tmp[0][3]=m0[0]*t0[3]+m0[1]*t1[3]+m0[2]*t2[3]+m0[3]*t3[3];
  tmp[1][0]=m1[0]*t0[0]+m1[1]*t1[0]+m1[2]*t2[0]+m1[3]*t3[0];
  tmp[1][1]=m1[0]*t0[1]+m1[1]*t1[1]+m1[2]*t2[1]+m1[3]*t3[1];
  tmp[1][2]=m1[0]*t0[2]+m1[1]*t1[2]+m1[2]*t2[2]+m1[3]*t3[2];
  tmp[1][3]=m1[0]*t0[3]+m1[1]*t1[3]+m1[2]*t2[3]+m1[3]*t3[3];
  tmp[2][0]=m2[0]*t0[0]+m2[1]*t1[0]+m2[2]*t2[0]+m2[3]*t3[0];
  tmp[2][1]=m2[0]*t0[1]+m2[1]*t1[1]+m2[2]*t2[1]+m2[3]*t3[1];
  tmp[2][2]=m2[0]*t0[2]+m2[1]*t1[2]+m2[2]*t2[2]+m2[3]*t3[2];
  tmp[2][3]=m2[0]*t0[3]+m2[1]*t1[3]+m2[2]*t2[3]+m2[3]*t3[3];
  tmp[3][0]=m3[0]*t0[0]+m3[1]*t1[0]+m3[2]*t2[0]+m3[3]*t3[0];
  tmp[3][1]=m3[0]*t0[1]+m3[1]*t1[1]+m3[2]*t2[1]+m3[3]*t3[1];
  tmp[3][2]=m3[0]*t0[2]+m3[1]*t1[2]+m3[2]*t2[2]+m3[3]*t3[2];
  tmp[3][3]=m3[0]*t0[3]+m3[1]*t1[3]+m3[2]*t2[3]+m3[3]*t3[3];

  memcpy(this->matrix, tmp.matrix, sizeof(float)*4*4);
  return *this;
}

/*!
  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the right side of the
  expression, i.e. dst = M*src.
  
  \sa multVecMatrix(), multDirMatrix() and multLineMatrix().
*/
void
SbMatrix::multMatrixVec(const SbVec3f& src, SbVec3f& dst) const
{
  const float *t0 = (*this)[0];
  const float *t1 = (*this)[1];
  const float *t2 = (*this)[2];
  const float *t3 = (*this)[3];
  // Copy the src vector, just in case src and dst is the same vector.
  SbVec3f s = src;

  double W = s[0]*t3[0] + s[1]*t3[1] + s[2]*t3[2] + t3[3];

  dst[0] = (s[0]*t0[0] + s[1]*t0[1] + s[2]*t0[2] + t0[3])/W;
  dst[1] = (s[0]*t1[0] + s[1]*t1[1] + s[2]*t1[2] + t1[3])/W;
  dst[2] = (s[0]*t2[0] + s[1]*t2[1] + s[2]*t2[2] + t2[3])/W;
}

/*!
  Multiply \a src vector with this matrix and return the result in \a dst.
  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src*M.

  It is safe to let \a src and \dst be the same SbVec3f instance.
  
  \sa multMatrixVec(), multDirMatrix() and multLineMatrix().
*/
void
SbMatrix::multVecMatrix(const SbVec3f& src, SbVec3f& dst) const
{
  const float *t0 = (*this)[0];
  const float *t1 = (*this)[1];
  const float *t2 = (*this)[2];
  const float *t3 = (*this)[3];
  // Copy the src vector, just in case src and dst is the same vector.
  SbVec3f s = src;

  double W = s[0]*t0[3] + s[1]*t1[3] + s[2]*t2[3] + t3[3];

  dst[0] = (s[0]*t0[0] + s[1]*t1[0] + s[2]*t2[0] + t3[0])/W;
  dst[1] = (s[0]*t0[1] + s[1]*t1[1] + s[2]*t2[1] + t3[1])/W;
  dst[2] = (s[0]*t0[2] + s[1]*t1[2] + s[2]*t2[2] + t3[2])/W;
}

/*!
  Multiplies \a src by the matrix. \a src is assumed to be a direction
  vector, and the translation components of the matrix are therefore
  ignored.

  Multiplication is done with the vector on the left side of the
  expression, i.e. dst = src*M.

  \sa multVecMatrix(), multMatrixVec() and multLineMatrix().
 */
void
SbMatrix::multDirMatrix(const SbVec3f& src, SbVec3f& dst) const
{
  const float *t0 = (*this)[0];
  const float *t1 = (*this)[1];
  const float *t2 = (*this)[2];
  // Copy the src vector, just in case src and dst is the same vector.
  SbVec3f s = src;

  dst[0] = s[0]*t0[0] + s[1]*t1[0] + s[2]*t2[0];
  dst[1] = s[0]*t0[1] + s[1]*t1[1] + s[2]*t2[1];
  dst[2] = s[0]*t0[2] + s[1]*t1[2] + s[2]*t2[2];
}

/*!
  Multiplies line point with the full matrix and multiplies the
  line direction with the matrix without the translation components.

  \sa multVecMatrix(), multMatrixVec() and multDirMatrix().
 */
void
SbMatrix::multLineMatrix(const SbLine& src, SbLine& dst) const
{
  SbVec3f newpt, newdir;
  this->multVecMatrix(src.getPosition(), newpt);
  this->multDirMatrix(src.getDirection(), newdir);

  dst.setValue(newpt, newpt + newdir);
}

/*!
  Write out the matrix contents to the given file.
 */
void
SbMatrix::print(FILE * fp) const
{
  for (int i=0; i < 4; i++) {
    fprintf(fp, "%10.5g\t%10.5g\t%10.5g\t%10.5g\n",
	    this->matrix[i][0], this->matrix[i][1],
	    this->matrix[i][2], this->matrix[i][3]);
  }
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbMatrix::print(ostream & file) const
{
#if COIN_DEBUG
  for (int i=0; i < 4; i++) {
    file << "\t" << this->matrix[i][0] << "\t" << this->matrix[i][1] << "\t"
	 << this->matrix[i][2] << "\t" << this->matrix[i][3] << endl;
  }
#endif // COIN_DEBUG
}
