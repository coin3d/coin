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
  \class SbPlane SbPlane.h Inventor/SbLinear.h
  \brief The SbPlane class represents a plane in 3D space.

  \ingroup coin_base

  SbPlane is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.
*/

#include <cassert>
#include <cstdio>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbMatrix.h>
#include <cfloat>
#include <cmath>
#include <limits>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "coindefs.h"

struct coin_sbplane_data {
  SbVec3d normal;
  double distance;
};

static SbBool
coin_sbplane_less(const coin_sbplane_data & lhs,
                  const coin_sbplane_data & rhs)
{
  for (int i = 0; i < 3; ++i) {
    if (lhs.normal[i] < rhs.normal[i]) return TRUE;
    if (lhs.normal[i] > rhs.normal[i]) return FALSE;
  }
  return lhs.distance < rhs.distance;
}

static void
coin_sbplane_sort(coin_sbplane_data planes[3])
{
  for (int i = 1; i < 3; ++i) {
    const coin_sbplane_data current = planes[i];
    int j = i;
    while (j > 0 && coin_sbplane_less(current, planes[j - 1])) {
      planes[j] = planes[j - 1];
      --j;
    }
    planes[j] = current;
  }
}


/*!
  An SbPlane instantiated with the default constructor will be
  uninitialized.
*/
SbPlane::SbPlane(void)
{
}

/*!
  Construct an SbPlane instance with a normal pointing in the given
  direction and the given shortest distance from the origin of the
  coordinate system to a point in the plane.

  \a normal must not be a null vector.
*/
SbPlane::SbPlane(const SbVec3f& normalref, const float D)
{
#if COIN_DEBUG
  if (normalref.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbPlane::SbPlane",
                              "Plane normal vector is a null vector.");
  }
#endif // COIN_DEBUG

  this->normal = normalref;
  // we test for a null vector above, just normalize
  (void) this->normal.normalize();
  this->distance = D;
}

/*!
  Construct an SbPlane with three points laying in the plane.  Make
  sure \a p0, \a p1 and \a p2 are actually three distinct points,
  not on a line, when using this constructor.
*/
SbPlane::SbPlane(const SbVec3f& p0, const SbVec3f& p1, const SbVec3f& p2)
{
#if COIN_DEBUG
  if(!(p0 != p1 && p1 != p2 && p0 != p2))
    SoDebugError::postWarning("SbPlane::SbPlane",
                              "The three points defining the plane cannot "
                              "be coincident.");
#endif // COIN_DEBUG

  this->normal = (p1 - p0).cross(p2 - p0);
#if COIN_DEBUG
  if (this->normal.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbPlane::SbPlane",
                              "The three points defining the plane cannot "
                              "be on line.");
  }
#endif // COIN_DEBUG

  // we test and warn about a null vector above
  (void) this->normal.normalize();
  //     N dot point
  // d = -------, |N| == 1
  //       |N|^2

  this->distance = this->normal.dot(p0);
}

