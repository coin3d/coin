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
  \class SbCylinderSectionProjector SbCylinderSectionProjector.h Inventor/projectors/SbCylinderSectionProjector.h
  \brief The SbCylinderSectionProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderSectionProjector.h>
#include <Inventor/SbVec2f.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*! \var SbCylinderSectionProjector::tolerance
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::tolDist
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeDir
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeLine
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeDist
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(const float edgeTol,
                                                       const SbBool orientToEye)
  : SbCylinderProjector(orientToEye),
    tolerance(edgeTol)
{
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(const SbCylinder & cyl,
                                                       const float edgeTol,
                                                       const SbBool orientToEye)
  : inherited(cyl, orientToEye),
    tolerance(edgeTol)
{
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
SbProjector *
SbCylinderSectionProjector::copy(void) const
{
  return new SbCylinderSectionProjector(*this);
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderSectionProjector::project(const SbVec2f &point)
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
      // set to 0,0,0 to avoid crazy rotations. lastPoint will then
      // never change, and there will be no rotation in getRotation()
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
    else {
      SbVec3f ptOnLine = this->planeLine.getClosestPoint(projpt);
      SbLine myLine(projpt, ptOnLine);
      if (!this->cylinder.intersect(myLine, projpt)) {
        assert(0 && "shouldn't happen");
        projpt = SbVec3f(0.0f, 0.0f, 0.0f);
      }
    }
  }

  this->lastPoint = projpt;
  return projpt;
}

/*!
  Find a rotation that rotates from \a point1 on cylinder to
  \a point2, also on cylinder. The rotation will be about the
  cylinder axis.
*/
SbRotation
SbCylinderSectionProjector::getRotation(const SbVec3f &point1,
                                        const SbVec3f &point2)
{
  const SbLine &axis = this->cylinder.getAxis();
  SbVec3f v1 = point1 - axis.getClosestPoint(point1);
  SbVec3f v2 = point2 - axis.getClosestPoint(point2);

  SbRotation rot(v1, v2); // rotate vector v1 into vector v2

  // this is to make sure rotation only happens around cylinder axis
  SbVec3f dummy;
  float angle;
  rot.getValue(dummy, angle);

  if (dummy.dot(axis.getDirection()) > 0.0f)
    return SbRotation(axis.getDirection(), angle);
  else
    return SbRotation(axis.getDirection(), -angle);
}

/*!
  FIXME: write doc
*/
void
SbCylinderSectionProjector::setTolerance(const float edgeTol)
{
  this->tolerance = edgeTol;
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
float
SbCylinderSectionProjector::getTolerance(void) const
{
  return this->tolerance;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderSectionProjector::isWithinTolerance(const SbVec3f &point)
{
  if (this->needSetup) this->setupTolerance();
  // check if behind tolerance plane
  if (!this->tolPlane.isInHalfSpace(point)) return FALSE;

  SbVec3f ptOnLine = this->planeLine.getClosestPoint(point);
  if ((ptOnLine-point).sqrLength() > this->sqrTolDist) return FALSE;
  return TRUE;
}

/*!
  FIXME: write doc
*/
void
SbCylinderSectionProjector::setupTolerance(void)
{
  SbVec3f refDir;
  if (this->orientToEye) {
    refDir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(refDir, refDir);
  }
  else {
    refDir = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  float radius = this->cylinder.getRadius();
  this->tolDist = this->tolerance * radius;
  this->sqrTolDist = this->tolDist * this->tolDist;
  const SbLine &axis = this->cylinder.getAxis();
  SbVec3f somePt = axis.getPosition() + refDir;
  SbVec3f ptOnAxis = axis.getClosestPoint(somePt);

  // find plane direction perpendicular to line
  this->planeDir = somePt - ptOnAxis;
  this->planeDir.normalize();
  if (!this->intersectFront) {
    this->planeDir = -this->planeDir;
  }
  // distance from plane to cylinder axis
  this->planeDist = sqrt(radius*radius - this->tolDist*this->tolDist);

  // create line parallel to axis, but in plane
  SbVec3f linePt = axis.getPosition()+this->planeDir*this->planeDist;
  this->planeLine = SbLine(linePt, linePt+axis.getDirection());
  this->tolPlane = SbPlane(this->planeDir, linePt);

  this->needSetup = FALSE;
}
