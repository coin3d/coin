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
  \class SbCylinderPlaneProjector SbCylinderPlaneProjector.h Inventor/projectors/SbCylinderPlaneProjector.h
  \brief The SbCylinderPlaneProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderPlaneProjector.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>
#include <float.h>

/*!
  FIXME: write doc
*/
SbCylinderPlaneProjector::SbCylinderPlaneProjector(const float edgeTol,
                                                   const SbBool orientToEye)
  : SbCylinderSectionProjector(edgeTol, orientToEye)
{
}

/*!
  FIXME: write doc
*/
SbCylinderPlaneProjector::SbCylinderPlaneProjector(const SbCylinder &cyl,
                                                   const float edgeTol,
                                                   const SbBool orientToEye)
  : SbCylinderSectionProjector(cyl, edgeTol, orientToEye)
{
}

/*!
  FIXME: write doc
*/
SbProjector *
SbCylinderPlaneProjector::copy(void) const
{
  return new SbCylinderPlaneProjector(*this);
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderPlaneProjector::project(const SbVec2f &point)
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

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderPlaneProjector::getRotation(const SbVec3f &point1,
                                      const SbVec3f &point2)
{
  SbBool tol1 = this->isWithinTolerance(point1);
  SbBool tol2 = this->isWithinTolerance(point2);
  return this->getRotation(point1, tol1, point2, tol2);
}

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderPlaneProjector::getRotation(const SbVec3f &point1, const SbBool tol1,
                                      const SbVec3f &point2, const SbBool tol2)
{
  if (tol1 && tol2) return inherited::getRotation(point1, point2);
  if (point1 == point2) {
    return SbRotation(this->cylinder.getAxis().getDirection(), 0.0f);
  }

  // create a line to project projections onto. This will make
  // the below calculations much simpler.
  SbLine horizLine;
  {
    SbVec3f dir = this->cylinder.getAxis().getDirection().cross(this->planeDir);
    horizLine = SbLine(this->cylinder.getAxis().getPosition(),
                       this->cylinder.getAxis().getPosition() + dir);
  }

  //
  // pt1 is the point projected onto horizLine. pt1_tol is different from
  // pt1 if tol1==FALSE. pt1_tol will then be on the edge of the cylinder
  // section, where the cylinder section intersects the plane, but it will
  // also be on horizLine.
  //
  SbVec3f pt1, pt1_tol, pt2, pt2_tol;

  pt1 = horizLine.getClosestPoint(point1);
  pt2 = horizLine.getClosestPoint(point2);

  if (tol1) {
    pt1_tol = pt1;
  }
  else {
    SbVec3f ptOnLine = this->planeLine.getClosestPoint(point1);
    SbLine myLine(point1, ptOnLine);
    if (!this->cylinder.intersect(myLine, pt1_tol)) {
      assert(0 && "shouldn't happen");
      pt1_tol = point1;
    }
    pt1_tol = horizLine.getClosestPoint(pt1_tol);
  }
  if (tol2) {
    pt2_tol = pt2;
  }
  else {
    SbVec3f ptOnLine = this->planeLine.getClosestPoint(point2);
    SbLine myLine(pt2, ptOnLine);
    if (!this->cylinder.intersect(myLine, pt2_tol)) {
      assert(0 && "shouldn't happen");
      pt2_tol = point2;
    }
    pt2_tol = horizLine.getClosestPoint(pt2_tol);
  }

  // find normal cylinder-section rotation
  SbRotation rot = inherited::getRotation(tol1 ? point1 : pt1_tol,
                                          tol2 ? point2 : pt2_tol);
  SbVec3f axis;
  float angle;
  rot.getValue(axis, angle);
  SbBool positiveAngle = TRUE;
  if (float(fabs(angle)) < FLT_EPSILON) {
    // test if angle will be positive or negative
    SbRotation dummy = inherited::getRotation(point1, point2);
    SbVec3f axis;
    float angle;
    dummy.getValue(axis, angle);
    if (angle < 0.0f) positiveAngle = FALSE;
  }
  else if (angle < 0.0f) {
    positiveAngle = FALSE;
  }

  float len = 0.0f;

  // both pts on same side of cylinder ?
  if (!tol1 && !tol2 && (pt1_tol == pt2_tol)) {
    len += (pt1-pt2).length();
  }
  else {
    if (!tol1) {
      len += (pt1_tol-pt1).length();
    }
    if (!tol2) {
      len += (pt2_tol-pt2).length();
    }
  }
  angle += len / this->cylinder.getRadius();

  if (axis.dot(this->cylinder.getAxis().getDirection()) > 0.0f)
    return SbRotation(this->cylinder.getAxis().getDirection(), angle);
  return SbRotation(this->cylinder.getAxis().getDirection(), angle);
}
