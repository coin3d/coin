/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SbPlaneProjector SbPlaneProjector.h Inventor/projectors/SbPlaneProjector.h
  \brief The SbPlaneProjector class projects 2D points to 3D points in a plane.
  \ingroup projectors

  The 3D projection of the 2D coordinates is for this projector class
  constrained to lie inside a pre-defined 3D plane.

  \sa SbLineProjector
*/

#include <Inventor/projectors/SbPlaneProjector.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \var SbPlaneProjector::plane

  The projection plane.
*/
/*!
  \var SbPlaneProjector::orientToEye

  Which direction the plane is oriented.
*/
/*!
  \var SbPlaneProjector::lastPoint

  Stores the previously projected 3D point.
*/
/*!
  \var SbPlaneProjector::needSetup

  Set to \c TRUE whenever the plane needs to be recalculated according
  to the setting of the SbPlaneProjector::orientToEye flag.
 */
/*!
  \var SbPlaneProjector::nonOrientPlane

  The "original" plane which was set (as opposed to the recalculated
  projection plane according to the SbPlaneProjector::orientToEye
  setting).
*/



/*!
  Constructor. Sets up a projection plane parallel with the XY-plane
  in world coordinates.
*/
SbPlaneProjector::SbPlaneProjector(const SbBool orient)
  : plane(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f),
    nonOrientPlane(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f),
    orientToEye(orient),
    needSetup(orient) // will need setup if orient-to-plane
{
}

/*!
  Constructor taking an explicit projection \a plane definition.
*/
SbPlaneProjector::SbPlaneProjector(const SbPlane & plane, const SbBool orient)
  : plane(plane),
    nonOrientPlane(plane),
    orientToEye(orient),
    needSetup(orient) // will need setup if orient-to-plane
{
}

/*!
  Projects 2D \a point into a 3D point within the current projection
  plane.
*/
SbVec3f
SbPlaneProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupPlane();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  SbBool ortho = this->viewVol.getProjectionType() == SbViewVolume::ORTHOGRAPHIC;

  SbBool ok = this->plane.intersect(projline, projpt);

  SbBool valid = ok;
  if (ok && !ortho) valid = this->verifyProjection(projpt);

  if (!valid && !ortho) {
    SbPlane wrldplane = this->plane;
    SbLine wrldline;
    this->workingToWorld.multLineMatrix(projline, wrldline);

    wrldplane.transform(this->workingToWorld);

    SbPlane farplane =
      this->viewVol.getPlane(SbProjector::findVanishingDistance());
    SbLine farline;

    if (farplane.intersect(wrldplane, farline)) {
      SbVec3f dummy;
      if (wrldline.getClosestPoints(farline, dummy, projpt)) {
        this->worldToWorking.multVecMatrix(projpt, projpt);
        valid = TRUE;
      }
    }
  }
  if (!valid) {
    // this can happen for instance with orthographic view volumes,
    // when the plane is perpendicular to the view.
#if COIN_DEBUG
    static int first = 1;
    if (first) {
      SoDebugError::post("SbPlaneProjector::project",
                         "Unable to find projection point. "
                         "Setting result to middle of view volume.");
      first = 0;
    }
#endif // COIN_DEBUG
    float depth = this->viewVol.getNearDist() + this->viewVol.getDepth() * 0.5f;
    SbLine worldline;
    this->workingToWorld.multLineMatrix(projline, worldline);
    SbVec3f ptonline = worldline.getPosition() + worldline.getDirection() * depth;
    this->worldToWorking.multVecMatrix(ptonline, ptonline);
    // project this point into plane.
    projpt = ptonline - this->plane.getNormal() * this->plane.getDistance(ptonline);
  }
  return projpt;
}

/*!
  Sets whether or not the plane should always be oriented towards the
  "eye" of the viewer.
*/
void
SbPlaneProjector::setOrientToEye(const SbBool orienttoeye)
{
  if (orienttoeye != this->orientToEye) {
    this->orientToEye = orienttoeye;
    this->needSetup = TRUE;
  }
}

/*!
  Returns the state of the plane orientation flag.
*/
SbBool
SbPlaneProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  Set a new projection \a plane.
*/
void
SbPlaneProjector::setPlane(const SbPlane & plane)
{
  this->nonOrientPlane = this->plane = plane;
  this->needSetup = TRUE;
}

/*!
  Returns the current projection plane.
*/
const SbPlane &
SbPlaneProjector::getPlane(void) const
{
  return this->plane;
}

/*!
  Returns a vector between the projected 3D points of \a viewpos1 and
  \a viewpos2.
*/
SbVec3f
SbPlaneProjector::getVector(const SbVec2f & viewpos1, const SbVec2f & viewpos2)
{
  SbVec3f mp1 = this->project(viewpos1);
  SbVec3f mp2 = this->project(viewpos2);
  this->lastPoint = mp2;
  return mp2 - mp1;
}

/*!
  Returns a vector between the last projected point and the projected
  3D point of \a viewpos.
*/
SbVec3f
SbPlaneProjector::getVector(const SbVec2f & viewpos)
{
  SbVec3f lp = this->lastPoint; // lastPoint is updated in project()
  return (this->project(viewpos) - lp);
}

/*!
  Explicitly set position of initial projection, so we get correct
  values for later calls to getVector() etc.
*/
void
SbPlaneProjector::setStartPosition(const SbVec2f & viewpos)
{
  this->lastPoint = this->project(viewpos);
}

/*!
  Explicitly set position of initial projection, so we get correct
  values for later calls to getVector() etc.
*/
void
SbPlaneProjector::setStartPosition(const SbVec3f & point)
{
  this->lastPoint = point;
}

// Documented in superclass.
SbProjector *
SbPlaneProjector::copy(void) const
{
  return new SbPlaneProjector(*this);
}

/*!
  Should be called whenever SbPlaneProjector::needSetup is \c
  TRUE. Will recalculate projection plane.
*/
void
SbPlaneProjector::setupPlane(void)
{
  if (this->orientToEye) {
    SbVec3f pnt = -this->nonOrientPlane.getNormal() *
      this->nonOrientPlane.getDistanceFromOrigin();
    SbVec3f dir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(dir, dir);
    this->plane = SbPlane(dir, pnt);
  }
  else {
    this->plane = this->nonOrientPlane;
  }
  this->needSetup = FALSE;
}
