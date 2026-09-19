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
  \class SbDPPlane SbDPPlane.h Inventor/SbLinear.h
  \brief The SbDPPlane class represents a plane in 3D space.

  \ingroup coin_base

  SbDPPlane is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.

  \COIN_CLASS_EXTENSION

  \since Coin 2.0
*/

#include <cassert>
#include <cstdio>
#include <Inventor/SbDPPlane.h>
#include <Inventor/SbDPLine.h>
#include <Inventor/SbDPMatrix.h>
#include <cfloat>
#include <cmath>
#include <limits>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "coindefs.h"

struct coin_sbdpplane_data {
  SbVec3d normal;
  double distance;
};

static SbBool
coin_sbdpplane_less(const coin_sbdpplane_data & lhs,
                    const coin_sbdpplane_data & rhs)
{
  for (int i = 0; i < 3; ++i) {
    if (lhs.normal[i] < rhs.normal[i]) return TRUE;
    if (lhs.normal[i] > rhs.normal[i]) return FALSE;
  }
  return lhs.distance < rhs.distance;
}

static void
coin_sbdpplane_sort(coin_sbdpplane_data planes[3])
{
  for (int i = 1; i < 3; ++i) {
    const coin_sbdpplane_data current = planes[i];
    int j = i;
    while (j > 0 && coin_sbdpplane_less(current, planes[j - 1])) {
      planes[j] = planes[j - 1];
      --j;
    }
    planes[j] = current;
  }
}

struct coin_sbdpplane_scaled {
  double fraction;
  int exponent;
};

static coin_sbdpplane_scaled
coin_sbdpplane_scaled_product(const double lhs, const double rhs)
{
  if (lhs == 0.0 || rhs == 0.0) {
    const coin_sbdpplane_scaled zero = { 0.0, 0 };
    return zero;
  }

  int lhsexponent;
  int rhsexponent;
  int adjustment;
  const double lhsfraction = std::frexp(lhs, &lhsexponent);
  const double rhsfraction = std::frexp(rhs, &rhsexponent);
  const double product = lhsfraction * rhsfraction;
  const coin_sbdpplane_scaled result = {
    std::frexp(product, &adjustment),
    lhsexponent + rhsexponent + adjustment
  };
  return result;
}

static coin_sbdpplane_scaled
coin_sbdpplane_scaled_add(coin_sbdpplane_scaled lhs,
                          coin_sbdpplane_scaled rhs)
{
  if (lhs.fraction == 0.0) return rhs;
  if (rhs.fraction == 0.0) return lhs;
  if (lhs.exponent < rhs.exponent) {
    const coin_sbdpplane_scaled tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }

  const double fraction = lhs.fraction +
                          std::ldexp(rhs.fraction,
                                     rhs.exponent - lhs.exponent);
  if (fraction == 0.0) {
    const coin_sbdpplane_scaled zero = { 0.0, 0 };
    return zero;
  }

  int adjustment;
  const coin_sbdpplane_scaled result = {
    std::frexp(fraction, &adjustment),
    lhs.exponent + adjustment
  };
  return result;
}

static SbBool
coin_sbdpplane_scaled_magnitude_greater(
  const coin_sbdpplane_scaled & lhs,
  const coin_sbdpplane_scaled & rhs)
{
  if (lhs.fraction == 0.0) return FALSE;
  if (rhs.fraction == 0.0) return TRUE;
  if (lhs.exponent != rhs.exponent) return lhs.exponent > rhs.exponent;
  return std::fabs(lhs.fraction) > std::fabs(rhs.fraction);
}

static coin_sbdpplane_scaled
coin_sbdpplane_scaled_add3(const coin_sbdpplane_scaled values[3])
{
  int positive = -1;
  int negative = -1;
  for (int i = 0; i < 3; ++i) {
    if (values[i].fraction > 0.0 &&
        (positive == -1 ||
         coin_sbdpplane_scaled_magnitude_greater(values[i],
                                                 values[positive]))) {
      positive = i;
    }
    else if (values[i].fraction < 0.0 &&
             (negative == -1 ||
              coin_sbdpplane_scaled_magnitude_greater(values[i],
                                                      values[negative]))) {
      negative = i;
    }
  }

  // Combine the largest opposite-sign terms first. This lets a small third
  // term survive when the large terms cancel.
  if (positive != -1 && negative != -1) {
    const int remaining = 3 - positive - negative;
    return coin_sbdpplane_scaled_add(
      coin_sbdpplane_scaled_add(values[positive], values[negative]),
      values[remaining]);
  }

  int order[3] = { 0, 1, 2 };
  for (int i = 1; i < 3; ++i) {
    const int current = order[i];
    int j = i;
    while (j > 0 &&
           coin_sbdpplane_scaled_magnitude_greater(values[order[j - 1]],
                                                   values[current])) {
      order[j] = order[j - 1];
      --j;
    }
    order[j] = current;
  }
  return coin_sbdpplane_scaled_add(
    coin_sbdpplane_scaled_add(values[order[0]], values[order[1]]),
    values[order[2]]);
}

static coin_sbdpplane_scaled
coin_sbdpplane_scaled_divide(coin_sbdpplane_scaled value,
                             const double divisor)
{
  if (value.fraction == 0.0) return value;

  int divisorexponent;
  int adjustment;
  const double divisorfraction = std::frexp(divisor, &divisorexponent);
  const double quotient = value.fraction / divisorfraction;
  value.fraction = std::frexp(quotient, &adjustment);
  value.exponent += adjustment - divisorexponent;
  return value;
}

