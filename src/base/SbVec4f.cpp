/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SbVec4f SbLinear.h Inventor/SbLinear.h
  \brief The SbVec4f class is a 4 dimensional vector with floating point coordinates.
  \ingroup base

  This vector class is used by many other classes in
  Coin. It provides storage for a 3 dimensional homogeneoues
  vector (with the 4 components usually referred to as <x, y, z, w>)
  aswell as simple floating point arithmetic operations.

  \sa SbVec2f, SbVec2s, SbVec3f, SbVec3s.
*/


#include <assert.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec3f.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  The default constructor does nothing. The vector coordinates will be
  uninitialized until a call the setValue().
*/
SbVec4f::SbVec4f(void)
{
}

/*!
  Constructs an SbVec4f instance with initial values from \a v.
 */
SbVec4f::SbVec4f(const float v[4])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
  this->vec[3] = v[3];
}

/*!
  Constructs an SbVec4f instance with the initial homogeneous vector
  set to \a <x,y,z,w>.
 */
SbVec4f::SbVec4f(const float x, const float y, const float z, const float w)
{
  this->vec[0] = x;
  this->vec[1] = y;
  this->vec[2] = z;
  this->vec[3] = w;
}

/*!
  Calculates and returns the result of taking the dot product of this
  vector and \a v.
 */
float
SbVec4f::dot(const SbVec4f& v) const
{
  return vec[0]*v.vec[0] + vec[1]*v.vec[1] + vec[2]*v.vec[2] + vec[3]*v.vec[3];
}

/*!
  Compares the vector with \a v and returns TRUE if the largest distance
  between the vectors are larger than the square root of the given
  tolerance value.

  The comparison is done in 4D-space, i.e. the \a w component of the
  vector is \e not used to make x, y and z into Cartesian coordinates
  first.
 */
SbBool
SbVec4f::equals(const SbVec4f& v, const float tolerance) const
{
#if COIN_DEBUG
  if(!(tolerance >= 0.0f))
    SoDebugError::postWarning("SbVec4f::equals","Tolerance should be >= 0.0f");
#endif // COIN_DEBUG

  float xdist = this->vec[0] - v[0];
  float ydist = this->vec[1] - v[1];
  float zdist = this->vec[2] - v[2];
  float wdist = this->vec[3] - v[3];

  if((xdist*xdist + ydist*ydist + zdist*zdist + wdist*wdist) <= tolerance)
    return TRUE;
  return FALSE;
}

/*!
  Returns the vector as a Cartesian 3D vector in \a v. This means that
  the 3 first components x, y and z will be divided by the fourth, w.
*/
void
SbVec4f::getReal(SbVec3f & v) const
{
#if COIN_DEBUG
  if(!(this->vec[3] != 0.0f))
    SoDebugError::postWarning("SbVec4f::getReal",
                              "The 4th vector component is zero => "
                              "division by zero");
#endif // COIN_DEBUG

  v[0] = this->vec[0]/this->vec[3];
  v[1] = this->vec[1]/this->vec[3];
  v[2] = this->vec[2]/this->vec[3];
}

/*!
  Returns a pointer to an array of four floats containing the
  x, y, z and w coordinates of the vector.

  \sa setValue().
 */
const float *
SbVec4f::getValue(void) const
{
  return this->vec;
}

/*!
  Returns the x, y, z and w coordinates of the vector.

  \sa setValue().
 */
void
SbVec4f::getValue(float& x, float& y, float& z, float& w) const
{
  x = this->vec[0];
  y = this->vec[1];
  z = this->vec[2];
  w = this->vec[3];
}

/*!
  Return the length of the vector in 4D space.
 */
float
SbVec4f::length(void) const
{
  return (float)sqrt(vec[0]*vec[0] + vec[1]*vec[1] +
                     vec[2]*vec[2] + vec[3]*vec[3]);
}

/*!
  Negate the vector.
 */
void
SbVec4f::negate(void)
{
  this->vec[0] = -this->vec[0];
  this->vec[1] = -this->vec[1];
  this->vec[2] = -this->vec[2];
  this->vec[3] = -this->vec[3];
}

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.
 */
