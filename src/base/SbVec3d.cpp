/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SbVec3d SbLinear.h Inventor/SbLinear.h
  \brief The SbVec3d class is a 3 dimensional vector with double precision floating point coordinates.
  \ingroup base

  This vector class provides storage for a 3 dimensional double
  precision floating point vector aswell as simple floating point
  arithmetic operations.

  \sa SbVec2s, SbVec2f, SbVec2d, SbVec3s, SbVec3f, SbVec4f, SbVec4d.
  \since Coin 2.0
  \since TGS Inventor 2.6
*/


//
// documentation of inlined methods
//

/*!
  \fn double & SbVec3d::operator[](const int i)

  Index operator. Returns modifiable x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn double SbVec3d::operator[](const int i) const
  Index operator. Returns x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

#include <assert.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbDPPlane.h>
#include <Inventor/C/tidbitsp.h> // coin_debug_normalize()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \fn SbVec3d::SbVec3d(void)

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  Constructs an SbVec3d instance with initial values from \a v.
*/
SbVec3d::SbVec3d(const double v[3])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
}

/*!
  Constructs an SbVec3d instance with the initial vector endpoint set to
  \a <x,y,z>.
*/
SbVec3d::SbVec3d(const double x, const double y, const double z)
{
  this->vec[0] = x;
  this->vec[1] = y;
  this->vec[2] = z;
}

/*!
  Constructs an SbVec3d instance by combining the three given planes.
  None of the planes should be parallel to any of the other two, otherwise
  a divide by zero error will occur.
*/
SbVec3d::SbVec3d(const SbDPPlane & p0, const SbDPPlane & p1, const SbDPPlane & p2)
{
  SbVec3d n0 = p0.getNormal();
  SbVec3d n1 = p1.getNormal();
  SbVec3d n2 = p2.getNormal();

#if COIN_DEBUG
  if (!((fabs(n0.dot(n1)) != 1.0) &&
       (fabs(n0.dot(n2)) != 1.0) &&
       (fabs(n1.dot(n2)) != 1.0)))
    SoDebugError::postWarning("SbVec3d::SbVec3d",
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
  double a[3][4], x[3], m[3][4];

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
    double sum = 0.0;
    for (j=i+1; j < n; j++) sum += a[i][j]*x[j];

    x[i] = (a[i][n] - sum)/a[i][i];
  }

  this->vec[0] = x[0];
  this->vec[1] = x[1];
  this->vec[2] = x[2];
}

/*!
  Returns the result of taking the cross product of this vector and \a v.
*/
SbVec3d
SbVec3d::cross(const SbVec3d & v) const
{
  return SbVec3d(this->vec[1]*v.vec[2] - this->vec[2]*v.vec[1],
                 this->vec[2]*v.vec[0] - this->vec[0]*v.vec[2],
                 this->vec[0]*v.vec[1] - this->vec[1]*v.vec[0]);
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/
double
SbVec3d::dot(const SbVec3d & v) const
{
  return this->vec[0]*v.vec[0] + this->vec[1]*v.vec[1] + this->vec[2]*v.vec[2];
}

/*!
  Compares the vector with \a v and returns \c TRUE if the distance
  between the vectors is smaller or equal to the square root of
  \a tolerance.
*/
SbBool
SbVec3d::equals(const SbVec3d & v, const double tolerance) const
{
#if COIN_DEBUG
  if (!(tolerance >= 0.0))
    SoDebugError::postWarning("SbVec3d::equals",
                              "Tolerance should be >= 0.0");
#endif // COIN_DEBUG

  double xdist = this->vec[0] - v[0];
  double ydist = this->vec[1] - v[1];
  double zdist = this->vec[2] - v[2];

  return ((xdist*xdist + ydist*ydist + zdist*zdist) <= tolerance);
}

/*!
  Return the vector representing the principal axis closest to this
  vector.
*/
SbVec3d
SbVec3d::getClosestAxis(void) const
{
  SbVec3d closest(0.0, 0.0, 0.0);
  double xabs = (double)fabs(this->vec[0]);
  double yabs = (double)fabs(this->vec[1]);
  double zabs = (double)fabs(this->vec[2]);

  if (xabs>=yabs && xabs>=zabs) closest[0] = (this->vec[0] > 0.0) ? 1.0 : -1.0;
  else if (yabs>=zabs) closest[1] = (this->vec[1] > 0.0) ? 1.0 : -1.0;
  else closest[2] = (this->vec[2] > 0.0) ? 1.0 : -1.0;

  return closest;
}

/*!
  Returns a pointer to an array of three doubles containing the x, y
  and z coordinates of the vector.

  \sa setValue().
*/
const double *
SbVec3d::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x, y and z coordinates of the vector.

  \sa setValue().
*/
void
SbVec3d::getValue(double & x, double & y, double & z) const
{
  x = this->vec[0];
  y = this->vec[1];
  z = this->vec[2];
}

/*!
  Return length of vector.
*/
double
SbVec3d::length(void) const
{
  return (double)sqrt(this->vec[0]*this->vec[0] +
                     this->vec[1]*this->vec[1] +
                     this->vec[2]*this->vec[2]);
}

/*!
  Returns the squared length of the vector.
*/
double
SbVec3d::sqrLength(void) const
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
SbVec3d::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
  this->vec[2] = -this->vec[2];
}

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.

  If the vector is the null vector, no attempt at normalization will
  be done.
