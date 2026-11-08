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

/*!
  \class SbVec3f SbVec3f.h Inventor/SbVec3f.h
  \brief The SbVec3f class is a 3 dimensional vector with floating point coordinates.

  \ingroup coin_base

  This vector class is used by many other classes in
  Coin. It provides storage for a 3 dimensional vector
  as well as simple floating point arithmetic operations.

  \sa SbVec2s, SbVec2f, SbVec2d, SbVec3s, SbVec3d, SbVec4f, SbVec4d.
*/

#include <Inventor/SbVec3f.h>

#include <limits>
#include <cassert>

#include <Inventor/SbVec3d.h>
#include <Inventor/SbVec3b.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbVec3i32.h>

#include <Inventor/SbPlane.h>

#include <Inventor/fields/SoSFVec3f.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "tidbitsp.h" // coin_debug_normalize()
#include "coinString.h"

#include "coindefs.h"

/*!
  \fn SbVec3f::SbVec3f(void)

  The default constructor does nothing. The vector coordinates will be
  uninitialized until you do a setValue() call.
*/

/*!
  \fn SbVec3f::SbVec3f(const float v[3])

  Constructs an SbVec3f instance with initial values from \a v.
*/

/*!
  \fn SbVec3f::SbVec3f(float x, float y, float z)

  Constructs an SbVec3f instance with the initial vector endpoint set to
  \a <x,y,z>.
*/

/*!
  \fn SbVec3f::SbVec3f(const SbVec3d & v)
  Constructs an SbVec3f instance from an SbVec3d instance.
*/

/*!
  \fn SbVec3f::SbVec3f(const SbVec3b & v)
  Constructs an SbVec3f instance from an SbVec3b instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec3f::SbVec3f(const SbVec3s & v)
  Constructs an SbVec3f instance from an SbVec3s instance.

  \since Coin 2.5
*/

/*!
  \fn SbVec3f::SbVec3f(const SbVec3i32 & v)
  Constructs an SbVec3f instance from an SbVec3i32 instance.

  \since Coin 2.5
*/

/*!
  \var SbVec3f::vec[3]

  The container for all values of this class.
*/

/*!
  Constructs an SbVec3f instance from the unique intersection point of the
  three given planes.

  If the planes do not define a numerically stable, finite point, the vector
  is set to zero. Use SbPlane::intersect(const SbPlane &, const SbPlane &,
  SbVec3f &) when failure must be distinguished from a point at the origin.
*/

SbVec3f::SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2)
{
  this->setValue(0.0f, 0.0f, 0.0f);
  if (!p0.intersect(p1, p2, *this)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbVec3f::SbVec3f",
                              "The given planes do not define a unique, "
                              "numerically stable intersection point. "
                              "Using the zero vector instead.");
#endif // COIN_DEBUG
  }
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
  \fn float SbVec3f::dot(const SbVec3f & v) const

  Calculates and returns the result of taking the dot product of this
  vector and \a v.
*/

/*!
  Compares the vector with \a v and returns \c TRUE if the distance
  between the vectors is smaller or equal to the square root of
  \a tolerance.
*/
SbBool
SbVec3f::equals(const SbVec3f & v, float tolerance) const
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
  float xabs = static_cast<float>(fabs(this->vec[0]));
  float yabs = static_cast<float>(fabs(this->vec[1]));
  float zabs = static_cast<float>(fabs(this->vec[2]));

  if (xabs>=yabs && xabs>=zabs) closest[0] = (this->vec[0] > 0.0f) ? 1.0f : -1.0f;
  else if (yabs>=zabs) closest[1] = (this->vec[1] > 0.0f) ? 1.0f : -1.0f;
  else closest[2] = (this->vec[2] > 0.0f) ? 1.0f : -1.0f;

  return closest;
}

/*!
  \fn const float * SbVec3f::getValue(void) const

  Returns a pointer to an array of three floats containing the x, y
  and z coordinates of the vector.

  \sa setValue().
*/

/*!
  \fn void SbVec3f::getValue(float & x, float & y, float & z) const

  Returns the x, y and z coordinates of the vector.

  \sa setValue().
*/

/*!
  Return length of vector.
*/

float
SbVec3f::length(void) const
{
  return static_cast<float>(sqrt(this->sqrLength()));
}

/*!
  \fn float SbVec3f::sqrLength(void) const
  Returns the squared length of the vector.
*/

/*!
  \fn void SbVec3f::negate(void)
  Negate the vector (i.e. point it in the opposite direction).
*/

