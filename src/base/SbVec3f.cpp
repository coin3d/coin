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

/*!
  \class SbVec3f SbLinear.h Inventor/SbLinear.h
  \brief The SbVec3f class is a 3 dimensional vector with floating point coordinates.
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a 3 dimensional vector
  aswell as simple floating point arithmetic operations.

  \sa SbVec2f, SbVec2s, SbVec3s, SbVec4f.
*/


//
// documentation of inlined methods
//

/*!
  \fn float & SbVec3f::operator[](const int i)

  Index operator. Returns modifiable x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn float SbVec3f::operator[](const int i) const
  Index operator. Returns x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

#include <assert.h>
#include <float.h> // FLT_MAX
#include <Inventor/SbPlane.h>
#include <Inventor/SbVec3d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \fn SbVec3f::SbVec3f(void);

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  Constructs an SbVec3f instance with initial values from \a v.
*/
SbVec3f::SbVec3f(const float v[3])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
}

/*!
  Constructs an SbVec3f instance with the initial vector endpoint set to
  \a <x,y,z>.
*/
SbVec3f::SbVec3f(const float x, const float y, const float z)
{
  this->vec[0] = x;
  this->vec[1] = y;
  this->vec[2] = z;
}

/*!
  Constructs an SbVec3f instance by combining the three given planes.
  None of the planes should be parallel to any of the other two, otherwise
  a divide by zero error will occur.
*/
SbVec3f::SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2)
{
  SbVec3f n0 = p0.getNormal();
  SbVec3f n1 = p1.getNormal();
  SbVec3f n2 = p2.getNormal();

#if COIN_DEBUG
  if (!((fabs(n0.dot(n1)) != 1.0f) &&
       (fabs(n0.dot(n2)) != 1.0f) &&
       (fabs(n1.dot(n2)) != 1.0f)))
    SoDebugError::postWarning("SbVec3f::SbVec3f",
                              "Two or more of the given planes are parallel"
                              " => Can't create intersection point.");
#endif // COIN_DEBUG

  // The equation for a point in a plane can be:
  //
  //                N·(P - P0) = 0    , N is the plane's normal vectors,
  //                                    P is the point and P0 is the "root
  //                                    point" of the plane (i.e. the point
  //                                    in the plane closest to the coordinate
  //                                    system origin)
  //
  // Simplifying and substituting, we get this:
  //
  //                N·P = d           , d is the distance from the origin to
  //                                    the closest point on the plane
  //
  // Using this for all three given planes:
  //                N0·P = d0
  //                N1·P = d1
  //                N2·P = d2
  //
  // Taking the dot products we get a set of linear equations:
  //
  //   n0x*px + n0y*py + n0z*pz = d0
  //   n1x*px + n1y*py + n1z*pz = d1
  //   n2x*px + n2y*py + n2z*pz = d2   , where [px, py, pz] are the unknowns.
  //
  // This can be solved by applying the Gauss elimination method. See
  // for instance "Advanced Engineering Mathemathics", Kreyszig, 6th edition,
  // chapter 19.
  //                                                        19980817 mortene.


  // a is the input matrix, x is the solution vector, m is a matrix
  // used for temporary storage.
  float a[3][4], x[3], m[3][4];

  a[0][0] = n0[0];
  a[0][1] = n0[1];
  a[0][2] = n0[2];
  a[0][3] = p0.getDistanceFromOrigin();
  a[1][0] = n1[0];
  a[1][1] = n1[1];
  a[1][2] = n1[2];
  a[1][3] = p1.getDistanceFromOrigin();
  a[2][0] = n2[0];
  a[2][1] = n2[1];
  a[2][2] = n2[2];
  a[2][3] = p2.getDistanceFromOrigin();


  int i, j;
  const int n = 3; // Input matrix dimensions are n × (n+1).

  for (int k=0; k < n-1; k++) {
    j=k;

    while (a[j][k] == 0.0f) j++;
    if (j != k) for (i=0; i < n+1; i++) SbSwap(a[j][i], a[k][i]);

    for (j = k+1; j < n; j++) {
      m[j][k] = a[j][k]/a[k][k];

      for (int p=k+1; p < n+1; p++) a[j][p] -= m[j][k]*a[k][p];
    }
  }

  // Back substitution.
  x[n-1] = a[n-1][n]/a[n-1][n-1];
  for (i=n-2; i >= 0; i--) {
    float sum = 0.0f;
    for (j=i+1; j < n; j++) sum += a[i][j]*x[j];

    x[i] = (a[i][n] - sum)/a[i][i];
  }

  this->vec[0] = x[0];
  this->vec[1] = x[1];
  this->vec[2] = x[2];
}

/*!  
  Constructs an SbVec3f instance with initial values from the
  double precision vector \a v.  

  \since 2002-04-25
*/
SbVec3f::SbVec3f(const SbVec3d & v)
{
  this->setValue(v);
}

