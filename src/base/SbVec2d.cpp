/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

  \sa SbVec2s, SbVec2f, SbVec3s, SbVec3f, SbVec3d, SbVec4f, SbVec4d.
*/

#include <Inventor/SbVec2d.h>

#include <limits>
#include <cassert>

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2b.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2i32.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "tidbitsp.h" // coin_debug_normalize()

// *************************************************************************

/*!
  \fn SbVec2d::SbVec2d(void)
  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue().
*/

/*!
  \fn SbVec2d::SbVec2d(const double v[2])

  Constructs an SbVec2d instance with initial values from \a v.
*/

/*!
  \fn SbVec2d::SbVec2d(double x, double y)

  Constructs an SbVec2d instance with the initial vector endpoints from
  \a x and \a y.
*/

/*!
  \fn SbVec2d::SbVec2d(const SbVec2f & v)

  Constructs an SbVec2d instance from an SbVec2f instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec2d::SbVec2d(const SbVec2b & v)

  Constructs an SbVec2d instance from an SbVec2b instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec2d::SbVec2d(const SbVec2s & v)

  Constructs an SbVec2d instance from an SbVec2s instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec2d::SbVec2d(const SbVec2i32 & v)

  Constructs an SbVec2d instance from an SbVec2i32 instance.

  \since Coin 2.5
*/

/*!
  \fn double SbVec2d::dot(const SbVec2d& v) const

  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/

/*!
  Compares the vector with \a v and returns \c TRUE if the distance
  between the vectors is smaller or equal to the square root of
  \a tolerance.
*/

SbBool
SbVec2d::equals(const SbVec2d& v, double tolerance) const
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
  \fn const double * SbVec2d::getValue(void) const

  Returns a pointer to an array of two double containing the x and y
  coordinates of the vector.

  \sa setValue().
*/

/*!
  \fn void SbVec2d::getValue(double& x, double& y) const

  Returns the x and y coordinates of the vector.

  \sa setValue().
*/

/*!
  Return length of vector.
*/

double
SbVec2d::length(void) const
{
  return static_cast<double>(sqrt(this->sqrLength()));
}

/*!
  \fn void SbVec2d::negate(void)

  Negate the vector (i.e. point it in the opposite direction).
*/

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.
*/

double
SbVec2d::normalize(void)
{
  double len = this->length();
  if (len > 0.0) {
    operator/=(len);
  }
#if COIN_DEBUG
  else if (coin_debug_normalize()) {
    SoDebugError::postWarning("SbVec2d::normalize",
                              "The length of the vector should be > 0.0f "
                              "to be able to normalize.");
  }
#endif // COIN_DEBUG
  return len;
}

/*!
  \fn SbVec2d & SbVec2d::setValue(const double v[2])

  Set new x and y coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
*/

/*!
  \fn SbVec2d & SbVec2d::setValue(double x, double y)

  Set new x and y coordinates for the vector. Returns reference to self.

  \sa getValue().
*/

/*!
  Sets the value from an SbVec2f instance.
  Returns reference to itself.

  \since Coin 2.5
*/

SbVec2d &
SbVec2d::setValue(const SbVec2f & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  return *this;
}

/*!
  Sets the value from an SbVec2b instance.
  Returns reference to itself.

  \since Coin 2.5
*/

SbVec2d &
SbVec2d::setValue(const SbVec2b & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  return *this;
}

/*!
  Sets the value from an SbVec2s instance.
  Returns reference to itself.

  \since Coin 2.5
*/

SbVec2d &
SbVec2d::setValue(const SbVec2s & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  return *this;
}
/*!
  Sets the value from an SbVec2i32 instance.
  Returns reference to itself.

  \since Coin 2.5
*/

SbVec2d &
SbVec2d::setValue(const SbVec2i32 & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  return *this;
}

/*!
  \fn double & SbVec2d::operator [] (int i)

  Index operator. Returns modifiable x or y coordinate.

  \sa getValue() and setValue().
 */

/*!
  \fn const double & SbVec2d::operator [] (int i) const

  Index operator. Returns x or y coordinate.

  \sa getValue().
*/

/*!
  \fn SbVec2d & SbVec2d::operator *= (double d)

  Multiply components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn SbVec2d & SbVec2d::operator /= (double d)

  Divides components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn SbVec2d & SbVec2d::operator += (const SbVec2d & v)

  Adds this vector and vector \a u. Returns reference to self.
*/

/*!
  \fn SbVec2d & SbVec2d::operator -= (const SbVec2d & v)

  Subtracts vector \a u from this vector. Returns reference to self.
*/

/*!
  \fn SbVec2d SbVec2d::operator - (void) const

  Non-destructive negation operator. Returns a new SbVec2d instance which
  points in the opposite direction of this vector.

  \sa negate().
*/

/*!
  \fn SbVec2d operator * (const SbVec2d & v, double d)
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  multiplied with \a d.
 */

/*!
  \fn SbVec2d operator * (double d, const SbVec2d & v)
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  multiplied with \a d.
 */

/*!
  \fn SbVec2d operator / (const SbVec2d & v, double d)
  \relates SbVec2d

  Returns an SbVec2d instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/

/*!
  \fn SbVec2d operator + (const SbVec2d & v1, const SbVec2d & v2)
  \relates SbVec2d

  Returns an SbVec2d instance which is the sum of vectors \a v1 and \a v2.
*/

/*!
  \fn SbVec2d operator - (const SbVec2d & v1, const SbVec2d & v2)
  \relates SbVec2d

  Returns an SbVec2d instance which is vector \a v2 subtracted from
  vector \a v1.
*/

/*!
  \fn int operator == (const SbVec2d & v1, const SbVec2d & v2)
  \relates SbVec2d

  Returns \a 1 if \a v1 and \a v2 are equal, \a 0 otherwise.

  \sa equals().
*/

/*!
  \fn int operator != (const SbVec2d & v1, const SbVec2d & v2)
  \relates SbVec2d

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/

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