float
SbVec4f::normalize(void)
{
  float len = this->length();

  if (len > 0.0f) {
    operator/=(len);
  }
#if COIN_DEBUG
  else {
    SoDebugError::postWarning("SbVec4f::normalize",
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
SbVec4f&
SbVec4f::setValue(const float v[4])
{
  this->vec[0] = v[0];
  this->vec[1] = v[1];
  this->vec[2] = v[2];
  this->vec[3] = v[3];
  return *this;
}

/*!
  Set new coordinates for the vector. Returns reference to self.

  \sa getValue().
 */
SbVec4f&
SbVec4f::setValue(const float x, const float y, const float z, const float w)
{
  this->vec[0] = x;
  this->vec[1] = y;
  this->vec[2] = z;
  this->vec[3] = w;
  return *this;
}

/*!
  Index operator. Returns modifiable x, y, z or w component of vector.

  \sa getValue() and setValue().
 */
float&
SbVec4f::operator [](const int i)
{
#if COIN_DEBUG
  if(!(i>=0 && i<=3))
    SoDebugError::postWarning("SbVec4f::operator[]",
                              "Index out of bounds [0..3].");
#endif // COIN_DEBUG

  return this->vec[i];
}

/*!
  Index operator. Returns x, y, z or w component of vector.

  \sa getValue() and setValue().
 */
const float&
SbVec4f::operator [](const int i) const
{
#if COIN_DEBUG
  if(!(i>=0 && i<=3))
    SoDebugError::postWarning("SbVec4f::operator[]",
                              "Index out of bounds [0..3].");
#endif // COIN_DEBUG

  return this->vec[i];
}

/*!
  Multiply components of vector with value \a d. Returns reference to self.
 */
SbVec4f&
SbVec4f::operator *=(const float d)
{
  this->vec[0] *= d;
  this->vec[1] *= d;
  this->vec[2] *= d;
  this->vec[3] *= d;
  return *this;
}

/*!
  Divides components of vector with value \a d. Returns reference to self.
 */
SbVec4f&
SbVec4f::operator /=(const float d)
{
#if COIN_DEBUG
  if(!(d != 0.0f))
    SoDebugError::postWarning("SbVec4f::operator/=",
                              "Division by zero.");
#endif // COIN_DEBUG

  // Assumes 1 div and 4 muls is quicker than 4 divs.
  float inv = 1.0f/d;
  this->vec[0] *= inv;
  this->vec[1] *= inv;
  this->vec[2] *= inv;
  this->vec[3] *= inv;
  return *this;
}

/*!
  Adds this vector and vector \a u. Returns reference to self.
 */
SbVec4f&
SbVec4f::operator +=(const SbVec4f& u)
{
  this->vec[0] += u.vec[0];
  this->vec[1] += u.vec[1];
  this->vec[2] += u.vec[2];
  this->vec[3] += u.vec[3];
  return *this;
}

/*!
  Subtracts vector \a u from this vector. Returns reference to self.
 */
SbVec4f&
SbVec4f::operator -=(const SbVec4f& u)
{
  this->vec[0] -= u.vec[0];
  this->vec[1] -= u.vec[1];
  this->vec[2] -= u.vec[2];
  this->vec[3] -= u.vec[3];
  return *this;
}

/*!
  Non-destructive negation operator. Returns a new SbVec4f instance which
  has all components negated.

  \sa negate().
 */
SbVec4f
SbVec4f::operator -(void) const
{
  return SbVec4f(-this->vec[0], -this->vec[1], -this->vec[2], -this->vec[3]);
}

/*!
  \relates SbVec4f

  Returns an SbVec4f instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec4f
operator *(const SbVec4f& v, const float d)
{
  return SbVec4f(v.vec[0] * d, v.vec[1] * d, v.vec[2] * d, v.vec[3] * d);
}

/*!
  \relates SbVec4f

  Returns an SbVec4f instance which is the components of vector \a v
  multiplied with \a d.
 */
SbVec4f
operator *(const float d, const SbVec4f& v)
{
  return v*d;
}

/*!
  \relates SbVec4f

  Returns an SbVec4f instance which is the components of vector \a v
  divided on the scalar factor \a d.
 */
SbVec4f
operator /(const SbVec4f& v, const float d)
{
#if COIN_DEBUG
  if(!(d != 0.0f))
    SoDebugError::postWarning("SbVec4f::operator/",
                              "Division by zero.");
#endif // COIN_DEBUG

  return SbVec4f(v.vec[0] / d, v.vec[1] / d, v.vec[2] / d, v.vec[3] / d);
}

/*!
  \relates SbVec4f

  Returns an SbVec4f instance which is the sum of vectors \a v1 and \a v2.
 */
SbVec4f
operator +(const SbVec4f& v1, const SbVec4f& v2)
{
  return SbVec4f(v1.vec[0] + v2.vec[0],
                 v1.vec[1] + v2.vec[1],
                 v1.vec[2] + v2.vec[2],
                 v1.vec[3] + v2.vec[3]);
}

/*!
  \relates SbVec4f

  Returns an SbVec4f instance which is vector \a v2 subtracted from
  vector \a v1.
 */
SbVec4f
operator -(const SbVec4f& v1, const SbVec4f& v2)
{
  return SbVec4f(v1.vec[0] - v2.vec[0],
                 v1.vec[1] - v2.vec[1],
                 v1.vec[2] - v2.vec[2],
                 v1.vec[3] - v2.vec[3]);
}

/*!
  \relates SbVec4f

  Returns \a 1 if \a v1 and \a v2 are equal, \a 0 otherwise.

  \sa equals().
 */
int
operator ==(const SbVec4f& v1, const SbVec4f& v2)
{
  if(v1.vec[0] == v2.vec[0] &&
     v1.vec[1] == v2.vec[1] &&
     v1.vec[2] == v2.vec[2] &&
     v1.vec[3] == v2.vec[3]) return TRUE;
  return FALSE;
}

/*!
  \relates SbVec4f

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
 */
int
operator !=(const SbVec4f& v1, const SbVec4f& v2)
{
  return !(v1 == v2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbVec4f::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "<%f, %f, %f, %f>", this->vec[0], this->vec[1], this->vec[2],
    this->vec[3] );
#endif // COIN_DEBUG
}
