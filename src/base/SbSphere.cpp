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
  \class SbSphere SbLinear.h Inventor/SbLinear.h
  \brief The SbSphere class is a representation of a sphere.
  \ingroup base

  This class is used within many other classes in Coin. It contains
  the data neccessary to represent a sphere (a 3D point and a radius).

  \sa SbCylinder */

#include <assert.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbLine.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor does nothing. The center point and the radius
  will be uninitialized.
 */
SbSphere::SbSphere(void)
{
}

/*!
  Construct an SbSphere instance with the given center point and radius.
 */
SbSphere::SbSphere(const SbVec3f &center, const float radius)
{
#if COIN_DEBUG
  if (radius<0.0f)
    SoDebugError::postWarning("SbSphere::SbSphere",
                              "Radius should be >= 0.0f.");
#endif // COIN_DEBUG

  this->setValue(center, radius);
}

/*!
  Set the sphere's center point and radius.

  \sa getCenter(), getRadius().
 */
void
SbSphere::setValue(const SbVec3f &center, const float radius)
{
#if COIN_DEBUG
  if (radius<0.0f)
    SoDebugError::postWarning("SbSphere::setValue",
                              "Radius should be >= 0.0f.");
#endif // COIN_DEBUG
  this->setCenter(center);
  this->setRadius(radius);
}

/*!
  Set the sphere's center point.

  \sa setValue(), setRadius() and getCenter().
 */
void
SbSphere::setCenter(const SbVec3f &center)
{
  this->center = center;
}

/*!
  Set the sphere's radius.

  \sa setValue(), setCenter() and getRadius().
 */
void
SbSphere::setRadius(const float radius)
{
#if COIN_DEBUG
  if (radius<0.0f)
    SoDebugError::postWarning("SbSphere::setRadius",
                              "Radius should be >= 0.0f.");
#endif // COIN_DEBUG
  this->radius = radius;
}

/*!
  Returns an SbVec3f with the sphere's center point.

  \sa setCenter(), getRadius().
 */
const SbVec3f &
SbSphere::getCenter(void) const
{
  return this->center;
}

/*!
  Returns the sphere's radius.

  \sa setRadius(), getCenter().
 */
float
SbSphere::getRadius(void) const
{
  return this->radius;
}

/*!
  Make the sphere exactly contain \a box, i.e. the sphere center point
  will be the same as that of the box, and the radius will be the distance
  from the box center point to any of the corners.
 */
void
SbSphere::circumscribe(const SbBox3f &box)
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SbSphere::circumscribe",
                              "The box is empty.");
    return;
  }
#endif // COIN_DEBUG

  this->setCenter(box.getCenter());

  float dx, dy, dz;
  box.getSize(dx, dy, dz);

  this->setRadius(float(sqrt(dx*dx + dy*dy + dz*dz)) / 2.0f);
}

/*!
  Finds the intersection enter point for the given line \a l
  on the sphere.

  If the line does not intersect the sphere, \a FALSE is returned.
 */
SbBool
SbSphere::intersect(const SbLine &l, SbVec3f &intersection) const
{
  SbVec3f dummy;
  return this->intersect(l, intersection, dummy);
}

/*!
  Find the intersection points of the ray \a l on the sphere and
  return these in \a enter and \a exit. If the ray just "grazes"
  the sphere, the \a enter and \a exit points have equal values.

  If the ray does not intersect the sphere, \a FALSE is returned, otherwise
  we will return \a TRUE.
 */
SbBool
SbSphere::intersect(const SbLine &l, SbVec3f &enter, SbVec3f &exit) const
{
#if COIN_DEBUG
  if (!(l.getDirection().length()>0.0f))
    SoDebugError::postWarning("SbSphere::intersect",
                              "The line 'l' has no direction.");
#endif // COIN_DEBUG

  // We know the parameterized equation for a point on a line:
  //         Q = P + t*D,  P is a known line point, D is the line direction
  //
  // And the equation for a point on a sphere:
  //         |Q - C| = r,  C is the sphere center point, r is the radius
  //
  // Combining these, we get this equation which we need to solve for t:
  //         |P + t*D - C| = r
  //
  // A little bit of algebra, and we get a second order equation for t:
  //         t² + t * 2*(P·D - C·D) + (|P|² + |C|² - 2*P·C - r²) = 0
  //
  // Using t = (-b ± sqrt(b² - 4ac))/2a, a = 1, b = 2*(P·D - C·D),
  //                                     c = (|P|² + |C|² - 2*P·C - r²),
  //
  // we can solve for t and backsubstitute into the line equation to
  // find the enter and exit point (if any).
  //
  //                                                        19980823 mortene.

  SbVec3f linepos = l.getPosition();
  SbVec3f linedir = l.getDirection();
  linedir.normalize();
  SbVec3f scenter = this->getCenter();
  float r = this->getRadius();

  float b = 2.0f * (linepos.dot(linedir) - scenter.dot(linedir));
  float c =
    (linepos[0]*linepos[0] + linepos[1]*linepos[1] + linepos[2]*linepos[2]) +
    (scenter[0]*scenter[0] + scenter[1]*scenter[1] + scenter[2]*scenter[2]) -
    2.0f * linepos.dot(scenter) - r*r;

  float core = b*b - 4.0f*c;
  if(core >= 0.0f) {
    float t1 = (-b + float(sqrt(core)))/2.0f;
    float t2 = (-b - float(sqrt(core)))/2.0f;

    if(t1 > t2) SbSwap(t1, t2);
    enter = linepos + t1*linedir;
    exit = linepos + t2*linedir;
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/*!
  Returns \a TRUE of the given point \a p lies within the sphere.
 */
SbBool
SbSphere::pointInside(const SbVec3f &p) const
{
  return (p - center).length() < radius;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbSphere::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "center: " );
  this->getCenter().print(fp);
  fprintf( fp, "  radius: %f ", this->getRadius() );
#endif // COIN_DEBUG
}
