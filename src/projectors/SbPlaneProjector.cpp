/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

#include <assert.h>
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
  if (!this->plane.intersect(projline, projpt)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbPlaneProjector::project",
                              "working line is parallel to plane.");
#endif // COIN_DEBUG
    // set to (0, 0, 0) to avoid crazy rotations
    projpt = SbVec3f(0.0f, 0.0f, 0.0f);
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

// Overloaded from parent.
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
