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
  \class SbVec2s SbLinear.h Inventor/SbLinear.h
  \brief The SbVec2s class is a 2 dimensional vector with short integer 
  coordinates.
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a vector in 2 dimensions
  as well as simple integer arithmetic operations.

  \sa SbVec3s, SbVec2f, SbVec3f, SbVec4f.
*/


#include <assert.h>
#include <Inventor/SbVec2s.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue().
 */
SbVec2s::SbVec2s(void)
{
}

/*!
  Constructs an SbVec2s instance with initial values from \a v.
 */
SbVec2s::SbVec2s(const short v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
}

/*!
  Constructs an SbVec2s instance with the initial vector endpoints from
  \a x and \a y.
 */
SbVec2s::SbVec2s(const short x, const short y)
{
  this->vec[0] = x;
  this->vec[1] = y;
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
 */
int32_t
SbVec2s::dot(const SbVec2s& v) const
{
  return this->vec[0]*v.vec[0] + this->vec[1]*v.vec[1];
}

/*!
  Returns a pointer to an array of two floats containing the x and y
  coordinates of the vector.

  \sa setValue().
 */
const short *
SbVec2s::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x and y coordinates of the vector.

  \sa setValue().
 */
void
SbVec2s::getValue(short& x, short& y) const
{
  x = this->vec[0];
  y = this->vec[1];
}

/*!
  Negate the vector (i.e. point it in the opposite direction).
 */
void
SbVec2s::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
}

/*!
  Set new x and y coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
 */
SbVec2s&
SbVec2s::setValue(const short v[2])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  return *this;
}

/*!
  Set new x and y coordinates for the vector. Returns reference to self.

  \sa getValue().
 */
SbVec2s&
SbVec2s::setValue(short x, short y)
{
  this->vec[0] = x;
  this->vec[1] = y;
  return *this;
}

/*!
  Index operator. Returns modifiable x or y coordinate.

  \sa getValue() and setValue().
 */
short&
SbVec2s::operator [](const int i)
{
#if COIN_DEBUG
  if(!(i==0 || i==1)) {
    SoDebugError::postWarning("SbVec2s::operator[]",
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
const short&
SbVec2s::operator [](const int i) const
{
#if COIN_DEBUG
  if(!(i==0 || i==1)) {
    SoDebugError::postWarning("SbVec2s::operator[]",
                              "Index out of bounds [0,1].");
    return this->vec[0];
  }
#endif // COIN_DEBUG
  return this->vec[i];
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator *=(int d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  return *this;
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator *=(double d)
{
  this->vec[0] = (short int)(this->vec[0] * d);
  this->vec[1] = (short int)(this->vec[1] * d);
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator /=(int d)
{
#if COIN_DEBUG
  if(!(d != 0))
    SoDebugError::postWarning("SbVec2s::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  this->vec[0] /= d;
  this->vec[1] /= d;
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator /=(double d)
{
#if COIN_DEBUG
  if(!(d != 0.0))
    SoDebugError::postWarning("SbVec2s::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  this->vec[0] = (short int)(this->vec[0] / d);
  this->vec[1] = (short int)(this->vec[1] / d);
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator +=(const SbVec2s& u)
{
  this->vec[0] += u[0];
  this->vec[1] += u[1];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
 */
SbVec2s&
SbVec2s::operator -=(const SbVec2s& u)
{
  this->vec[0] -= u[0];
  this->vec[1] -= u[1];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec2s instance which
  points in the opposite direction of this vector.

  \sa negate().
 */
SbVec2s
SbVec2s::operator-(void) const
{
  return SbVec2s(-this->vec[0], -this->vec[1]);
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2s
operator *(const SbVec2s& v, int d)
{
  return SbVec2s(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2s
operator *(const SbVec2s& v, double d)
{
  return SbVec2s((short)(v[0]*d), (short)(v[1]*d));
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2s
operator *(int d, const SbVec2s& v)
{
  return SbVec2s(v[0]*d, v[1]*d);
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec2s
operator *(double d, const SbVec2s& v)
{
  return SbVec2s((short)(v[0]*d), (short)(v[1]*d));
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  divided on \a d.
 */
SbVec2s
operator /(const SbVec2s& v, int d)
{
#if COIN_DEBUG
  if(!(d != 0))
    SoDebugError::postWarning("SbVec2s::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec2s(v[0]/d, v[1]/d);
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the components of vector \a v
  divided on \a d.
 */
SbVec2s
operator /(const SbVec2s& v, double d)
{
#if COIN_DEBUG
  if(!(d != 0.0))
    SoDebugError::postWarning("SbVec2s::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec2s((short)(v[0]/d), (short)(v[1]/d));
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is the sum of vectors \a v1 and \a v2.
 */
SbVec2s
operator +(const SbVec2s& v1, const SbVec2s& v2)
{
  return SbVec2s(v1[0]+v2[0], v1[1]+v2[1]);
}

/*!
  \relates SbVec2s

  Returns an SbVec2s instance which is vector \a v2 subtracted from
  vector \a v1.
 */
SbVec2s
operator -(const SbVec2s& v1, const SbVec2s& v2)
{
  return SbVec2s(v1[0]-v2[0], v1[1]-v2[1]);
}

/*!
  \relates SbVec2s

  Returns \a 1 if \a v1 and \a v2 are equal, \a 0 otherwise.
 */
int
operator ==(const SbVec2s& v1, const SbVec2s& v2)
{
  if((v1[0] == v2[0]) && (v1[1] == v2[1])) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec2s

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.
 */
int
operator !=(const SbVec2s& v1, const SbVec2s& v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbVec2s::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "<%d, %d>", this->vec[0], this->vec[1] );
#endif // COIN_DEBUG
}