static SbBool
coin_sbdpplane_direct_product(const double lhs, const double rhs,
                              double & result)
{
  if (!std::isfinite(lhs) || !std::isfinite(rhs)) return FALSE;
  if (lhs == 0.0 || rhs == 0.0) {
    result = 0.0;
    return TRUE;
  }

  const double abslhs = std::fabs(lhs);
  const double absrhs = std::fabs(rhs);
  if (abslhs > DBL_MAX / absrhs || abslhs < DBL_MIN / absrhs) {
    return FALSE;
  }

  const double product = lhs * rhs;
  if (!std::isfinite(product) || product == 0.0 ||
      std::fpclassify(product) == FP_SUBNORMAL) {
    return FALSE;
  }
  result = product;
  return TRUE;
}

static SbBool
coin_sbdpplane_direct_add(const double lhs, const double rhs,
                          double & result)
{
  if (!std::isfinite(lhs) || !std::isfinite(rhs)) return FALSE;
  if (lhs == 0.0) {
    result = rhs == 0.0 ? 0.0 : rhs;
    return TRUE;
  }
  if (rhs == 0.0) {
    result = lhs;
    return TRUE;
  }
  if ((lhs > 0.0 && rhs > 0.0 && lhs > DBL_MAX - rhs) ||
      (lhs < 0.0 && rhs < 0.0 && lhs < -DBL_MAX - rhs)) {
    return FALSE;
  }

  const double sum = lhs + rhs;
  if (!std::isfinite(sum) || std::fpclassify(sum) == FP_SUBNORMAL) {
    return FALSE;
  }
  if (sum == 0.0) {
    if (lhs != -rhs) return FALSE;
    result = 0.0;
    return TRUE;
  }
  result = sum;
  return TRUE;
}

static void
coin_sbdpplane_two_sum(const double lhs, const double rhs,
                       double & sum, double & error)
{
  sum = lhs + rhs;
  const double rhsvirtual = sum - lhs;
  const double lhsvirtual = sum - rhsvirtual;
  error = (lhs - lhsvirtual) + (rhs - rhsvirtual);
}

static SbBool
coin_sbdpplane_magnitude_sum_exceeds_max(const double values[3],
                                         const SbBool positive)
{
  double expansion[4] = { DBL_MAX, 0.0, 0.0, 0.0 };
  int expansionlength = 1;
  for (int valueidx = 0; valueidx < 3; ++valueidx) {
    if ((positive && values[valueidx] <= 0.0) ||
        (!positive && values[valueidx] >= 0.0)) {
      continue;
    }

    double next[4];
    int nextlength = 0;
    double accumulator = -std::fabs(values[valueidx]);
    for (int i = 0; i < expansionlength; ++i) {
      double sum;
      double error;
      coin_sbdpplane_two_sum(accumulator, expansion[i], sum, error);
      if (error != 0.0) next[nextlength++] = error;
      accumulator = sum;
    }
    if (accumulator != 0.0 || nextlength == 0) {
      next[nextlength++] = accumulator;
    }
    expansionlength = nextlength;
    for (int i = 0; i < expansionlength; ++i) expansion[i] = next[i];

    for (int i = expansionlength - 1; i >= 0; --i) {
      if (expansion[i] < 0.0) return TRUE;
      if (expansion[i] > 0.0) break;
    }
  }
  return FALSE;
}

static SbBool
coin_sbdpplane_direct_add3(const double values[3], double & result)
{
  int positive = -1;
  int negative = -1;
  SbBool positiveneedscheck = FALSE;
  SbBool negativeneedscheck = FALSE;
  for (int i = 0; i < 3; ++i) {
    const double magnitude = std::fabs(values[i]);
    if (values[i] > 0.0) {
      if (magnitude > DBL_MAX * 0.25) positiveneedscheck = TRUE;
      if (positive == -1 ||
          magnitude > std::fabs(values[positive])) {
        positive = i;
      }
    }
    else if (values[i] < 0.0) {
      if (magnitude > DBL_MAX * 0.25) negativeneedscheck = TRUE;
      if (negative == -1 ||
          magnitude > std::fabs(values[negative])) {
        negative = i;
      }
    }
  }

  if ((positiveneedscheck &&
       coin_sbdpplane_magnitude_sum_exceeds_max(values, TRUE)) ||
      (negativeneedscheck &&
       coin_sbdpplane_magnitude_sum_exceeds_max(values, FALSE))) {
    return FALSE;
  }

  double partial;
  if (positive != -1 && negative != -1) {
    const int remaining = 3 - positive - negative;
    return coin_sbdpplane_direct_add(values[positive], values[negative],
                                     partial) &&
           coin_sbdpplane_direct_add(partial, values[remaining], result);
  }

  int order[3] = { 0, 1, 2 };
  for (int i = 1; i < 3; ++i) {
    const int current = order[i];
    int j = i;
    while (j > 0 &&
           std::fabs(values[order[j - 1]]) > std::fabs(values[current])) {
      order[j] = order[j - 1];
      --j;
    }
    order[j] = current;
  }
  return coin_sbdpplane_direct_add(values[order[0]], values[order[1]],
                                   partial) &&
         coin_sbdpplane_direct_add(partial, values[order[2]], result);
}

