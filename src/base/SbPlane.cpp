/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SbPlane SbLinear.h Inventor/SbLinear.h
  \brief The SbPlane class represents a plane in 3D space.
  \ingroup base

  SbPlane is used by many other classes in Coin.  It provides a way of
  representing a plane, specified by a plane normal vector and a
  distance from the origin of the coordinate system.
*/

#include <assert.h>
#include <stdio.h>
#include <iostream.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbMatrix.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <iostream.h> // For print() functionality.
#endif // COIN_DEBUG


/*!
  An SbPlane instantiated with the default constructor will have undefined
  behavior.
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
SbPlane::SbPlane(const SbVec3f& normal, const float D)
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbPlane::SbPlane",
			      "Plane normal vector is a null vector.");
#endif // COIN_DEBUG

  this->normal = normal;
  this->normal.normalize();
  this->distance = D;
}

/*!
  Construct an SbPlane with three points lying in the plane.
  Make sure \a p0, \a p1 and \a p2 are actually three distinct points
  when using this constructor.
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
  this->normal.normalize();

  //     N·point
  // d = -------, |N| == 1
  //       |N|²

  this->distance = this->normal.dot(p0);
}

/*!
  Construct an SbPlane from a normal and a point lying in the plane.

  \a normal must not be a null vector.
 */
SbPlane::SbPlane(const SbVec3f& normal, const SbVec3f& point)
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbPlane::SbPlane",
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
SbPlane::offset(const float d)
{
  this->distance += d;
}

/*!
  Find the point on given line \a l intersecting the plane and return
  it in \a intersection. If the line is parallel to the plane,
  we return \a FALSE, otherwise \a TRUE.
  
  Do not pass an invalid line for the \a l parameter (i.e. with a
  null direction vector).
 */
SbBool
SbPlane::intersect(const SbLine& l, SbVec3f& intersection) const
{
#if COIN_DEBUG
  if(!(normal.length() != 0.0f))
    SoDebugError::postWarning("SbPlane::intersect",
			      "Intersecting line doesn't have a direction.");
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
  // FIXME: this code is not tested, but it looks correct
  // 19990423, pederb
  SbVec3f ptInPlane = this->normal * this->distance;

  matrix.multVecMatrix(ptInPlane, ptInPlane);
  matrix.multDirMatrix(this->normal, this->normal);

  this->normal.normalize();
  this->distance = ptInPlane.length();
}

/*!
  Check if the given point lies in the halfspace of the plane which the
  plane normal vector is pointing.
 */
SbBool
SbPlane::isInHalfSpace(const SbVec3f& point) const
{
  // This one is dead easy, we just take the dot product of the normal
  // vector and the vector going from the plane base point to the
  // point we're checking against, and see if the angle between the
  // vectors are within 90° (which is the same as checking the sign
  // of the dot product).
  //                                                    19980816 mortene.

  SbVec3f pointToPlaneBase = point - (this->normal * this->distance);
  float dotWithNormal = this->normal.dot(pointToPlaneBase);
  if(dotWithNormal >= 0.0f) return TRUE;
  return FALSE;
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
  debug version of library, method does nothing in an optimized compile.
 */
void
SbPlane::print(ostream & file) const
{
#if COIN_DEBUG
  this->getNormal().print(file);
  file << "  " << this->getDistanceFromOrigin();
#endif // COIN_DEBUG
}
