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
  \class SbCylinderPlaneProjector SbCylinderPlaneProjector.h Inventor/projectors/SbCylinderPlaneProjector.h
  \brief The SbCylinderPlaneProjector class projects 2D points to a half-cylinder and a plane.
  \ingroup projectors

  This projector uses a plane along with the half-cylinder of
  SbCylinderSectionProjector for projections. If the 2D point mapping
  "misses" the cylinder section, the 3D point will be projected onto
  the plane.

  \sa SbSpherePlaneProjector
 */

#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <float.h>

/*!
  Default constructor. See
  SbCylinderSectionProjector::SbCylinderSectionProjector().
*/
SbCylinderPlaneProjector::SbCylinderPlaneProjector(const float edgetol,
                                                   const SbBool orienttoeye)
  : SbCylinderSectionProjector(edgetol, orienttoeye)
{
}

/*!
  Constructor with explicit specification of projection cylinder.
*/
SbCylinderPlaneProjector::SbCylinderPlaneProjector(const SbCylinder & cyl,
                                                   const float edgetol,
                                                   const SbBool orienttoeye)
  : SbCylinderSectionProjector(cyl, edgetol, orienttoeye)
{
}

// Documented in superclass.
SbProjector *
SbCylinderPlaneProjector::copy(void) const
{
  return new SbCylinderPlaneProjector(*this);
}

// Documented in superclass.
SbVec3f
SbCylinderPlaneProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupTolerance();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  SbBool tst = this->intersectCylinderFront(projline, projpt);
  if (!tst || !this->isWithinTolerance(projpt)) {
    if (!this->tolPlane.intersect(projline, projpt)) {
#if COIN_DEBUG
      SoDebugError::postWarning("SbCylinderSectionProjector::project",
                                "working line is parallel to cylinder axis.");
#endif // COIN_DEBUG
      return SbVec3f(0.0f, 0.0f, 0.0f);
    }
  }
  this->lastPoint = projpt;
  return projpt;
}

// Documented in superclass.
SbRotation
SbCylinderPlaneProjector::getRotation(const SbVec3f & point1,
                                      const SbVec3f & point2)
{
  SbBool tol1 = this->isWithinTolerance(point1);
  SbBool tol2 = this->isWithinTolerance(point2);
  return this->getRotation(point1, tol1, point2, tol2);
}

/*!
  Calculates rotation from \a point1 to \a point2, with \a tol1 and \a
  tol2 deciding whether or not to use the tolerance setting.
*/
SbRotation
SbCylinderPlaneProjector::getRotation(const SbVec3f & point1, const SbBool tol1,
                                      const SbVec3f & point2, const SbBool tol2)
{
  if (tol1 && tol2) return inherited::getRotation(point1, point2);
  if (point1 == point2) {
    return SbRotation(this->cylinder.getAxis().getDirection(), 0.0f);
  }


  // create a line to project projections onto. This will make
  // the below calculations much simpler.
  SbLine horizline;
  SbVec3f dir = this->cylinder.getAxis().getDirection().cross(this->planeDir);
  horizline = SbLine(this->planeLine.getPosition(),
                     this->planeLine.getPosition() + dir);
  //
  // pt1 is the point projected onto horizline. pt1_tol is different from
  // pt1 if tol1==FALSE. pt1_tol will then be on the edge of the cylinder
  // section, where the cylinder section intersects the plane, but it will
  // also be on horizline.
  //
  SbVec3f pt1, pt1_tol, pt2, pt2_tol;

  pt1 = horizline.getClosestPoint(point1);
  pt2 = horizline.getClosestPoint(point2);

  if (tol1) {
    pt1_tol = pt1;
  }
  else {
    SbVec3f ptOnLine = this->planeLine.getClosestPoint(point1);
    SbLine myLine(point1, ptOnLine);
    if (!this->cylinder.intersect(myLine, pt1_tol)) {
      // shouldn't happen, but be robust if it does
      return SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
    }
    pt1_tol = horizline.getClosestPoint(pt1_tol);
  }
  if (tol2) {
    pt2_tol = pt2;
  }
  else {
    SbVec3f ptOnLine = this->planeLine.getClosestPoint(point2);
    SbLine myLine(pt2, ptOnLine);
    if (!this->cylinder.intersect(myLine, pt2_tol)) {
      // shouldn't happen, but be robust if it does
      return SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f);
    }
    pt2_tol = horizline.getClosestPoint(pt2_tol);
  }

  // find normal cylinder-section rotation
  SbRotation rot = inherited::getRotation(tol1 ? point1 : pt1_tol,
                                          tol2 ? point2 : pt2_tol);
  SbVec3f axis;
  float angle;
  rot.getValue(axis, angle);

  if (axis.dot(this->cylinder.getAxis().getDirection()) < 0.0f) {
    axis = -axis;
    angle = 2.0f*float(M_PI) - angle;
  }
  float len = 0.0f;

  // both pts on same side of cylinder ?
  if (!tol1 && !tol2 && (pt1_tol == pt2_tol)) {
    if ((pt1-pt2).dot(dir) < 0.0f) {
      len += (pt1-pt2).length();
    }
    else {
      len -= (pt1-pt2).length();
    }
  }
  else {
    if (!tol1) {
      if ((pt1_tol-pt1).dot(dir) < 0.0f) {
        len -= (pt1_tol-pt1).length();
      }
      else {
        len += (pt1_tol-pt1).length();
      }
    }
    if (!tol2) {
      if ((pt2_tol-pt2).dot(dir) < 0.0f) {
        len += (pt2_tol-pt2).length();
      }
      else {
        len -= (pt2_tol-pt2).length();
      }
    }
  }
  angle += len / this->cylinder.getRadius();

  return SbRotation(this->cylinder.getAxis().getDirection(), angle);
}