static SbBool
coin_sbdpplane_direct_divide(const double numerator,
                             const double denominator,
                             double & result)
{
  if (!std::isfinite(numerator) || !std::isfinite(denominator) ||
      denominator == 0.0) {
    return FALSE;
  }
  if (numerator == 0.0) {
    result = 0.0;
    return TRUE;
  }

  const double absnumerator = std::fabs(numerator);
  const double absdenominator = std::fabs(denominator);
  if ((absdenominator < 1.0 &&
       absnumerator > DBL_MAX * absdenominator) ||
      (absdenominator > 1.0 &&
       absnumerator < DBL_MIN * absdenominator)) {
    return FALSE;
  }

  const double quotient = numerator / denominator;
  if (!std::isfinite(quotient) || quotient == 0.0 ||
      std::fpclassify(quotient) == FP_SUBNORMAL) {
    return FALSE;
  }
  result = quotient;
  return TRUE;
}

static SbBool
coin_sbdpplane_direct_coordinate(const double distances[3],
                                 const SbVec3d cofactors[3],
                                 const int component,
                                 const double determinant,
                                 double & result)
{
  double terms[3];
  for (int i = 0; i < 3; ++i) {
    if (!coin_sbdpplane_direct_product(distances[i],
                                       cofactors[i][component], terms[i])) {
      return FALSE;
    }
  }

  double numerator;
  return coin_sbdpplane_direct_add3(terms, numerator) &&
         coin_sbdpplane_direct_divide(numerator, determinant, result);
}

static SbBool
coin_sbdpplane_scaled_coordinate(const double distances[3],
                                 const SbVec3d cofactors[3],
                                 const int component,
                                 const double determinant,
                                 double & result)
{
  coin_sbdpplane_scaled terms[3];
  for (int i = 0; i < 3; ++i) {
    terms[i] = coin_sbdpplane_scaled_product(distances[i],
                                             cofactors[i][component]);
  }

  const coin_sbdpplane_scaled numerator =
    coin_sbdpplane_scaled_add3(terms);
  const coin_sbdpplane_scaled coordinate =
    coin_sbdpplane_scaled_divide(numerator, determinant);
  const double value = std::ldexp(coordinate.fraction,
                                  coordinate.exponent);
  if (!std::isfinite(value)) return FALSE;
  result = value == 0.0 ? 0.0 : value;
  return TRUE;
}


/*!
  An SbDPPlane instantiated with the default constructor will be
  uninitialized.
*/
SbDPPlane::SbDPPlane(void)
{
}

/*!
  Construct an SbDPPlane instance with a normal pointing in the given
  direction and the given shortest distance from the origin of the
  coordinate system to a point in the plane.

  \a normal must not be a null vector.
*/
SbDPPlane::SbDPPlane(const SbVec3d & normalref, const double D)
{
#if COIN_DEBUG
  if(!(normalref.length() != 0.0f))
    SoDebugError::postWarning("SbDPPlane::SbDPPlane",
                              "Plane normal vector is a null vector.");
#endif // COIN_DEBUG

  this->normal = normalref;
  // we test for a null vector above, just normalize
  (void) this->normal.normalize();
  this->distance = D;
}

/*!
  Construct an SbDPPlane with three points laying in the plane.  Make
  sure \a p0, \a p1 and \a p2 are actually three distinct points when
  using this constructor.
*/
SbDPPlane::SbDPPlane(const SbVec3d & p0, const SbVec3d & p1, const SbVec3d & p2)
{
#if COIN_DEBUG
  if(!(p0 != p1 && p1 != p2 && p0 != p2))
    SoDebugError::postWarning("SbDPPlane::SbDPPlane",
                              "The three points defining the plane cannot "
                              "be coincident.");
#endif // COIN_DEBUG

  this->normal = (p1 - p0).cross(p2 - p0);

  // we test and warn about a null vector above
  (void) this->normal.normalize();

  //     N dot point
  // d = -------, |N| == 1
  //       |N|^2

  this->distance = this->normal.dot(p0);
}

/*!
  Construct an SbDPPlane from a normal and a point laying in the plane.

  \a normal must not be a null vector.
*/
SbDPPlane::SbDPPlane(const SbVec3d & normalref, const SbVec3d & point)
{
#if COIN_DEBUG
  if(!(normalref.length() != 0.0f))
    SoDebugError::postWarning("SbDPPlane::SbDPPlane",
                              "Plane normal vector is a null vector.");
#endif // COIN_DEBUG

  this->normal = normalref;
  // we test and warn about a null vector above
  (void) this->normal.normalize();

  //     N dot point
  // d = -------, |N| == 1
  //       |N|^2

  this->distance = this->normal.dot(point);
}


/*!
  Add the given offset \a d to the plane distance from the origin.
*/
void
SbDPPlane::offset(const double d)
{
  this->distance += d;
}

/*!
  Find the point on given line \a l intersecting the plane and return
  it in \a intersection. If the line is parallel to the plane,
  we return \c FALSE, otherwise \c TRUE.

  Do not pass an invalid line for the \a l parameter (i.e. with a
  null direction vector).
*/
SbBool
SbDPPlane::intersect(const SbDPLine & l, SbVec3d & intersection) const
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbDPPlane::intersect",
                              "Intersecting line doesn't have a direction.");
