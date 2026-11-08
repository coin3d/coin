/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <Inventor/SbVec3d.h>

#include <limits>
#include <cassert>

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3b.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbVec3i32.h>
#include <Inventor/SbDPPlane.h>
#include <Inventor/fields/SoSFVec3d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "tidbitsp.h" // coin_debug_normalize()
#include "coinString.h"

#include "coindefs.h"

/*!
  \class SbVec3d SbVec3d.h Inventor/SbVec3d.h
  \brief The SbVec3d class is a 3 dimensional vector with double precision floating point coordinates.

  \ingroup coin_base

  This vector class provides storage for a 3 dimensional double
  precision floating point vector as well as simple floating point
  arithmetic operations.

  \sa SbVec2s, SbVec2f, SbVec2d, SbVec3s, SbVec3f, SbVec4f, SbVec4d.
  \since Coin 2.0
  \since TGS Inventor 2.6
*/

/*!
  \fn SbVec3d::SbVec3d(void)

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  \fn SbVec3d::SbVec3d(const double v[3])

  Constructs an SbVec3d instance with initial values from \a v.
*/

/*!
  \fn SbVec3d::SbVec3d(double x, double y, double z)

  Constructs an SbVec3d instance with the initial vector endpoint set to
  \a <x,y,z>.
*/

/*!
  \fn SbVec3d::SbVec3d(const SbVec3f & v)

  Constructs an SbVec3d instance from an SbVec3f instance.
*/

/*!
  \fn SbVec3d::SbVec3d(const SbVec3b & v)

  Constructs an SbVec3d instance from an SbVec3b instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec3d::SbVec3d(const SbVec3s & v)

  Constructs an SbVec3d instance from an SbVec3s instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec3d::SbVec3d(const SbVec3i32 & v)

  Constructs an SbVec3d instance from an SbVec3i32 instance.

  \since Coin 2.5
*/

/*!
  Constructs an SbVec3d instance from the unique intersection point of the
  three given planes.

  If the planes do not define a numerically stable, finite point, the vector
  is set to zero. Use SbDPPlane::intersect(const SbDPPlane &,
  const SbDPPlane &, SbVec3d &) when failure must be distinguished from a
  point at the origin.
*/

SbVec3d::SbVec3d(const SbDPPlane & p0, const SbDPPlane & p1, const SbDPPlane & p2)
{
  this->setValue(0.0, 0.0, 0.0);
  if (!p0.intersect(p1, p2, *this)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbVec3d::SbVec3d",
                              "The given planes do not define a unique, "
                              "numerically stable intersection point. "
                              "Using the zero vector instead.");
#endif // COIN_DEBUG
  }
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
  \fn double SbVec3d::dot(const SbVec3d & v) const

  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/

/*!
  Compares the vector with \a v and returns \c TRUE if the distance
  between the vectors is smaller or equal to the square root of
  \a tolerance.
*/
SbBool
SbVec3d::equals(const SbVec3d & v, double tolerance) const
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
  double xabs = static_cast<double>(fabs(this->vec[0]));
  double yabs = static_cast<double>(fabs(this->vec[1]));
  double zabs = static_cast<double>(fabs(this->vec[2]));

  if (xabs>=yabs && xabs>=zabs) closest[0] = (this->vec[0] > 0.0) ? 1.0 : -1.0;
  else if (yabs>=zabs) closest[1] = (this->vec[1] > 0.0) ? 1.0 : -1.0;
  else closest[2] = (this->vec[2] > 0.0) ? 1.0 : -1.0;

  return closest;
}

/*!
  \fn const double * SbVec3d::getValue(void) const

  Returns a pointer to an array of three doubles containing the x, y
  and z coordinates of the vector.

  \sa setValue().
*/

/*!
  \fn void SbVec3d::getValue(double & x, double & y, double & z) const

  Returns the x, y and z coordinates of the vector.

  \sa setValue().
*/

/*!
  Return length of vector.
*/
double
SbVec3d::length(void) const
{
  return static_cast<double>(sqrt(this->sqrLength()));
}

/*!
  \fn double SbVec3d::sqrLength(void) const

  Returns the squared length of the vector.
*/

/*!
  \fn void SbVec3d::negate(void)

  Negate the vector (i.e. point it in the opposite direction).
*/

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
  \fn SbVec3d & SbVec3d::setValue(const double v[3])

  Set new coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
*/

/*!
  \fn SbVec3d & SbVec3d::setValue(double x, double y, double z)

  Set new coordinates for the vector. Returns reference to self.

  \sa getValue().
*/

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
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  vec[2] = static_cast<double>(v[2]);
  return *this;
}

/*!
  Sets this vector to the vector \a v.

  \since Coin 2.5
*/

SbVec3d &
SbVec3d::setValue(const SbVec3b & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  vec[2] = static_cast<double>(v[2]);
  return *this;
}

/*!
  Sets this vector to the vector \a v.

  \since Coin 2.5
*/

