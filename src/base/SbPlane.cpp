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
  \class SbPlane SbPlane.h Inventor/SbLinear.h
  \brief The SbPlane class represents a plane in 3D space.
  \ingroup base

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

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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
  //     N·point
  // d = -------, |N| == 1
  //       |N|²

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

  //     N·point
  // d = -------, |N| == 1
  //       |N|²

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
  if((l.getDirection()).dot(this->normal) == 0.0f) return FALSE;

  // From the discussion on SbLine::getClosestPoint() we know that
  // any point on the line can be expressed as:
  //                    Q = P + t*D    (1)
  //
  // We can also easily see that a point must satisfy this equation to lie
  // in the plane:
  //                    N·(Q - d*N) = 0, where N is the normal vector,
  //                                     Q is the point and d the offset
  //                                     from the origin.
  //
  // Combining these two equations and simplifying we get:
  //
  //                          d*|N|² - N·P
  //                    t = ----------------, |N| == 1
  //                               N·D
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
  the point is in the plane's half space.

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
  line = SbLine(xpt, xpt + xdir);
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

  Check the two given planes for unequality.
*/
int
operator !=(const SbPlane& p1, const SbPlane& p2)
{
  return !(p1 == p2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbPlane::print(FILE * fp) const
{
#if COIN_DEBUG
  this->getNormal().print(fp);
  (void)fprintf(fp, "  %f", this->getDistanceFromOrigin());
#endif // COIN_DEBUG
}