#endif // COIN_DEBUG

  // Check if the line is parallel to the plane.
  if(fabs(l.getDirection().dot(this->normal)) < DBL_EPSILON) return FALSE;

  // From the discussion on SbDPLine::getClosestPoint() we know that
  // any point on the line can be expressed as:
  //                    Q = P + t*D    (1)
  //
  // We can also easily see that a point must satisfy this equation to lie
  // in the plane:
  //                    N dot (Q - d*N) = 0, where N is the normal vector,
  //                                     Q is the point and d the offset
  //                                     from the origin.
  //
  // Combining these two equations and simplifying we get:
  //
  //                          d*|N|^2 - N dot P
  //                    t = ----------------, |N| == 1
  //                               N dot D
  //
  // Substituting t back in (1), we've solved the problem.
  //                                                         19980816 mortene.

  double t =
    (this->distance - this->normal.dot(l.getPosition()))
    / this->normal.dot(l.getDirection());

  intersection = l.getPosition() + t * l.getDirection();

  return TRUE;
}

/*!
  Transform the plane by \a matrix.

  \sa offset()
*/
void
SbDPPlane::transform(const SbDPMatrix & matrix)
{
  SbVec3d ptInPlane = this->normal * this->distance;

  // according to discussions on comp.graphics.algorithms, the inverse
  // transpose matrix should be used to rotate the plane normal.
  SbDPMatrix invtransp = matrix.inverse().transpose();
  invtransp.multDirMatrix(this->normal, this->normal);

  // the point should be transformed using the original matrix
  matrix.multVecMatrix(ptInPlane, ptInPlane);

  if (this->normal.normalize() == 0.0f) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbDPPlane::transform",
                              "The transformation invalidated the plane.");
#endif // COIN_DEBUG
  }
  this->distance = this->normal.dot(ptInPlane);
}

/*!
  Check if the given point lies in the halfspace of the plane which the
  plane normal vector is pointing.
*/
SbBool
SbDPPlane::isInHalfSpace(const SbVec3d & point) const
{
  // This one is dead easy, we just take the dot product of the normal
  // vector and the vector going from the plane base point to the
  // point we're checking against, and see if the angle between the
  // vectors are within 90 degree (which is the same as checking the sign
  // of the dot product).
  //                                                    19980816 mortene.
#if 0 // not very efficient code, disabled 19991012 pederb
  SbVec3d pointToPlaneBase = point - (this->normal * this->distance);
  double dotWithNormal = this->normal.dot(pointToPlaneBase);
  if(dotWithNormal >= 0.0f) return TRUE;
  return FALSE;
#else // this code uses distance to plane instead
  return this->getDistance(point) >= 0.0f;
#endif // new code
}

/*!
  Return the distance from \a point to plane. Positive distance means
  the point is in the plane's halfspace.

  This method is an extension specific to Coin versus the original SGI
  Inventor API.
*/
double
SbDPPlane::getDistance(const SbVec3d & point) const
{
  return point.dot(this->normal) - this->distance;
}

/*!
  Return the plane's normal vector, which indicates which direction the plane
  is oriented.

  \sa getDistanceFromOrigin().
*/
const SbVec3d&
SbDPPlane::getNormal(void) const
{
  return this->normal;
}

/*!
  Return distance from origin of coordinate system to the point in the plane
  which is closest to the origin.

  \sa getNormal().
*/
double
SbDPPlane::getDistanceFromOrigin(void) const
{
  return this->distance;
}

