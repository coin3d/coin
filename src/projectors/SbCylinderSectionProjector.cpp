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
  \class SbCylinderSectionProjector SbCylinderSectionProjector.h Inventor/projectors/SbCylinderSectionProjector.h
  \brief The SbCylinderSectionProjector projects 2D points to a sliced cylinder.
  \ingroup projectors

  The projection cylinder for this class is sliced by a clipping plane
  parallel to its height axis. Projections will be mapped to the
  remaining cylinder part.

  \sa SbSphereSectionProjector
*/

#include <Inventor/projectors/SbCylinderSectionProjector.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*! \var SbCylinderSectionProjector::tolerance
  Tolerance value, deciding how much of the half-cylinder to do
  projections against.
*/
/*! \var SbCylinderSectionProjector::tolDist
  Tolerance value multiplied with the cylinder radius.
*/
/*! \var SbCylinderSectionProjector::tolPlane
  Defines the plane cutting the cylinder into a projection part.
*/
/*! \var SbCylinderSectionProjector::planeDir
  Direction of cutting plane.
*/
/*! \var SbCylinderSectionProjector::planeLine
  A line within the plane which is parallel to the cylinder axis.
*/
/*! \var SbCylinderSectionProjector::planeDist
  Distance from plane to cylinder axis.
*/


/*!
  Default constructor. See SbCylinderProjector::SbCylinderProjector().

  The \a edgetol value should be within <0, 1], and specifies how much
  of the cylinder is used as a projection surface. 1.0 means the full
  front half is used.
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(const float edgetol,
                                                       const SbBool orienttoeye)
  : SbCylinderProjector(orienttoeye),
    tolerance(edgetol)
{
  this->needSetup = TRUE;
}

/*!
  Constructor with explicit setting of the projection cylinder.
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(const SbCylinder & cyl,
                                                       const float edgetol,
                                                       const SbBool orienttoeye)
  : inherited(cyl, orienttoeye),
    tolerance(edgetol)
{
  this->needSetup = TRUE;
}

// Documented in superclass.
SbProjector *
SbCylinderSectionProjector::copy(void) const
{
  return new SbCylinderSectionProjector(*this);
}

// Documented in superclass.
SbVec3f
SbCylinderSectionProjector::project(const SbVec2f & point)
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
      // set to 0, 0, 0 to avoid crazy rotations. lastPoint will then
      // never change, and there will be no rotation in getRotation()
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
    else {
      SbVec3f ptOnLine = this->planeLine.getClosestPoint(projpt);
      SbLine myLine(projpt, ptOnLine);
      if (!this->cylinder.intersect(myLine, projpt)) {
        // shouldn't happen, but be robust if it does
        projpt = SbVec3f(0.0f, 0.0f, 0.0f);
      }
    }
  }

  this->lastPoint = projpt;
  return projpt;
}

// Documented in superclass.
SbRotation
SbCylinderSectionProjector::getRotation(const SbVec3f & point1,
                                        const SbVec3f & point2)
{
  const SbLine & axis = this->cylinder.getAxis();
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
  The \a edgetol value decides how much of the surface of the cylinder
  is used for projection. 1.0 means the full cylinder half is used.
*/
void
SbCylinderSectionProjector::setTolerance(const float edgetol)
{
#if COIN_DEBUG // COIN_DEBUG
  if (edgetol <= 0.0f || edgetol > 1.0f) {
    SoDebugError::postWarning("SbCylinderSectionProjector::setTolerance",
                              "edge tolerance should be within <0, 1].");
  }
#endif // COIN_DEBUG
  this->tolerance = edgetol;
  this->needSetup = TRUE;
}

/*!
  Returns edge tolerance for the cylinder half.
*/
float
SbCylinderSectionProjector::getTolerance(void) const
{
  return this->tolerance;
}

/*!
  Check if \a point is within the part of the cylinder used for
  projections.
*/
SbBool
SbCylinderSectionProjector::isWithinTolerance(const SbVec3f & point)
{
  if (this->needSetup) this->setupTolerance();
  // check if behind tolerance plane
  if (!this->tolPlane.isInHalfSpace(point)) return FALSE;

  SbVec3f ptonline = this->planeLine.getClosestPoint(point);
  if ((ptonline-point).sqrLength() > this->sqrtoldist) return FALSE;
  return TRUE;
}

/*!
  Recalculate the internal projection surface settings. Needs to be
  done if any of the parameters influencing the projection surface
  have been changed from subclasses without using the access methods.
*/
void
SbCylinderSectionProjector::setupTolerance(void)
{
  SbVec3f refdir;
  if (this->orientToEye) {
    refdir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(refdir, refdir);
  }
  else {
    refdir = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  float radius = this->cylinder.getRadius();
  this->tolDist = this->tolerance * radius;
  this->sqrtoldist = this->tolDist * this->tolDist;
  const SbLine & axis = this->cylinder.getAxis();
  SbVec3f somept = axis.getPosition() + refdir;
  SbVec3f ptonaxis = axis.getClosestPoint(somept);

  // find plane direction perpendicular to line
  this->planeDir = somept - ptonaxis;
  this->planeDir.normalize();
  if (!this->intersectFront) {
    this->planeDir = -this->planeDir;
  }
  // distance from plane to cylinder axis
  this->planeDist = (float)sqrt(radius * radius - this->tolDist * this->tolDist);

  // create line parallel to axis, but in plane
  SbVec3f linept = axis.getPosition()+this->planeDir * this->planeDist;
  this->planeLine = SbLine(linept, linept + axis.getDirection());
  this->tolPlane = SbPlane(this->planeDir, linept);

  this->needSetup = FALSE;
}
