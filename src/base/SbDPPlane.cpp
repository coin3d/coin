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
  \class SbPlane SbPlane.h Inventor/SbLinear.h
  \brief The SbPlane class represents a plane in 3D space.
  \ingroup base

  SbDPPlane is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.
*/

#include <assert.h>
#include <stdio.h>
#include <Inventor/SbDPPlane.h>
#include <Inventor/SbDPLine.h>
#include <Inventor/SbDPMatrix.h>
#include <float.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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
SbDPPlane::SbDPPlane(const SbVec3d & normal, const double D)
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbDPPlane::SbDPPlane",
                              "Plane normal vector is a null vector.");
#endif // COIN_DEBUG

  this->normal = normal;
  this->normal.normalize();
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
  this->normal.normalize();

  //     N·point
  // d = -------, |N| == 1
  //       |N|²

  this->distance = this->normal.dot(p0);
}

/*!
  Construct an SbDPPlane from a normal and a point laying in the plane.

  \a normal must not be a null vector.
*/
SbDPPlane::SbDPPlane(const SbVec3d & normal, const SbVec3d & point)
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbDPPlane::SbDPPlane",
                              "Plane normal vector is a null vector.");
#endif // COIN_DEBUG

  this->normal = normal;
  this->normal.normalize();

  //     N·point
  // d = -------, |N| == 1
  //       |N|²

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
  if((l.getDirection()).dot(this->normal) == 0.0f) return FALSE;

  // From the discussion on SbDPLine::getClosestPoint() we know that
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

  this->normal.normalize();
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
  // vectors are within 90° (which is the same as checking the sign
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
  the point is in the plane's half space.

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

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-11 pederb
*/
SbBool
SbDPPlane::intersect(const SbDPPlane & pl, SbDPLine & line)
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
    invdet = 1.0f / xdir[1];
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
  invdet = 1.0f / (double) sqrt(dir2[0] + dir2[1] + dir2[2]);

  xdir *= invdet;
  line = SbDPLine(xpt, xpt+xdir);
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

  Check the two given planes for unequality.
*/
int
operator !=(const SbDPPlane & p1, const SbDPPlane & p2)
{
  return !(p1 == p2);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized build.
*/
void
SbDPPlane::print(FILE * fp) const
{
#if COIN_DEBUG
  this->getNormal().print(fp);
  (void)fprintf(fp, "  %f", this->getDistanceFromOrigin());
#endif // COIN_DEBUG
}