/*!
  Intersect this plane with \a pl, and return the resulting line in \a
  line. Returns \c TRUE if an intersection line can be found, and \c
  FALSE if the planes are parallel.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SbBool
SbDPPlane::intersect(const SbDPPlane & pl, SbDPLine & line) const
{
  // Based on code from Graphics Gems III, Plane-to-Plane Intersection
  // by Priamos Georgiades

  double invdet;  // inverse of 2x2 matrix determinant
  SbVec3d dir2;  // holds the squares of the coordinates of xdir

  SbVec3d xpt;
  SbVec3d xdir;
  xdir = this->normal.cross(pl.normal);

  dir2[0] = xdir[0] * xdir[0];
  dir2[1] = xdir[1] * xdir[1];
  dir2[2] = xdir[2] * xdir[2];

  const SbVec3d & pl1n = this->normal;
  const SbVec3d & pl2n = pl.normal;
  const double pl1w = - this->distance;
  const double pl2w = - pl.distance;

  if (dir2[2] > dir2[1] && dir2[2] > dir2[0] && dir2[2] > DBL_EPSILON) {
    // then get a point on the XY plane
    invdet = 1.0f / xdir[2];
    xpt = SbVec3d(pl1n[1] * pl2w - pl2n[1] * pl1w,
                  pl2n[0] * pl1w - pl1n[0] * pl2w, 0.0f);
  }
  else if (dir2[1] > dir2[0] && dir2[1] > DBL_EPSILON) {
    // then get a point on the XZ plane
    invdet = -1.0f / xdir[1];
    xpt = SbVec3d(pl1n[2] * pl2w - pl2n[2] * pl1w, 0.0f,
                  pl2n[0] * pl1w - pl1n[0] * pl2w);
  }
  else if (dir2[0] > DBL_EPSILON) {
    // then get a point on the YZ plane
    invdet = 1.0f / xdir[0];
    xpt = SbVec3d(0.0f, pl1n[2] * pl2w - pl2n[2] * pl1w,
                  pl2n[1] * pl1w - pl1n[1] * pl2w);
  }
  else // xdir is zero, then no point of intersection exists
    return FALSE;

  xpt *= invdet;
  invdet = 1.0f / static_cast<double>(sqrt(dir2[0] + dir2[1] + dir2[2]));

  xdir *= invdet;
  line.setPosDir(xpt, xdir);
  return TRUE;
}

/*!
  Intersect this plane with \a p1 and \a p2, and return the unique
  intersection point in \a point. Returns \c FALSE when the three planes do
  not define a numerically stable, finite point.

  A system is considered numerically stable when both its absolute normalized
  scalar triple product and its infinity-norm reciprocal-condition estimate
  are strictly greater than the square root of the double-precision machine
  epsilon. Coordinates use a direct fixed-size Cramer path when its products,
  sums and division are safe, and otherwise use a scaled accumulator fallback.

  The \a point argument is left unchanged when this method returns \c FALSE.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SbDPPlane::intersect(const SbDPPlane & p1, const SbDPPlane & p2,
                     SbVec3d & point) const
{
  coin_sbdpplane_data planes[3] = {
    { this->normal, this->distance },
    { p1.normal, p1.distance },
    { p2.normal, p2.distance }
  };
  for (int i = 0; i < 3; ++i) {
    if (!std::isfinite(planes[i].distance)) return FALSE;
    for (int component = 0; component < 3; ++component) {
      if (!std::isfinite(planes[i].normal[component])) return FALSE;
    }
  }
  coin_sbdpplane_sort(planes);

  const SbVec3d & n0 = planes[0].normal;
  const SbVec3d & n1 = planes[1].normal;
  const SbVec3d & n2 = planes[2].normal;
  const SbVec3d c12 = n1.cross(n2);
  const SbVec3d c20 = n2.cross(n0);
  const SbVec3d c01 = n0.cross(n1);
  const double determinant = n0.dot(c12);
  const double normalproduct = n0.length() * n1.length() * n2.length();
  const double determinantlimit = std::sqrt(DBL_EPSILON);

  if (!std::isfinite(determinant) || !std::isfinite(normalproduct) ||
      normalproduct == 0.0 ||
      std::fabs(determinant) <= determinantlimit * normalproduct) {
    return FALSE;
  }

  const SbVec3d cofactors[3] = { c12, c20, c01 };
  double matrixnorm = 0.0;
  double adjugatenorm = 0.0;
  for (int i = 0; i < 3; ++i) {
    const double rowsum = std::fabs(planes[i].normal[0]) +
                          std::fabs(planes[i].normal[1]) +
                          std::fabs(planes[i].normal[2]);
    if (!std::isfinite(rowsum)) return FALSE;
    if (rowsum > matrixnorm) matrixnorm = rowsum;
  }
  for (int component = 0; component < 3; ++component) {
    double columnsum = 0.0;
    for (int i = 0; i < 3; ++i) {
      if (!std::isfinite(cofactors[i][component])) return FALSE;
      columnsum += std::fabs(cofactors[i][component]);
    }
    if (!std::isfinite(columnsum)) return FALSE;
    if (columnsum > adjugatenorm) adjugatenorm = columnsum;
  }

  const double conditiondenominator = matrixnorm * adjugatenorm;
  if (!std::isfinite(conditiondenominator) ||
      conditiondenominator == 0.0) {
    return FALSE;
  }
  const double reciprocalcondition =
    std::fabs(determinant) / conditiondenominator;
  if (!std::isfinite(reciprocalcondition) ||
      reciprocalcondition <= determinantlimit) {
    return FALSE;
  }

  const double distances[3] = {
    planes[0].distance, planes[1].distance, planes[2].distance
  };

  SbVec3d result(0.0, 0.0, 0.0);
  for (int component = 0; component < 3; ++component) {
    double coordinate;
    if (!coin_sbdpplane_direct_coordinate(distances, cofactors, component,
                                          determinant, coordinate) &&
        !coin_sbdpplane_scaled_coordinate(distances, cofactors, component,
                                          determinant, coordinate)) {
      return FALSE;
    }
    result[component] = coordinate == 0.0 ? 0.0 : coordinate;
  }

  point = result;
  return TRUE;
}

/*!
  \relates SbDPPlane

  Check the two given planes for equality.
*/
int
operator ==(const SbDPPlane & p1, const SbDPPlane & p2)
{
  if(p1.getDistanceFromOrigin() == p2.getDistanceFromOrigin() &&
     p1.getNormal() == p2.getNormal()) return TRUE;
  return FALSE;
}

/*!
  \relates SbDPPlane

  Check the two given planes for inequality.
*/
int
operator !=(const SbDPPlane & p1, const SbDPPlane & p2)
{
  return !(p1 == p2);
}

