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
  \class SbSphereSectionProjector SbSphereSectionProjector.h Inventor/projectors/SbSphereSectionProjector.h
  \brief The SbSphereSectionProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbSphereSectionProjector.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*! \var SbSphereSectionProjector::tolerance
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::tolDist
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::radialFactor
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planePoint
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planeDir
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planeDist
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbSphereSectionProjector::SbSphereSectionProjector(const float edgeTol,
                                                   const SbBool orientToEye)
  : inherited(orientToEye),
    tolerance(edgeTol),
    radialFactor(0.0f)
{
  // needSetup will be TRUE, and other members will be initialized later
}

/*!
  FIXME: write doc
*/
SbSphereSectionProjector::SbSphereSectionProjector(const SbSphere & sph,
                                                   const float edgeTol,
                                                   const SbBool orientToEye)
  : inherited(sph, orientToEye),
    tolerance(edgeTol),
    radialFactor(0.0f)
{
  // needSetup will be TRUE, and other members will be initialized later
}

/*!
  FIXME: write doc
*/
SbProjector *
SbSphereSectionProjector::copy(void) const
{
  return new SbSphereSectionProjector(*this);
}

/*!
  FIXME: write doc
*/
SbVec3f
SbSphereSectionProjector::project(const SbVec2f &point)
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
      // set to 0,0,0 to avoid crazy rotations. lastPoint will then
      // never change, and there will be no rotation in getRotation()
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
    else {
      SbLine myLine(projpt, this->sphere.getCenter());
      if (!this->sphere.intersect(myLine, projpt)) {
        assert(0 && "shouldn't happen");
        projpt = SbVec3f(0.0f, 0.0f, 0.0f);
      }
    }
  }
  this->lastPoint = projpt;
  return projpt;
}

/*!
  FIXME: write doc
*/
SbRotation
SbSphereSectionProjector::getRotation(const SbVec3f &point1,
                                      const SbVec3f &point2)
{
  // FIXME: test how radialFactor affects rotations
  // pederb, 19991209
  const SbVec3f &c = this->sphere.getCenter();
  return SbRotation(point1-c, point2-c);
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setTolerance(const float edgeTol)
{
  this->needSetup = TRUE;
  this->tolerance = edgeTol;
}

/*!
  FIXME: write doc
*/
float
SbSphereSectionProjector::getTolerance(void) const
{
  return this->tolerance;
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setRadialFactor(const float rad)
{
  this->radialFactor = rad;
}

/*!
  FIXME: write doc
*/
float
SbSphereSectionProjector::getRadialFactor(void) const
{
  return this->radialFactor;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereSectionProjector::isWithinTolerance(const SbVec3f &point)
{
  if (this->needSetup) this->setupTolerance();
  return (planePoint-point).sqrLength() <= sqrTolDist;
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setupTolerance(void)
{
  if (this->orientToEye) {
    this->planeDir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(this->planeDir, this->planeDir);
    this->planeDir.normalize();
  }
  else {
    this->planeDir.setValue(0.0f, 0.0f, 1.0f);
  }
  if (!this->intersectFront) this->planeDir = -this->planeDir;

  float radius = this->sphere.getRadius();
  this->tolDist = this->tolerance * radius;
  this->sqrTolDist = this->tolDist * this->tolDist;
  this->planeDist = sqrt(radius*radius - this->tolDist*this->tolDist);
  this->planePoint = this->sphere.getCenter() +
    this->planeDir * this->planeDist;
  this->tolPlane = SbPlane(this->planeDir, this->planePoint);
  this->needSetup = FALSE;
}