/*!
  Returns the result of taking the cross product of this vector and \a v.
*/
SbVec3f
SbVec3f::cross(const SbVec3f & v) const
{
  return SbVec3f(this->vec[1]*v.vec[2] - this->vec[2]*v.vec[1],
                 this->vec[2]*v.vec[0] - this->vec[0]*v.vec[2],
                 this->vec[0]*v.vec[1] - this->vec[1]*v.vec[0]);
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/
float
SbVec3f::dot(const SbVec3f & v) const
{
  return this->vec[0]*v.vec[0] + this->vec[1]*v.vec[1] + this->vec[2]*v.vec[2];
}

/*!
  Compares the vector with \a v and returns \c TRUE if the largest
  distance between the vectors are larger than the square root of the
  given tolerance value.
*/
SbBool
SbVec3f::equals(const SbVec3f & v, const float tolerance) const
{
#if COIN_DEBUG
  if (!(tolerance >= 0.0f))
    SoDebugError::postWarning("SbVec3f::equals",
                              "Tolerance should be >= 0.0f");
#endif // COIN_DEBUG

  float xdist = this->vec[0] - v[0];
  float ydist = this->vec[1] - v[1];
  float zdist = this->vec[2] - v[2];

  return ((xdist*xdist + ydist*ydist + zdist*zdist) <= tolerance);
}

/*!
  Return the vector representing the principal axis closest to this
  vector.
*/
SbVec3f
SbVec3f::getClosestAxis(void) const
{
  SbVec3f closest(0.0f, 0.0f, 0.0f);
  float xabs = (float)fabs(this->vec[0]);
  float yabs = (float)fabs(this->vec[1]);
  float zabs = (float)fabs(this->vec[2]);

  if (xabs>=yabs && xabs>=zabs) closest[0] = (this->vec[0] > 0.0f) ? 1.0f : -1.0f;
  else if (yabs>=zabs) closest[1] = (this->vec[1] > 0.0f) ? 1.0f : -1.0f;
  else closest[2] = (this->vec[2] > 0.0f) ? 1.0f : -1.0f;

  return closest;
}

/*!
  Returns a pointer to an array of three floats containing the x, y
  and z coordinates of the vector.

  \sa setValue().
*/
const float *
SbVec3f::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x, y and z coordinates of the vector.

  \sa setValue().
*/
void
SbVec3f::getValue(float & x, float & y, float & z) const
{
  x = this->vec[0];
  y = this->vec[1];
  z = this->vec[2];
}

/*!
  Return length of vector.
*/
float
SbVec3f::length(void) const
{
  return (float)sqrt(this->vec[0]*this->vec[0] +
                     this->vec[1]*this->vec[1] +
                     this->vec[2]*this->vec[2]);
}

/*!
  Returns the squared length of the vector.
*/
float
SbVec3f::sqrLength(void) const
{
  return
    this->vec[0]*this->vec[0] +
    this->vec[1]*this->vec[1] +
    this->vec[2]*this->vec[2];
}

/*!
  Negate the vector (i.e. point it in the opposite direction).
*/
void
SbVec3f::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
  this->vec[2] = -this->vec[2];
}

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.

  If the vector is the null vector, no attempt at normalization will
  be done, and if the Coin library was built in a debug version, this
  error message will then be shown:

  \verbatim
    Coin warning in SbVec3f::normalize(): The length of the vector
    should be > 0.0f to be able to normalize.
  \endverbatim

  We've made Coin spit out a warning when an attempt at normalizing a
  null-vector is made, as that seems to always be a symptom caused by
  some graver programming error somewhere else -- either internally in
  Coin code, or in application code.

  If this happens, you should run the application in a debugger and see
  how the call-stack backtrace looks when it hits. An easy way of
  getting a debugger break at the warning spot is to set the following
  debugging environment variable which will make the code assert:

  \verbatim
    COIN_DEBUG_BREAK="SbVec3f::normalize"
  \endverbatim

  If you from the backtrace analysis strongly suspects an internal Coin
  bug, please report the call-stack to us at \e coin-support@coin3d.org
  and we'll look into it. Example code that triggers the bug would
  then also be very helpful.
*/
float
SbVec3f::normalize(void)
{
  float len = this->length();

  if (len > 0.0f) {
    operator/=(len);
  }
#if COIN_DEBUG
  else {
    SoDebugError::postWarning("SbVec3f::normalize",
                              "The length of the vector should be > 0.0f "
                              "to be able to normalize.");
  }
#endif // COIN_DEBUG

  return len;
}

/*!
  Set new coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
*/
SbVec3f &
SbVec3f::setValue(const float v[3])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
  return *this;
}

/*!
  Set new coordinates for the vector. Returns reference to self.

  \sa getValue().
*/
SbVec3f &
SbVec3f::setValue(const float x, const float y, const float z)
{
  this->vec[0] = x;
  this->vec[1] = y;
  this->vec[2] = z;
  return *this;
}

