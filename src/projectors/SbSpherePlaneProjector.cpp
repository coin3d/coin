/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SbSpherePlaneProjector SbSpherePlaneProjector.h Inventor/projectors/SbSpherePlaneProjector.h
  \brief The SbSpherePlaneProjector class projects 2D points to a half-sphere and a plane.
  \ingroup projectors

  This projector uses a plane along with the half-sphere of
  SbSphereSectionProjector for projections. If the 2D point mapping
  "misses" the sphere section, the 3D point will be projected onto
  the plane.

  \sa SbCylinderPlaneProjector
 */

#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  Default constructor. See
  SbSphereSectionProjector::SbSphereSectionProjector().
*/
SbSpherePlaneProjector::SbSpherePlaneProjector(const float edgetol,
                                               const SbBool orienttoeye)
  : inherited(edgetol, orienttoeye)
{
}

/*!
  Constructor with explicit specification of projection sphere.
*/
SbSpherePlaneProjector::SbSpherePlaneProjector(const SbSphere & sph,
                                               const float edgetol,
                                               const SbBool orienttoeye)
  : inherited(sph, edgetol, orienttoeye)
{
}

// Overloaded from parent.
SbProjector *
SbSpherePlaneProjector::copy(void) const
{
  return new SbSpherePlaneProjector(*this);
}

// Overloaded from parent.
SbVec3f
SbSpherePlaneProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupTolerance();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  SbBool tst = this->intersectSphereFront(projline, projpt);
  if (!tst || !this->isWithinTolerance(projpt)) {
    if (!this->tolPlane.intersect(projline, projpt)) {
#if COIN_DEBUG
      SoDebugError::postWarning("SbSphereSectionProjector::project",
                                "working line is perpendicular to plane direction.");
#endif // COIN_DEBUG
      // set to 0, 0, 0 to avoid crazy rotations. lastPoint will then
      // never change, and there will be no rotation in getRotation()
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
  }
  this->lastPoint = projpt;
  return projpt;
}

// Overloaded from parent.
SbRotation
SbSpherePlaneProjector::getRotation(const SbVec3f & point1,
                                    const SbVec3f & point2)
{
  return this->getRotation(point1, this->isWithinTolerance(point1),
                           point2, this->isWithinTolerance(point2));
}

/*!
  Calculates rotation from \a point1 to \a point2, with \a tol1 and \a
  tol2 deciding whether or not to use the tolerance setting.
*/
SbRotation
SbSpherePlaneProjector::getRotation(const SbVec3f & point1, const SbBool tol1,
                                    const SbVec3f & point2, const SbBool tol2)
{
  if (tol1 && tol2) return inherited::getRotation(point1, point2);
  SbVec3f vec = point2 - point1;
  SbVec3f axis = vec.cross(this->planeDir);
  axis.normalize();

  float angle = 0.0f;

  if (!tol1 && !tol2) {
    // FIXME: this might not be 100% correct in all cases, but
    // I doubt anyone will notice :-) pederb, 20000220
    angle = vec.length() / this->sphere.getRadius();
  }
  else {
    SbVec3f planePt;
    SbVec3f spherePt;
    if (!tol1) {
      planePt = point1;
      spherePt = point2;
    }
    else {
      planePt = point2;
      spherePt = point1;
    }

    SbVec3f dir = planePt - this->planePoint;
    dir.normalize();
    SbVec3f tolpt = this->planePoint + dir * this->tolDist;
    SbVec3f vec1 = tolpt - this->sphere.getCenter();
    SbVec3f vec2 = spherePt - this->sphere.getCenter();
    vec1.normalize();
    vec2.normalize();
    float cosval = vec1.dot(vec2);
    if (cosval > 1.0f) cosval = 1.0f;
    else if (cosval < -1.0f) cosval = -1.0f;
    // rotation on sphere
    angle = acos(cosval);
    // rotation caused by dragging plane
    angle += (tolpt-planePt).length() / this->sphere.getRadius();
  }
  return SbRotation(axis, -angle);
}