SbVec3d &
SbVec3d::setValue(const SbVec3s & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  vec[2] = static_cast<double>(v[2]);
  return *this;
}

/*!
  Sets this vector to the vector \a v.

  \since Coin 2.5
*/

SbVec3d &
SbVec3d::setValue(const SbVec3i32 & v)
{
  vec[0] = static_cast<double>(v[0]);
  vec[1] = static_cast<double>(v[1]);
  vec[2] = static_cast<double>(v[2]);
  return *this;
}

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

/*!
  \fn SbVec3d & SbVec3d::operator *= (double d)

  Multiply components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn SbVec3d & SbVec3d::operator /= (const double d)

  Divides components of vector with value \a d. Returns reference to self.
*/

/*!
  \fn SbVec3d & SbVec3d::operator += (const SbVec3d & v)

  Adds this vector and vector \a v. Returns reference to self.
*/

/*!
  \fn SbVec3d & SbVec3d::operator -=(const SbVec3d & v)

  Subtracts vector \a v from this vector. Returns reference to self.
*/

/*!
  \fn SbVec3d SbVec3d::operator - (void) const
  Non-destructive negation operator. Returns a new SbVec3d instance
  which points in the opposite direction of this vector.

  \sa negate().
*/

/*!
  \fn SbVec3d operator *(const SbVec3d & v, double d)
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn SbVec3d operator * (double d, const SbVec3d & v)
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn SbVec3d operator / (const SbVec3d & v, double d)
  \relates SbVec3d

  Returns an SbVec3d instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/

/*!
  \fn SbVec3d operator + (const SbVec3d & v1, const SbVec3d & v2)
  \relates SbVec3d

  Returns an SbVec3d instance which is the sum of vectors \a v1 and \a v2.
*/

/*!
  \fn SbVec3d operator - (const SbVec3d & v1, const SbVec3d & v2)
  \relates SbVec3d

  Returns an SbVec3d instance which is vector \a v2 subtracted from
  vector \a v1.
*/

/*!
  \fn int operator == (const SbVec3d & v1, const SbVec3d & v2)
  \relates SbVec3d

  Returns \a 1 if \a v1 and \a v2 are \e exactly equal, \a 0 otherwise.

  \sa equals().
*/

/*!
  \fn int operator != (const SbVec3d & v1, const SbVec3d & v2)
  \relates SbVec3d

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/

/*!
  Return a string representation of this object
*/
SbString
SbVec3d::toString() const
{
  return CoinInternal::ToString(*this);
}

/*!
  Convert from a string representation, return whether this is a valid conversion
*/
SbBool
SbVec3d::fromString(const SbString & str)
{
  SbBool conversionOk;
  *this = CoinInternal::FromString<SbVec3d>(str,&conversionOk);
  return conversionOk;
}

/*!
  Dump the state of this object to the \a fp file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbVec3d::print(FILE * COIN_UNUSED_ARG(fp)) const
{
#if COIN_DEBUG
  fputs(this->toString().getString(),fp);
#endif // COIN_DEBUG
}

#ifdef COIN_TEST_SUITE
#include <Inventor/SbDPPlane.h>

typedef SbVec3d ToTest;
BOOST_AUTO_TEST_CASE(toString) {
  ToTest val(1.0/3,2,3);
  SbString str("0.3333333333333333 2 3");
  BOOST_CHECK_MESSAGE(str == val.toString(),
                      std::string("Mismatch between ") +  val.toString().getString() + " and control string " + str.getString());

}

BOOST_AUTO_TEST_CASE(fromString) {
  ToTest foo;
  SbString test = "0.3333333333333333 -2 -3.0";
  ToTest trueVal(0.3333333333333333,-2,-3);
  SbBool conversionOk = foo.fromString(test);
  BOOST_CHECK_MESSAGE(conversionOk && trueVal == foo,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + trueVal.toString().getString());
}

BOOST_AUTO_TEST_CASE(threePlaneConstructorUsesIntersectionOrZeroFallback) {
  const SbVec3d expected(2.0, -3.0, 5.0);
  const SbDPPlane xplane(SbVec3d(1.0, 0.0, 0.0), expected);
  const SbDPPlane yplane(SbVec3d(0.0, 1.0, 0.0), expected);
  const SbDPPlane zplane(SbVec3d(0.0, 0.0, 1.0), expected);
  const SbVec3d unique(xplane, yplane, zplane);
  BOOST_CHECK_MESSAGE(unique == expected,
                      "Legacy constructor must use the unique intersection");

  const SbDPPlane dependent(SbVec3d(1.0, 1.0, 0.0), expected);
  const SbVec3d fallback(xplane, yplane, dependent);
  BOOST_CHECK_MESSAGE(fallback == SbVec3d(0.0, 0.0, 0.0),
                      "Legacy constructor must use zero on failure");
}

#endif //COIN_TEST_SUITE