/*!
  Set this vector to be the average of \a v0, \a v1 and \a v2.
  The vector components are weighted by the \a barycentric vector.

  \sa getValue().
*/
SbVec3f &
SbVec3f::setValue(const SbVec3f & barycentric,
                  const SbVec3f & v0, const SbVec3f & v1, const SbVec3f & v2)
{
  this->vec[0] = barycentric[0]*v0[0]+barycentric[1]*v1[0]+barycentric[2]*v2[0];
  this->vec[1] = barycentric[0]*v0[1]+barycentric[1]*v1[1]+barycentric[2]*v2[1];
  this->vec[2] = barycentric[0]*v0[2]+barycentric[1]*v1[2]+barycentric[2]*v2[2];
  return *this;
}

/*!
  Sets this vector to the double precision vector \a v, converting the
  vector to a single precision vector.

  \since 2002-04-25
*/
SbVec3f & 
SbVec3f::setValue(const SbVec3d & v)
{
#if COIN_DEBUG
  if (v[0] < - FLT_MAX ||
      v[0] > FLT_MAX ||
      v[1] < -FLT_MAX ||
      v[1] > FLT_MAX ||
      v[2] < -FLT_MAX ||
      v[2] > FLT_MAX) {
    SoDebugError::postWarning("SbVec3f::setValue",
                              "The double precision vector will not fit into a "
                              "single precision vector.");
  }
#endif // COIN_DEBUG

  this->vec[0] = (float) v[0];
  this->vec[1] = (float) v[1];
  this->vec[2] = (float) v[2];
  return *this;
}

/*!
  Multiply components of vector with scalar value \a d. Returns
  reference to self.
*/
SbVec3f &
SbVec3f::operator *=(const float d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  this->vec[2] *= d;
  return *this;
}

/*!
  Divides components of vector with scalar value \a d. Returns
  reference to self.
*/
SbVec3f &
SbVec3f::operator /=(const float d)
{
#if COIN_DEBUG
  if (!(d != 0.0f))
    SoDebugError::postWarning("SbVec3f::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  // Assumes 1 div and 3 muls is quicker than 3 divs.
  float inv = 1.0f/d;
  this->vec[0] *= inv;
  this->vec[1] *= inv;
  this->vec[2] *= inv;
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
*/
SbVec3f &
SbVec3f::operator +=(const SbVec3f & u)
{
  this->vec[0] += u.vec[0];
  this->vec[1] += u.vec[1];
  this->vec[2] += u.vec[2];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
*/
SbVec3f &
SbVec3f::operator -=(const SbVec3f & u)
{
  this->vec[0] -= u.vec[0];
  this->vec[1] -= u.vec[1];
  this->vec[2] -= u.vec[2];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec3f instance
  which points in the opposite direction of this vector.

  \sa negate().
*/
SbVec3f
SbVec3f::operator-(void) const
{
  return SbVec3f(-this->vec[0], -this->vec[1], -this->vec[2]);
}

/*!
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  multiplied with \a d.
*/
SbVec3f
operator *(const SbVec3f & v, const float d)
{
  return SbVec3f(v.vec[0] * d, v.vec[1] * d, v.vec[2] * d);
}

/*!
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  multiplied with \a d.
*/
SbVec3f
operator *(const float d, const SbVec3f & v)
{
  return v * d;
}

/*!
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/
SbVec3f
operator /(const SbVec3f & v, const float d)
{
#if COIN_DEBUG
  if (!(d != 0.0f))
    SoDebugError::postWarning("SbVec3f::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec3f(v.vec[0] / d, v.vec[1] / d, v.vec[2] / d);
}

/*!
  \relates SbVec3f

  Returns an SbVec3f instance which is the sum of vectors \a v1 and \a v2.
*/
SbVec3f
operator +(const SbVec3f & v1, const SbVec3f & v2)
{
  return SbVec3f(v1.vec[0] + v2.vec[0],
                 v1.vec[1] + v2.vec[1],
                 v1.vec[2] + v2.vec[2]);
}

/*!
  \relates SbVec3f

  Returns an SbVec3f instance which is vector \a v2 subtracted from
  vector \a v1.
*/
SbVec3f
operator -(const SbVec3f & v1, const SbVec3f & v2)
{
  return SbVec3f(v1.vec[0] - v2.vec[0],
                 v1.vec[1] - v2.vec[1],
                 v1.vec[2] - v2.vec[2]);
}

/*!
  \relates SbVec3f

  Returns \a 1 if \a v1 and \a v2 are \e exactly equal, \a 0 otherwise.

  \sa equals().
*/
int
operator ==(const SbVec3f & v1, const SbVec3f & v2)
{
  if (v1.vec[0] == v2.vec[0] &&
      v1.vec[1] == v2.vec[1] &&
      v1.vec[2] == v2.vec[2]) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec3f

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/
int
operator !=(const SbVec3f & v1, const SbVec3f & v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized
  compile.
*/
void
SbVec3f::print(FILE * fp) const
{
#if COIN_DEBUG
  (void)fprintf(fp, "<%f, %f, %f>", this->vec[0], this->vec[1], this->vec[2]);
#endif // COIN_DEBUG
}