*/
double
SbVec3d::normalize(void)
{
  double len = this->length();
  if (len > 0.0) {
    operator/=(len);
  }
#if COIN_DEBUG
  else if (coin_debug_normalize()) {
    SoDebugError::postWarning("SbVec3d::normalize",
                              "The length of the vector should be > 0.0 "
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
SbVec3d &
SbVec3d::setValue(const double v[3])
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
SbVec3d &
SbVec3d::setValue(const double x, const double y, const double z)
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
SbVec3d &
SbVec3d::setValue(const SbVec3d & barycentric,
                  const SbVec3d & v0, const SbVec3d & v1, const SbVec3d & v2)
{
  this->vec[0] = barycentric[0]*v0[0]+barycentric[1]*v1[0]+barycentric[2]*v2[0];
  this->vec[1] = barycentric[0]*v0[1]+barycentric[1]*v1[1]+barycentric[2]*v2[1];
  this->vec[2] = barycentric[0]*v0[2]+barycentric[1]*v1[2]+barycentric[2]*v2[2];
  return *this;
}

/*!
  Sets this vector to the single precision vector \a v, converting
  the vector to a double precision vector.
*/
SbVec3d & 
SbVec3d::setValue(const SbVec3f & v)
{
  this->vec[0] = (double) v[0];
  this->vec[1] = (double) v[1];
  this->vec[2] = (double) v[2];
  return *this;
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
*/
SbVec3d &
SbVec3d::operator *=(const double d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  this->vec[2] *= d;
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
*/
SbVec3d &
SbVec3d::operator /=(const double d)
{
#if COIN_DEBUG
  if (!(d != 0.0))
    SoDebugError::postWarning("SbVec3d::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  // Assumes 1 div and 3 muls is quicker than 3 divs.
  double inv = 1.0/d;
  this->vec[0] *= inv;
  this->vec[1] *= inv;
  this->vec[2] *= inv;
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
*/
SbVec3d &
SbVec3d::operator +=(const SbVec3d & u)
{
  this->vec[0] += u.vec[0];
  this->vec[1] += u.vec[1];
  this->vec[2] += u.vec[2];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
*/
SbVec3d &
SbVec3d::operator -=(const SbVec3d & u)
{
  this->vec[0] -= u.vec[0];
  this->vec[1] -= u.vec[1];
  this->vec[2] -= u.vec[2];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec3d instance
  which points in the opposite direction of this vector.

  \sa negate().
*/
SbVec3d
SbVec3d::operator -(void) const
{
  return SbVec3d(-this->vec[0], -this->vec[1], -this->vec[2]);
}

/*!
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  multiplied with \a d.
*/
SbVec3d
operator *(const SbVec3d & v, const double d)
{
  return SbVec3d(v.vec[0] * d, v.vec[1] * d, v.vec[2] * d);
}

/*!
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  multiplied with \a d.
*/
SbVec3d
operator *(const double d, const SbVec3d & v)
{
  return v * d;
}

/*!
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/
SbVec3d
operator /(const SbVec3d & v, const double d)
{
#if COIN_DEBUG
  if (!(d != 0.0))
    SoDebugError::postWarning("SbVec3d::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec3d(v.vec[0] / d, v.vec[1] / d, v.vec[2] / d);
}

/*!
  \relates SbVec3d

  Returns an SbVec3d instance which is the sum of vectors \a v1 and \a v2.
*/
SbVec3d
operator +(const SbVec3d & v1, const SbVec3d & v2)
{
  return SbVec3d(v1.vec[0] + v2.vec[0],
                 v1.vec[1] + v2.vec[1],
                 v1.vec[2] + v2.vec[2]);
}

/*!
  \relates SbVec3d

  Returns an SbVec3d instance which is vector \a v2 subtracted from
  vector \a v1.
*/
SbVec3d
operator -(const SbVec3d & v1, const SbVec3d & v2)
{
  return SbVec3d(v1.vec[0] - v2.vec[0],
                 v1.vec[1] - v2.vec[1],
                 v1.vec[2] - v2.vec[2]);
}

/*!
  \relates SbVec3d

  Returns \a 1 if \a v1 and \a v2 are \e exactly equal, \a 0 otherwise.

  \sa equals().
*/
int
operator ==(const SbVec3d & v1, const SbVec3d & v2)
{
  if (v1.vec[0] == v2.vec[0] &&
     v1.vec[1] == v2.vec[1] &&
     v1.vec[2] == v2.vec[2]) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec3d

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/
int
operator !=(const SbVec3d & v1, const SbVec3d & v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized
  compile.
*/
void
SbVec3d::print(FILE * fp) const
{
#if COIN_DEBUG
  (void)fprintf(fp, "<%f, %f, %f>", this->vec[0], this->vec[1], this->vec[2]);
#endif // COIN_DEBUG
}