/*!
  Normalize the vector to unit length. Return value is the original
  length of the vector before normalization.

  If the vector is the null vector, no attempt at normalization will
  be done. If the Coin library was built in a debug version, and
  the COIN_DEBUG_NORMALIZE environment variable is set, this error
  message will then be shown:

  \verbatim
    Coin warning in SbVec3f::normalize(): The length of the vector
    should be > 0.0f to be able to normalize.
  \endverbatim

  We've made it possible for Coin to spit out a warning when an
  attempt at normalizing a null-vector is made, as that seems to
  sometimes be a symptom caused by some graver error somewhere else --
  either an internal error in Coin code, a programming error in
  application code, or an error in an input file (like for instance
  invalid polygon specifications).

  If you run into bugs/problems with your application or with Coin, it could be a
  good idea to set COIN_DEBUG_NORMALIZE=1, and then restart the
  application to see if you get any warnings from normalize().

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

  A note for developers porting code from SGI or TGS Inventor to Coin:
  those Inventor implementations are more slack about detecting and
  giving out warning messages upon API misuses, and you may suddenly
  have gotten this warning with Coin without seeing any indication of
  an error with SGI/TGS Inventor. This does \e not necessarily mean
  that it is a bug in Coin -- it is much more likely that you are
  getting a warning as an indication of API misuse or import file
  errors which were just not detected and/or reported with those
  Inventor implementations.
*/

float
SbVec3f::normalize(void)
{
  float len = this->length();

  if (len > 0.0f) {
    operator/=(len);
  }
#if COIN_DEBUG
  else if (coin_debug_normalize()) {
    SoDebugError::postWarning("SbVec3f::normalize",
                              "The length of the vector should be > 0.0f "
                              "to be able to normalize.");
  }
#endif // COIN_DEBUG

  return len;
}

/*!
  \fn SbVec3f & SbVec3f::setValue(const float v[3])

  Set new coordinates for the vector from \a v. Returns reference to
  self.

  \sa getValue().
*/

/*!
  \fn SbVec3f & SbVec3f::setValue(float x, float y, float z)

  Set new coordinates for the vector. Returns reference to self.

  \sa getValue().
*/


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

  This is a Coin extension.

  \since Coin 2.0
*/
SbVec3f &
SbVec3f::setValue(const SbVec3d & v)
{
#if COIN_DEBUG
  if (v[0] > std::numeric_limits<float>::max() || v[0] < -std::numeric_limits<float>::max() ||
      v[1] > std::numeric_limits<float>::max() || v[1] < -std::numeric_limits<float>::max() ||
      v[2] > std::numeric_limits<float>::max() || v[2] < -std::numeric_limits<float>::max()) {
    SoDebugError::postWarning("SbVec3f::setValue",
                              "The double precision vector will not fit into a "
                              "single precision vector.");
  }
#endif // COIN_DEBUG

  vec[0] = static_cast<float>(v[0]);
  vec[1] = static_cast<float>(v[1]);
  vec[2] = static_cast<float>(v[2]);
  return *this;
}

/*!
  \since Coin 2.5
  \sa getValue()
*/

SbVec3f &
SbVec3f::setValue(const SbVec3b & v)
{
  vec[0] = static_cast<float>(v[0]);
  vec[1] = static_cast<float>(v[1]);
  vec[2] = static_cast<float>(v[2]);
  return *this;
}

/*!
  \since Coin 2.5
  \sa getValue()
*/

SbVec3f &
SbVec3f::setValue(const SbVec3s & v)
{
  vec[0] = static_cast<float>(v[0]);
  vec[1] = static_cast<float>(v[1]);
  vec[2] = static_cast<float>(v[2]);
  return *this;
}

/*!
  \since Coin 2.5
  \sa getValue()
*/

SbVec3f &
SbVec3f::setValue(const SbVec3i32 & v)
{
  vec[0] = static_cast<float>(v[0]);
  vec[1] = static_cast<float>(v[1]);
  vec[2] = static_cast<float>(v[2]);
  return *this;
}

/*!
  \fn float & SbVec3f::operator[] (int i)

  Index operator. Returns modifiable x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn float SbVec3f::operator[] (int i) const
  Index operator. Returns x, y or z coordinate of vector.

  \sa getValue() and setValue().
*/

/*!
  \fn SbVec3f & SbVec3f::operator *= (float d)

  Multiply components of vector with scalar value \a d. Returns
  reference to self.
*/


/*!
  \fn SbVec3f & SbVec3f::operator /= (float d)

  Divides components of vector with scalar value \a d. Returns
  reference to self.
*/

/*!
  \fn SbVec3f & SbVec3f::operator += (const SbVec3f & u)

  Adds this vector and vector \a u. Returns reference to self.
*/


/*!
  \fn SbVec3f & SbVec3f::operator -= (const SbVec3f & u)

  Subtracts vector \a u from this vector. Returns reference to self.
*/

/*!
  \fn SbVec3f SbVec3f::operator - (void) const

  Non-destructive negation operator. Returns a new SbVec3f instance
  which points in the opposite direction of this vector.

  \sa negate().
*/