/*!
  Construct an SbPlane from a normal and a point laying in the plane.

  \a normal must not be a null vector.
*/
SbPlane::SbPlane(const SbVec3f& normalref, const SbVec3f& point)
{
#if COIN_DEBUG
  if(normalref.sqrLength() == 0.0f)
    SoDebugError::postWarning("SbPlane::SbPlane",
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
SbPlane::offset(const float d)
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
SbPlane::intersect(const SbLine& l, SbVec3f& intersection) const
{
#if COIN_DEBUG
  if (this->normal.sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbPlane::intersect",
                              "Normal vector for plane is null vector");

  }
  if (l.getDirection().sqrLength() == 0.0f) {
    SoDebugError::postWarning("SbPlane::intersect",
                              "Intersecting line doesn't have a direction.");
  }
#endif // COIN_DEBUG

  // Check if the line is parallel to the plane.
  if(fabs(l.getDirection().dot(this->normal)) < FLT_EPSILON) return FALSE;

  // From the discussion on SbLine::getClosestPoint() we know that
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

  float t =
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
SbPlane::transform(const SbMatrix& matrix)
{
  SbVec3f ptInPlane = this->normal * this->distance;

  // according to discussions on comp.graphics.algorithms, the inverse
  // transpose matrix should be used to rotate the plane normal.
  SbMatrix invtransp = matrix.inverse().transpose();
  invtransp.multDirMatrix(this->normal, this->normal);

  // the point should be transformed using the original matrix
  matrix.multVecMatrix(ptInPlane, ptInPlane);

  if (this->normal.normalize() == 0.0f) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbPlane::transform",
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
SbPlane::isInHalfSpace(const SbVec3f& point) const
{
  return this->getDistance(point) >= 0.0f;
}

/*!
  Return the distance from \a point to plane. Positive distance means
  the point is in the plane's halfspace.

  This method is an extension specific to Coin versus the original SGI
  Inventor API.
*/
float
SbPlane::getDistance(const SbVec3f &point) const
{
  // convert to double before doing the dot product to increase precision of the result
  SbVec3d dp(point);
  SbVec3d dn(this->normal);
  return static_cast<float> (dp.dot(dn)) - this->distance;
}

/*!
  Return the plane's normal vector, which indicates which direction the plane
  is oriented.

  \sa getDistanceFromOrigin().
*/
const SbVec3f&
SbPlane::getNormal(void) const
{
  return this->normal;
}

/*!
  Return distance from origin of coordinate system to the point in the plane
  which is closest to the origin.

  \sa getNormal().
*/
float
SbPlane::getDistanceFromOrigin(void) const
{
  return this->distance;
}

/*!
  Intersect this plane with \a pl, and return the resulting line in \a
  line. Returns \c TRUE if an intersection line can be found, and \c
  FALSE if the planes are parallel.

  Please note that the resulting SbLine must be considered as a
  \e line intersecting the SbLine's origin, extending infinitely in both
  directions.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SbBool
SbPlane::intersect(const SbPlane & pl, SbLine & line) const
{
  // Based on code from Graphics Gems III, Plane-to-Plane Intersection
  // by Priamos Georgiades

  float invdet;  // inverse of 2x2 matrix determinant
  SbVec3f dir2;  // holds the squares of the coordinates of xdir

  SbVec3f xpt;
  SbVec3f xdir;
  xdir = this->normal.cross(pl.normal);

  dir2[0] = xdir[0] * xdir[0];
  dir2[1] = xdir[1] * xdir[1];
  dir2[2] = xdir[2] * xdir[2];

  const SbVec3f & pl1n = this->normal;
  const SbVec3f & pl2n = pl.normal;
  const float pl1w = -this->distance;
  const float pl2w = -pl.distance;

  if ((dir2[2] > dir2[1]) && (dir2[2] > dir2[0]) && (dir2[2] > FLT_EPSILON)) {
    // then get a point on the XY plane
    invdet = 1.0f / xdir[2];
    xpt = SbVec3f(pl1n[1] * pl2w - pl2n[1] * pl1w,
                  pl2n[0] * pl1w - pl1n[0] * pl2w, 0.0f);
  }
  else if ((dir2[1] > dir2[0]) && (dir2[1] > FLT_EPSILON)) {
    // then get a point on the XZ plane
    invdet = - 1.0f / xdir[1];
    xpt = SbVec3f(pl1n[2] * pl2w - pl2n[2] * pl1w, 0.0f,
                  pl2n[0] * pl1w - pl1n[0] * pl2w);
  }
  else if (dir2[0] > FLT_EPSILON) {
    // then get a point on the YZ plane
    invdet = 1.0f / xdir[0];
    xpt = SbVec3f(0.0f, pl1n[2] * pl2w - pl2n[2] * pl1w,
                  pl2n[1] * pl1w - pl1n[1] * pl2w);
  }
  else // xdir is zero, then no point of intersection exists
    return FALSE;

  xpt *= invdet;
  invdet = 1.0f / static_cast<float>(sqrt(dir2[0] + dir2[1] + dir2[2]));

  xdir *= invdet;
  line.setPosDir(xpt, xdir);
  return TRUE;
}

/*!
  Intersect this plane with \a p1 and \a p2, and return the unique
  intersection point in \a point. Returns \c FALSE when the three planes do
  not define a numerically stable, finite point.

  A system is considered numerically stable when the absolute normalized
  scalar triple product of the plane normals is greater than the square root
  of the single-precision machine epsilon.

  The \a point argument is left unchanged when this method returns \c FALSE.

  \COIN_FUNCTION_EXTENSION
*/
SbBool
SbPlane::intersect(const SbPlane & p1, const SbPlane & p2,
                   SbVec3f & point) const
{
  // Do the computation in double precision. The plane data is still float,
  // so the conditioning limit below is based on FLT_EPSILON.
  coin_sbplane_data planes[3] = {
    { SbVec3d(this->normal), this->distance },
    { SbVec3d(p1.normal), p1.distance },
    { SbVec3d(p2.normal), p2.distance }
  };
  for (int i = 0; i < 3; ++i) {
    if (!std::isfinite(planes[i].distance)) return FALSE;
    for (int component = 0; component < 3; ++component) {
      if (!std::isfinite(planes[i].normal[component])) return FALSE;
    }
  }
  coin_sbplane_sort(planes);

  const SbVec3d & n0 = planes[0].normal;
  const SbVec3d & n1 = planes[1].normal;
  const SbVec3d & n2 = planes[2].normal;
  const SbVec3d c12 = n1.cross(n2);
  const SbVec3d c20 = n2.cross(n0);
  const SbVec3d c01 = n0.cross(n1);
  const double determinant = n0.dot(c12);
  const double normalproduct = n0.length() * n1.length() * n2.length();
  const double determinantlimit =
    std::sqrt(static_cast<double>(FLT_EPSILON));

  if (!std::isfinite(determinant) || !std::isfinite(normalproduct) ||
      normalproduct == 0.0 ||
      std::fabs(determinant) <= determinantlimit * normalproduct) {
    return FALSE;
  }

  const double distances[3] = {
    planes[0].distance, planes[1].distance, planes[2].distance
  };
  double distancescale = 0.0;
  for (int i = 0; i < 3; ++i) {
    const double magnitude = std::fabs(distances[i]);
    if (magnitude > distancescale) distancescale = magnitude;
  }

  SbVec3d candidate(0.0, 0.0, 0.0);
  if (distancescale != 0.0) {
    const SbVec3d scaledcandidate =
      ((distances[0] / distancescale) * c12 +
       (distances[1] / distancescale) * c20 +
       (distances[2] / distancescale) * c01) / determinant;
    candidate = scaledcandidate * distancescale;
  }

  for (int i = 0; i < 3; ++i) {
    if (!std::isfinite(candidate[i]) ||
        std::fabs(candidate[i]) > std::numeric_limits<float>::max()) {
      return FALSE;
    }
  }

  const SbVec3f result(static_cast<float>(candidate[0]),
                       static_cast<float>(candidate[1]),
                       static_cast<float>(candidate[2]));

  point = result;
  return TRUE;
}

/*!
  \relates SbPlane

  Check the two given planes for equality.
*/
int
operator ==(const SbPlane& p1, const SbPlane& p2)
{
  if(p1.getDistanceFromOrigin() == p2.getDistanceFromOrigin() &&
     p1.getNormal() == p2.getNormal()) return TRUE;
  return FALSE;
}

/*!
  \relates SbPlane

  Check the two given planes for inequality.
*/
int
operator !=(const SbPlane& p1, const SbPlane& p2)
{
  return !(p1 == p2);
}

/*!
  Dump the state of this object to the \a fp file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbPlane::print(FILE * COIN_UNUSED_ARG(fp)) const
{
#if COIN_DEBUG
  this->getNormal().print(fp);
  (void)fprintf(fp, "  %f", this->getDistanceFromOrigin());
#endif // COIN_DEBUG
}

#ifdef COIN_TEST_SUITE
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <cfloat>
#include <cmath>
#include <limits>

using namespace SIM::Coin::TestSuite;

BOOST_AUTO_TEST_CASE(signCorrect)
{
  SbPlane plane1(SbVec3f(0.0, 0.0, 1.0), 3.0);
  SbPlane plane2(SbVec3f(1.0, 0.0, 0.0), 21.0);
  SbLine line;
  plane1.intersect(plane2, line); 

  SbVec3f intersect = line.getPosition();
  SbVec3f vec(21, 0, 3);

  check_compare(intersect,vec, "SbPlane SignCorrect", .1f);

}

BOOST_AUTO_TEST_CASE(intersectThreePlanesUniquePointAndPermutations)
{
  const SbVec3f expected(2.0f, -3.0f, 5.0f);
  const SbPlane planes[3] = {
    SbPlane(SbVec3f(1.0f, 2.0f, 3.0f), expected),
    SbPlane(SbVec3f(-2.0f, 1.0f, 4.0f), expected),
    SbPlane(SbVec3f(3.0f, -1.0f, 2.0f), expected)
  };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3f result(123.0f, -456.0f, 789.0f);
    const SbBool ok = planes[permutations[i][0]].intersect(
      planes[permutations[i][1]], planes[permutations[i][2]], result);
    BOOST_CHECK_MESSAGE(ok == TRUE,
                        "Three non-singular planes must have a unique point");
    BOOST_CHECK_MESSAGE(result.equals(expected, 1.0e-10f),
                        "Plane permutation changed the intersection point");
  }
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesFailurePreservesOutput)
{
  // These normals are pairwise non-parallel but linearly dependent.
  const SbPlane p0(SbVec3f(1.0f, 0.0f, 0.0f), 1.0f);
  const SbPlane p1(SbVec3f(0.0f, 1.0f, 0.0f), 2.0f);
  const SbPlane p2(SbVec3f(1.0f, 1.0f, 0.0f), 3.0f);
  const SbVec3f sentinel(123.0f, -456.0f, 789.0f);
  SbVec3f result = sentinel;

  BOOST_CHECK_MESSAGE(p0.intersect(p1, p2, result) == FALSE,
                      "A rank-deficient system must not produce a point");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "A failed intersection must preserve the output");

  const float maximum = std::numeric_limits<float>::max();
  const SbPlane huge0(SbVec3f(1.0f, 0.0f, 0.0f), maximum);
  const SbPlane huge1(SbVec3f(0.0f, 1.0f, 0.0f), maximum);
  const SbPlane huge2(SbVec3f(1.0f, 0.0f, 1.0e-3f), -maximum);
  result = sentinel;
  BOOST_CHECK_MESSAGE(huge0.intersect(huge1, huge2, result) == FALSE,
                      "An unrepresentable float point must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Overflow failure must preserve the output");

  const SbPlane infinite(SbVec3f(0.0f, 0.0f, 1.0f),
                         std::numeric_limits<float>::infinity());
  result = sentinel;
  BOOST_CHECK_MESSAGE(p0.intersect(p1, infinite, result) == FALSE,
                      "A non-finite plane distance must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Non-finite input failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesRejectsNearSingularFloatSystem)
{
  const SbVec3f expected(2.0f, -3.0f, 5.0f);
  const SbPlane p0(SbVec3f(1.0f, 0.0f, 0.0f), expected);
  const SbPlane p1(SbVec3f(0.0f, 1.0f, 0.0f), expected);
  const SbPlane p2(SbVec3f(1.0f, 1.0f, 1.0e-8f), expected);
  const SbVec3f sentinel(123.0f, -456.0f, 789.0f);
  SbVec3f result = sentinel;

  BOOST_CHECK_MESSAGE(p0.intersect(p1, p2, result) == FALSE,
                      "An ill-conditioned float system must be rejected");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Near-singular failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesConditioningBoundary)
{
  const float limit = std::sqrt(FLT_EPSILON);
  const SbVec3f expected(2.0f, -3.0f, 5.0f);
  const SbPlane xplane(SbVec3f(1.0f, 0.0f, 0.0f), expected);
  const SbPlane yplane(SbVec3f(0.0f, 1.0f, 0.0f), expected);
  const float stablez = 2.0f * limit;
  const SbPlane stable(SbVec3f(std::sqrt(1.0f - stablez * stablez),
                               0.0f, stablez), expected);
  const SbPlane planes[3] = { xplane, yplane, stable };
  const int permutations[6][3] = {
    { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 },
    { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 }
  };

  for (int i = 0; i < 6; ++i) {
    SbVec3f result(123.0f, -456.0f, 789.0f);
    BOOST_CHECK_MESSAGE(
      planes[permutations[i][0]].intersect(
        planes[permutations[i][1]], planes[permutations[i][2]], result) == TRUE,
      "A system above the float conditioning limit must be accepted");
    BOOST_CHECK_MESSAGE(result.equals(expected, 1.0e-6f),
                        "A plane permutation changed the stable result");
  }

  const float unstablez = 0.5f * limit;
  const SbPlane unstable(SbVec3f(std::sqrt(1.0f - unstablez * unstablez),
                                 0.0f, unstablez), expected);
  const SbVec3f sentinel(123.0f, -456.0f, 789.0f);
  SbVec3f result = sentinel;
  BOOST_CHECK_MESSAGE(xplane.intersect(yplane, unstable, result) == FALSE,
                      "A system below the float conditioning limit must fail");
  BOOST_CHECK_MESSAGE(result == sentinel,
                      "Conditioning failure must preserve the output");
}

BOOST_AUTO_TEST_CASE(intersectThreePlanesCanReturnTheOrigin)
{
  const SbPlane xplane(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f);
  const SbPlane yplane(SbVec3f(0.0f, 1.0f, 0.0f), 0.0f);
  const SbPlane zplane(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
  SbVec3f result(123.0f, -456.0f, 789.0f);

  BOOST_CHECK_MESSAGE(xplane.intersect(yplane, zplane, result) == TRUE,
                      "A valid intersection at the origin must succeed");
  BOOST_CHECK_MESSAGE(result == SbVec3f(0.0f, 0.0f, 0.0f),
                      "The origin intersection must be returned exactly");
}

#endif //COIN_TEST_SUITE
