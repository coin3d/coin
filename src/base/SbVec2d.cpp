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
  \class SbVec2d SbLinear.h Inventor/SbLinear.h
  \brief The SbVec2d class is a 2 dimensional vector with double precision
  floating point coordinates.
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a vector in 2 dimensions aswell
  as simple floating point arithmetic operations on this vector.

  \sa SbVec2s, SbVec3s, SbVec3f, SbVec4f
*/

#include <assert.h>
#include <Inventor/SbVec2d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

/*!
  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue().
 */

SbVec2d::SbVec2d(void)
{
}

/*!
  Constructs an SbVec2d instance with initial values from \a v.
 */

SbVec2d::SbVec2d(const double v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
}

/*!
  Constructs an SbVec2d instance with the initial vector endpoints from
  \a x and \a y.
 */

SbVec2d::SbVec2d(const double x, const double y)
{
  this->vec[0] = x;
  this->vec[1] = y;
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
 */

double
SbVec2d::dot(const SbVec2d& v) const
{
  return this->vec[0]*v.vec[0] + this->vec[1]*v.vec[1];
}

/*!
  Compares the vector with \a v and returns TRUE if the largest distance
  between the vectors are larger than the square root of the given
  tolerance value.
 */

SbBool
SbVec2d::equals(const SbVec2d& v, const double tolerance) const
{
#if COIN_DEBUG
  if(!(tolerance >= 0.0f))
    SoDebugError::postWarning("SbVec2d::equals",
                              "Tolerance should be >= 0.0f");
#endif // COIN_DEBUG

  double xdist = this->vec[0] - v[0];
  double ydist = this->vec[1] - v[1];

  if((xdist*xdist + ydist*ydist) <= tolerance) return TRUE;
  return FALSE;
}

/*!
  Returns a pointer to an array of two double containing the x and y
  coordinates of the vector.

  \sa setValue().
 */

const double *
SbVec2d::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x and y coordinates of the vector.

  \sa setValue().
 */

void
SbVec2d::getValue(double& x, double& y) const
{
  x = this->vec[0];
  y = this->vec[1];
}

/*!
  Return length of vector.
 */

double
SbVec2d::length(void) const
{
  return (double)sqrt(this->vec[0]*this->vec[0] + this->vec[1]*this->vec[1]);
}

/*!
  Negate the vector (i.e. point it in the opposite direction).
 */

void
SbVec2d::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
}

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.
 */

double
SbVec2d::normalize(void)
{
  double len = this->length();
#if COIN_DEBUG
  if(!(len > 0.0f)) {
    SoDebugError::postWarning("SbVec2d::normalize",
                              "The length of the vector should be > 0.0f "
                              "to be able to normalize.");
  }
#endif // COIN_DEBUG

  if (len > 0.0f) operator/=(len);
  return len;
}

/*!
  Set new x and y coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
 */

SbVec2d&
SbVec2d::setValue(const double v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  return *this;
}

/*!
  Set new x and y coordinates for the vector. Returns reference to self.

  \sa getValue().
 */

SbVec2d&
SbVec2d::setValue(const double x, const double y)
{
  this->vec[0] = x;
  this->vec[1] = y;
  return *this;
}

/*!
  Index operator. Returns modifiable x or y coordinate.

  \sa getValue() and setValue().
 */

double&
SbVec2d::operator [](const int i)
{
#if COIN_DEBUG
  if(!(i==0 || i==1))
    SoDebugError::postWarning("SbVec2d::operator[]",
                              "Index out of bounds [0,1].");
#endif // COIN_DEBUG
  return this->vec[i];
}

/*!
  Index operator. Returns x or y coordinate.

  \sa getValue().
 */

const double&
SbVec2d::operator [](const int i) const
{
#if COIN_DEBUG
  if(!(i==0 || i==1))
    SoDebugError::postWarning("SbVec2d::operator[]",
                              "Index out of bounds [0,1].");
#endif // COIN_DEBUG
  return this->vec[i];
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */

SbVec2d&
SbVec2d::operator *=(const double d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */

SbVec2d&
SbVec2d::operator /=(const double d)
{
#if COIN_DEBUG
  if(!(d != 0.0f))
    SoDebugError::postWarning("SbVec2d::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  this->vec[0] /= d;
  this->vec[1] /= d;
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
 */

SbVec2d&
SbVec2d::operator +=(const SbVec2d& u)
{
  this->vec[0] += u[0];
  this->vec[1] += u[1];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
 */
SbVec2d&
SbVec2d::operator -=(const SbVec2d& u)
{
  this->vec[0] -= u[0];
  this->vec[1] -= u[1];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec2d instance which
  points in the opposite direction of this vector.

  \sa negate().
 */

SbVec2d
SbVec2d::operator -(void) const
{
  return SbVec2d(-this->vec[0], -this->vec[1]);
}

/*!
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  multiplied with \a d.
 */

SbVec2d
operator *(const SbVec2d& v, const double d)
{
  return SbVec2d(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  multiplied with \a d.
 */

SbVec2d
operator *(const double d, const SbVec2d& v)
{
  return SbVec2d(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  divided on the scalar factor \a d.
 */

SbVec2d
operator /(const SbVec2d& v, const double d)
{
#if COIN_DEBUG
  if(!(d != 0.0f))
    SoDebugError::postWarning("SbVec2d::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec2d(v[0]/d, v[1]/d);
}

/*!
  \relates SbVec2d

  Returns an SbVec2d instance which is the sum of vectors \a v1 and \a v2.
 */

SbVec2d
operator +(const SbVec2d& v1, const SbVec2d& v2)
{
  return SbVec2d(v1[0]+v2[0], v1[1]+v2[1]);
}

/*!
  \relates SbVec2d

  Returns an SbVec2d instance which is vector \a v2 subtracted from
  vector \a v1.
 */

//$ EXPORT INLINE
SbVec2d
operator -(const SbVec2d& v1, const SbVec2d& v2)
{
  return SbVec2d(v1[0]-v2[0], v1[1]-v2[1]);
}

/*!
  \relates SbVec2d

  Returns \a 1 if \a v1 and \a v2 are equal, \a 0 otherwise.

  \sa equals().
 */

//$ EXPORT INLINE
int
operator ==(const SbVec2d& v1, const SbVec2d& v2)
{
  if((v1[0] == v2[0]) && (v1[1] == v2[1])) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec2d

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
 */

//$ EXPORT INLINE
int
operator !=(const SbVec2d& v1, const SbVec2d& v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbVec2d::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "<%f, %f>", this->vec[0], this->vec[1] );
#endif // COIN_DEBUG
}