/*!
  \fn SbVec3f operator * (const SbVec3f & v, float d)
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn SbVec3f operator * (float d, const SbVec3f & v)
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  multiplied with \a d.
*/

/*!
  \fn SbVec3f operator / (const SbVec3f & v, float d)
  \relates SbVec3f

  Returns an SbVec3f instance which is the components of vector \a v
  divided on the scalar factor \a d.
*/

/*!
  \fn SbVec3f operator + (const SbVec3f & v1, const SbVec3f & v2)
  \relates SbVec3f

  Returns an SbVec3f instance which is the sum of vectors \a v1 and \a v2.
*/

/*!
  \fn SbVec3f operator - (const SbVec3f & v1, const SbVec3f & v2)
  \relates SbVec3f

  Returns an SbVec3f instance which is vector \a v2 subtracted from
  vector \a v1.
*/

/*!
  \fn int operator == (const SbVec3f & v1, const SbVec3f & v2)
  \relates SbVec3f

  Returns \a 1 if \a v1 and \a v2 are \e exactly equal, \a 0 otherwise.

  \sa equals().
*/

/*!
  \fn int operator != (const SbVec3f & v1, const SbVec3f & v2)
  \relates SbVec3f

  Returns \a 1 if \a v1 and \a v2 are not equal, \a 0 if they are equal.

  \sa equals().
*/

/*!
  Return a string representation of this object
*/
SbString
SbVec3f::toString() const
{
  return CoinInternal::ToString(*this);
}

/*!
  Convert from a string representation, return whether this is a valid conversion
*/
SbBool
SbVec3f::fromString(const SbString & str)
{
  SbBool conversionOk;
  *this = CoinInternal::FromString<SbVec3f>(str,&conversionOk);
  return conversionOk;
}

/*!
  Dump the state of this object to the \a fp file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbVec3f::print(FILE * COIN_UNUSED_ARG(fp)) const
{
#if COIN_DEBUG
  fputs(this->toString().getString(),fp);
#endif // COIN_DEBUG
}

#ifdef COIN_TEST_SUITE
#include <Inventor/SbTypeInfo.h>
#include <Inventor/SbPlane.h>

typedef SbVec3f ToTest;
BOOST_AUTO_TEST_CASE(toString) {
  ToTest val(1.0f/3,2,3);
  SbString str("0.33333334 2 3");
  BOOST_CHECK_MESSAGE(str == val.toString(),
                      std::string("Mismatch between ") +  val.toString().getString() + " and control string " + str.getString());

}

BOOST_AUTO_TEST_CASE(fromString) {
  ToTest foo;
  SbString test = "0.333333343 -2 -3.0";
  ToTest trueVal(0.333333343f,-2,-3);
  SbBool conversionOk = foo.fromString(test);
  BOOST_CHECK_MESSAGE(conversionOk && trueVal == foo,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + trueVal.toString().getString());
}

BOOST_AUTO_TEST_CASE(fromInvalidString1) {
  ToTest foo;
  SbString test = "a 2 3";
  SbBool conversionOk = foo.fromString(test);
  BOOST_CHECK_MESSAGE(conversionOk == FALSE,
                      std::string("Able to convert from ") + test.getString() + " which is not a valid " + SbTypeInfo<ToTest>::getTypeName() + " representation");
}

BOOST_AUTO_TEST_CASE(fromInvalidString2) {
  ToTest foo;
  SbString test = "1,2,3";
  SbBool conversionOk = foo.fromString(test);
  BOOST_CHECK_MESSAGE(conversionOk == FALSE,
                      std::string("Able to convert from ") + test.getString() + " which is not a valid " + SbTypeInfo<ToTest>::getTypeName() + " representation");
}

BOOST_AUTO_TEST_CASE(threePlaneConstructorUsesIntersectionOrZeroFallback) {
  const SbVec3f expected(2.0f, -3.0f, 5.0f);
  const SbPlane xplane(SbVec3f(1.0f, 0.0f, 0.0f), expected);
  const SbPlane yplane(SbVec3f(0.0f, 1.0f, 0.0f), expected);
  const SbPlane zplane(SbVec3f(0.0f, 0.0f, 1.0f), expected);
  const SbVec3f unique(xplane, yplane, zplane);
  BOOST_CHECK_MESSAGE(unique == expected,
                      "Legacy constructor must use the unique intersection");

  const SbPlane dependent(SbVec3f(1.0f, 1.0f, 0.0f), expected);
  const SbVec3f fallback(xplane, yplane, dependent);
  BOOST_CHECK_MESSAGE(fallback == SbVec3f(0.0f, 0.0f, 0.0f),
                      "Legacy constructor must use zero on failure");
}

#endif //COIN_TEST_SUITE
