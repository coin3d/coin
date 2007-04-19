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
  \class SbVec2i32 SbLinear.h Inventor/SbLinear.h
  \brief The SbVec2i32 class is a 2 dimensional vector with short integer 
  coordinates.
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a vector in 2 dimensions
  as well as simple integer arithmetic operations.

  \sa SbVec2f, SbVec2d, SbVec3s, SbVec3f, SbVec3d, SbVec4f, SbVec4d.
*/


#include <assert.h>
#include <Inventor/SbVec2i32.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue().
 */
SbVec2i32::SbVec2i32(void)
{
}

/*!
  Constructs an SbVec2i32 instance with initial values from \a v.
 */
SbVec2i32::SbVec2i32(const int32_t v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
}

/*!
  Constructs an SbVec2i32 instance with the initial vector endpoints from
  \a x and \a y.
 */
SbVec2i32::SbVec2i32(const int32_t x, const int32_t y)
{
  this->vec[0] = x;
  this->vec[1] = y;
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
 */
int32_t
SbVec2i32::dot(const SbVec2i32& v) const
{
  return this->vec[0]*v.vec[0] + this->vec[1]*v.vec[1];
}

/*!
  Returns a pointer to an array of two floats containing the x and y
  coordinates of the vector.

  \sa setValue().
 */
const int32_t *
SbVec2i32::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x and y coordinates of the vector.

  \sa setValue().
 */
void
SbVec2i32::getValue(int32_t & x, int32_t & y) const
{
  x = this->vec[0];
  y = this->vec[1];
}

/*!
  Negate the vector (i.e. point it in the opposite direction).
 */
void
SbVec2i32::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
}

/*!
  Set new x and y coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
 */
SbVec2i32&
SbVec2i32::setValue(const int32_t v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  return *this;
}

/*!
  Set new x and y coordinates for the vector. Returns reference to self.

  \sa getValue().
 */
SbVec2i32&
SbVec2i32::setValue(int32_t x, int32_t y)
{
  this->vec[0] = x;
  this->vec[1] = y;
  return *this;
}

/*!
  Index operator. Returns modifiable x or y coordinate.

  \sa getValue() and setValue().
 */
int32_t &
SbVec2i32::operator [](const int i)
{
#if COIN_DEBUG
  if(!(i==0 || i==1)) {
    SoDebugError::postWarning("SbVec2i32::operator[]",
                              "Index out of bounds [0,1].");
    return this->vec[0];
  }
#endif // COIN_DEBUG

  return this->vec[i];
}

/*!
  Index operator. Returns x or y coordinate.

  \sa getValue().
 */
const int32_t &
SbVec2i32::operator [](const int i) const
{
#if COIN_DEBUG
  if(!(i==0 || i==1)) {
    SoDebugError::postWarning("SbVec2i32::operator[]",
                              "Index out of bounds [0,1].");
    return this->vec[0];
  }
#endif // COIN_DEBUG
  return this->vec[i];
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator *=(int d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  return *this;
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator *=(double d)
{
  this->vec[0] = (int32_t)(this->vec[0] * d);
  this->vec[1] = (int32_t)(this->vec[1] * d);
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator /=(int d)
{
#if COIN_DEBUG
  if(!(d != 0))
    SoDebugError::postWarning("SbVec2i32::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  this->vec[0] /= d;
  this->vec[1] /= d;
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator /=(double d)
{
#if COIN_DEBUG
  if(!(d != 0.0))
    SoDebugError::postWarning("SbVec2i32::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  this->vec[0] = (int32_t)(this->vec[0] / d);
  this->vec[1] = (int32_t)(this->vec[1] / d);
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator +=(const SbVec2i32& u)
{
  this->vec[0] += u[0];
  this->vec[1] += u[1];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
 */
SbVec2i32&
SbVec2i32::operator -=(const SbVec2i32& u)
{
  this->vec[0] -= u[0];
  this->vec[1] -= u[1];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec2i32 instance which
  points in the opposite direction of this vector.

  \sa negate().
 */
SbVec2i32
SbVec2i32::operator-(void) const
{
  return SbVec2i32(-this->vec[0], -this->vec[1]);
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2i32
operator *(const SbVec2i32& v, int d)
{
  return SbVec2i32(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2i32
operator *(const SbVec2i32& v, double d)
{
  return SbVec2i32((int32_t)(v[0]*d), (int32_t)(v[1]*d));
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2i32
operator *(int d, const SbVec2i32& v)
{
  return SbVec2i32(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2i32
operator *(double d, const SbVec2i32& v)
{
  return SbVec2i32((int32_t)(v[0]*d), (int32_t)(v[1]*d));
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  divided on \a d.
 */
SbVec2i32
operator /(const SbVec2i32& v, int d)
{
#if COIN_DEBUG
  if(!(d != 0))
    SoDebugError::postWarning("SbVec2i32::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec2i32(v[0]/d, v[1]/d);
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the components of vector \a v
  divided on \a d.
 */
SbVec2i32
operator /(const SbVec2i32& v, double d)
{
#if COIN_DEBUG
  if(!(d != 0.0))
    SoDebugError::postWarning("SbVec2i32::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec2i32((int32_t)(v[0]/d), (int32_t)(v[1]/d));
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is the sum of vectors \a v1 and \a v2.
 */
SbVec2i32
operator +(const SbVec2i32& v1, const SbVec2i32& v2)
{
  return SbVec2i32(v1[0]+v2[0], v1[1]+v2[1]);
}

/*!
  \relates SbVec2i32

  Returns an SbVec2i32 instance which is vector \a v2 subtracted from
  vector \a v1.
 */
SbVec2i32
operator -(const SbVec2i32& v1, const SbVec2i32& v2)
{
  return SbVec2i32(v1[0]-v2[0], v1[1]-v2[1]);
}

/*!
  \relates SbVec2i32

  Returns \a 1 if \a v1 and \a v2 are equal, \a 0 otherwise.
 */
int
operator ==(const SbVec2i32& v1, const SbVec2i32& v2)
{
  if((v1[0] == v2[0]) && (v1[1] == v2[1])) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec2i32

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.
 */
int
operator !=(const SbVec2i32& v1, const SbVec2i32& v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbVec2i32::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "<%d, %d>", this->vec[0], this->vec[1] );
#endif // COIN_DEBUG
}