/*!
  Dump the state of this object to the \a fp file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbDPPlane::print(FILE * COIN_UNUSED_ARG(fp)) const
{
#if COIN_DEBUG
  this->getNormal().print(fp);
  (void)fprintf(fp, "  %f", this->getDistanceFromOrigin());
#endif // COIN_DEBUG
}

#ifdef COIN_TEST_SUITE
#include <Inventor/SbDPLine.h>
#include <Inventor/SbDPPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbTypeInfo.h>

#include <cfloat>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace SIM::Coin::TestSuite;

float slew(float Start, float End, int steps, int step) {
  const float S = log(Start<0?-Start:Start);
  const float E = log(End<0?-End:End);

  --steps;

  float res=0;

  assert(Start<End);

  if (Start<0 && End < 0 ) {
    res=-exp(S+step*(E-S)/steps);
  }
  else if(Start>0 && End>0) {
    res=exp(S+step*(E-S)/steps);
  }
  else {
    float ZeroPoint = S*steps/(E+S);
    if(step<ZeroPoint) {
      res=-exp(S-2*step*S/ZeroPoint);
    }
    else if (step==ZeroPoint) {
      res=0;
    }
    else {
      res=exp(2*E*(step-ZeroPoint)/(steps-ZeroPoint)-E);
    }
  }

  return res;
}

BOOST_AUTO_TEST_CASE(signCorrect)
{
  SbDPPlane plane1(SbVec3d(0.0, 0.0, 1.0), 3.0);
  SbDPPlane plane2(SbVec3d(1.0, 0.0, 0.0), 21.0);
  SbDPLine line;
  plane1.intersect(plane2, line); 

  SbVec3d intersect = line.getPosition();
  SbVec3d vec(21, 0, 3);

  check_compare(intersect,vec, "SbDPPlane SignCorrect", .1f);
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesUniquePointAndPermutations)
{
  const SbVec3d expected(2.0, -3.0, 5.0);
  const SbDPPlane planes[3] = {
    SbDPPlane(SbVec3d(1.0, 2.0, 3.0), expected),
    SbDPPlane(SbVec3d(-2.0, 1.0, 4.0), expected),
    SbDPPlane(SbVec3d(3.0, -1.0, 2.0), expected)
  };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    const SbBool ok = planes[permutations[i][0]].intersect(
      planes[permutations[i][1]], planes[permutations[i][2]], result);
    BOOST_CHECK_MESSAGE(ok == TRUE,
                        "Three non-singular planes must have a unique point");
    BOOST_CHECK_MESSAGE((result - expected).sqrLength() <= 1.0e-26,
                        "Plane permutation changed the intersection point");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesFailurePreservesOutput)
{
  const SbDPPlane p0(SbVec3d(1.0, 0.0, 0.0), 1.0);
  const SbDPPlane p1(SbVec3d(0.0, 1.0, 0.0), 2.0);
  const SbDPPlane p2(SbVec3d(1.0, 1.0, 0.0), 3.0);
  const SbVec3d sentinel(123.0, -456.0, 789.0);
  SbVec3d result = sentinel;

  BOOST_CHECK_MESSAGE(p0.intersect(p1, p2, result) == FALSE,
                      "A rank-deficient system must not produce a point");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "A failed intersection must preserve the output");

  const double maximum = std::numeric_limits<double>::max();
  const SbDPPlane huge0(SbVec3d(1.0, 0.0, 0.0), maximum);
  const SbDPPlane huge1(SbVec3d(0.0, 1.0, 0.0), maximum);
  const SbDPPlane huge2(SbVec3d(1.0, 0.0, 1.0e-7), -maximum);
  result = sentinel;
  BOOST_CHECK_MESSAGE(huge0.intersect(huge1, huge2, result) == FALSE,
                      "A non-finite double point must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Overflow failure must preserve the output");

  const SbDPPlane infinite(SbVec3d(0.0, 0.0, 1.0),
                           std::numeric_limits<double>::infinity());
  result = sentinel;
  BOOST_CHECK_MESSAGE(p0.intersect(p1, infinite, result) == FALSE,
                      "A non-finite plane distance must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Non-finite input failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesRejectsNearSingularDoubleSystem)
{
  const SbVec3d expected(2.0, -3.0, 5.0);
  const SbDPPlane p0(SbVec3d(1.0, 0.0, 0.0), expected);
  const SbDPPlane p1(SbVec3d(0.0, 1.0, 0.0), expected);
  const SbDPPlane p2(SbVec3d(1.0, 1.0, 1.0e-16), expected);
  const SbVec3d sentinel(123.0, -456.0, 789.0);
  SbVec3d result = sentinel;

  BOOST_CHECK_MESSAGE(p0.intersect(p1, p2, result) == FALSE,
                      "An ill-conditioned double system must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Near-singular failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesConditioningBoundary)
{
  const double limit = std::sqrt(DBL_EPSILON);
  const SbVec3d expected(2.0, -3.0, 5.0);
  const SbDPPlane xplane(SbVec3d(1.0, 0.0, 0.0), expected);
  const SbDPPlane yplane(SbVec3d(0.0, 1.0, 0.0), expected);
  const double stablez = 4.0 * limit;
  const SbDPPlane stable(SbVec3d(std::sqrt(1.0 - stablez * stablez),
                                 0.0, stablez), expected);
  const SbDPPlane planes[3] = { xplane, yplane, stable };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A system above the double conditioning limit must be accepted");
    BOOST_CHECK_MESSAGE(result.equals(expected, 1.0e-12),
                        "A plane permutation changed the stable result");
  }

  const double unstablez = 0.5 * limit;
  const SbDPPlane unstable(SbVec3d(std::sqrt(1.0 - unstablez * unstablez),
                                   0.0, unstablez), expected);
  const SbVec3d sentinel(123.0, -456.0, 789.0);
  SbVec3d result = sentinel;
  BOOST_CHECK_MESSAGE(xplane.intersect(yplane, unstable, result) == FALSE,
                      "A system below the double conditioning limit must fail");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Conditioning failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesAvoidsIntermediateOverflow)
{
  const double invsqrt2 = 1.0 / std::sqrt(2.0);
  const double invsqrt3 = 1.0 / std::sqrt(3.0);
  const double invsqrt6 = 1.0 / std::sqrt(6.0);
  const SbVec3d normals[3] = {
    SbVec3d(invsqrt2, invsqrt2, 0.0),
    SbVec3d(invsqrt6, -invsqrt6, 2.0 * invsqrt6),
    SbVec3d(invsqrt3, -invsqrt3, -invsqrt3)
  };
  const double scale = 0.9 * std::numeric_limits<double>::max();
  const double distances[3] = { scale, scale, -0.5 * scale };
  const SbDPPlane planes[3] = {
    SbDPPlane(normals[0], distances[0]),
    SbDPPlane(normals[1], distances[1]),
    SbDPPlane(normals[2], distances[2])
  };
  const SbVec3d expectedscaled = planes[0].getNormal() +
                                 planes[1].getNormal() -
                                 0.5 * planes[2].getNormal();
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A finite point must survive intermediate Cramer scaling");
    BOOST_CHECK_MESSAGE((result / scale).equals(expectedscaled, 1.0e-28),
                        "Scaled Cramer result differs from the finite point");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesExtremePermutationInvariance)
{
  const double distance = 0.8 * std::numeric_limits<double>::max();
  const SbDPPlane planes[3] = {
    SbDPPlane(SbVec3d(2.0, 1.0, -2.0), distance),
    SbDPPlane(SbVec3d(2.0, -2.0, 1.0), distance),
    SbDPPlane(SbVec3d(-1.0, -2.0, -2.0), distance)
  };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };
  SbVec3d reference;

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A finite point near DBL_MAX must not depend on plane order");
    if (i == 0) reference = result;
    else {
      BOOST_CHECK_MESSAGE(result == reference,
                          "Canonical plane ordering must give one result");
    }
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesPreservesWideDynamicRange)
{
  const double maximum = std::numeric_limits<double>::max();
  const double minimum = std::numeric_limits<double>::min();
  const SbDPPlane xplane(SbVec3d(1.0, 0.0, 0.0), maximum);
  const SbDPPlane yplane(SbVec3d(0.0, 1.0, 0.0), minimum);
  const SbDPPlane zplane(SbVec3d(0.0, 0.0, 1.0), 1.0);
  SbVec3d result(123.0, -456.0, 789.0);

  BOOST_CHECK_MESSAGE(xplane.intersect(yplane, zplane, result) == TRUE,
                      "A finite point with a wide dynamic range must succeed");
  BOOST_CHECK_MESSAGE(result == SbVec3d(maximum, minimum, 1.0),
                      "The solver must not discard a small finite coordinate");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesRecoversSubnormalNumerator)
{
  if (std::numeric_limits<double>::has_denorm != std::denorm_present) return;
  volatile double subnormalprobe =
    std::numeric_limits<double>::denorm_min();
  if (subnormalprobe == 0.0) return;

  const double minimum = std::numeric_limits<double>::min();
  const double xcoefficient = 8.0e-8;
  const double ycoefficient = -1.0e-16;
  const double zcoefficient =
    std::sqrt(1.0 - xcoefficient * xcoefficient -
              ycoefficient * ycoefficient);
  const SbDPPlane planes[3] = {
    SbDPPlane(SbVec3d(0.0, 1.0, 0.0), minimum),
    SbDPPlane(SbVec3d(0.0, 0.0, 1.0), 0.0),
    SbDPPlane(SbVec3d(xcoefficient, ycoefficient, zcoefficient), 0.0)
  };
  const double expectedx = minimum *
                           (-ycoefficient / xcoefficient);
  const double tolerance =
    8.0 * std::numeric_limits<double>::denorm_min();
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A representable subnormal coordinate must be recovered");
    BOOST_CHECK_MESSAGE(result[0] != 0.0,
                        "The subnormal coordinate must not be rounded to zero");
    BOOST_CHECK_MESSAGE(std::fabs(result[0] - expectedx) <= tolerance,
                        "The recovered subnormal coordinate is inaccurate");
    BOOST_CHECK_MESSAGE(result[1] == minimum && result[2] == 0.0,
                        "Recovering a subnormal must preserve other coordinates");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesAvoidsProductOverflow)
{
  const double maximum = std::numeric_limits<double>::max();
  const double e = 0.00018279049925094723;
  const double f = 0.0001827904983780623;
  const SbDPPlane planes[3] = {
    SbDPPlane(SbVec3d(0.0, 0.0, 1.0), maximum),
    SbDPPlane(SbVec3d(1.0, e, 0.0), 0.0),
    SbDPPlane(SbVec3d(-f, 1.0, 0.0), 0.0)
  };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A finite quotient must survive numerator product overflow");
    BOOST_CHECK_MESSAGE((result / maximum).equals(
                          SbVec3d(0.0, 0.0, 1.0), 1.0e-30),
                        "Product reordering changed the finite point");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesCancelsOutOfRangeProducts)
{
  const double maximum = std::numeric_limits<double>::max();
  const double e = 0.00018279049925094723;
  const double f = 0.0001827904983780623;
  const SbDPPlane planes[3] = {
    SbDPPlane(SbVec3d(3.0e-8, 0.0, 1.0), maximum),
    SbDPPlane(SbVec3d(1.0, e, 0.0), 0.1 * maximum),
    SbDPPlane(SbVec3d(-f, 1.0, 0.0), 0.0)
  };
  const SbVec3d expectedscaled(0.099999998329381709,
                               1.8279049532433082e-05,
                               0.99999999700000053);
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3d result(123.0, -456.0, 789.0);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "Out-of-range terms must cancel before conversion to double");
    BOOST_CHECK_MESSAGE((result / maximum).equals(expectedscaled, 1.0e-28),
                        "Scaled accumulation changed the finite point");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesCanReturnTheOrigin)
{
  const SbDPPlane xplane(SbVec3d(1.0, 0.0, 0.0), 0.0);
  const SbDPPlane yplane(SbVec3d(0.0, 1.0, 0.0), 0.0);
  const SbDPPlane zplane(SbVec3d(0.0, 0.0, 1.0), 0.0);
  SbVec3d result(123.0, -456.0, 789.0);

  BOOST_CHECK_MESSAGE(xplane.intersect(yplane, zplane, result) == TRUE,
                      "A valid intersection at the origin must succeed");
  BOOST_CHECK_MESSAGE(result == SbVec3d(0.0, 0.0, 0.0),
                      "The origin intersection must be returned exactly");
}

BOOST_AUTO_TEST_CASE(equalityToFloatPlane)
{
  const float XMax = (float)pow(2.,FLT_MAX_EXP/3.);
  const float XMin = -XMax;

  const float YMax = (float)pow(2.,FLT_MAX_EXP/3.);
  const float YMin = -YMax;

#ifdef TEST_SUITE_QUICK
  const int XSteps = 4;
  const int YSteps = 4;
#endif //TEST_SUITE_QUICK
#ifdef TEST_SUITE_THOROUG
  const int XSteps = 6;
  const int YSteps = 6;
#endif //TEST_SUITE_THOROUG
#ifdef TEST_SUITE_EXPANSIVE
  const int XSteps = 10;
  const int YSteps = 10;
#endif //TEST_SUITE_EXPANSIVE

  for (int x1=0;x1<XSteps;++x1) {
    float X1=slew(XMin,XMax,XSteps,x1);
    for (int x2=0;x2<XSteps;++x2) {
      float X2=slew(XMin,XMax,XSteps,x2);
      for (int x3=0;x3<XSteps;++x3) {
        float X3=slew(XMin,XMax,XSteps,x3);
        SbVec3f fv1(X1,X2,X3);
        SbVec3d dv1(X1,X2,X3);
        
        for (int x4=0;x4<XSteps;++x4) {
          float X4=slew(XMin,XMax,XSteps,x4);

          SbPlane fp1(fv1,X4);
          SbDPPlane dp1(dv1,X4);

          check_compare(fp1.getDistanceFromOrigin(),dp1.getDistanceFromOrigin(), "Distance from origin differs", 64);
          check_compare(fp1.getNormal(),dp1.getNormal(),"Comparing normals yields different results",.000001f);
          for (int y1=0;y1<YSteps;++y1) {
            float Y1=slew(YMin,YMax,YSteps,y1);
            for (int y2=0;y2<YSteps;++y2) {
              float Y2=slew(YMin,YMax,YSteps,y2);
              for (int y3=0;y3<YSteps;++y3) {
                float Y3=slew(YMin,YMax,YSteps,y3);
                SbVec3f fv2(Y1,Y2,Y3);
                SbVec3d dv2(Y1,Y2,Y3);

                //A bit arbitrary, this holds
                const float tol = .03f;
                BOOST_CHECK_MESSAGE(
                                    floatEquals(fp1.getDistance(fv2),(float)dp1.getDistance(dv2),tol)||
                                    fabs(fp1.getDistance(fv2)-dp1.getDistance(dv2))/fabs(dp1.getDistanceFromOrigin())<tol,
                                    "Distance from plane is significantly different");
                for (int y4=0;y4<YSteps;++y4) {
                  float Y4=slew(YMin,YMax,YSteps,y3);
                  SbPlane fp2(fv2,Y4);
                  SbDPPlane dp2(dv2,Y4);

                  SbLine fLine;
                  SbDPLine dLine;
                  bool failed=false;
                  if (!fp1.intersect(fp2, fLine)) {
                    failed = true;
                  }
                  if (!dp1.intersect(dp2, dLine)) {
                    BOOST_CHECK_MESSAGE(failed,"Float intersection worked, but double intersection failed");
                    failed = true;
                  }
                  if (failed)
                    continue;


                  SbVec3f fDir(fLine.getDirection());
                  SbVec3d dDir(dLine.getDirection());

                  check_compare(fDir,dDir, "Intersection direction differs", .004f);
                }
              }
            }
          }
        }
      }
    }
  }
  
}

#endif //COIN_TEST_SUITE
