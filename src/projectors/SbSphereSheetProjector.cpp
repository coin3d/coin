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
  \class SbSphereSheetProjector SbSphereSheetProjector.h Inventor/projectors/SbSphereSheetProjector.h
  \brief The SbSphereSheetProjector class projects 2D points to 3D points on a sheet covering a spherical shape.
  \ingroup projectors
 */

// FIXME: Outside the sphere, points will map to a plane -- not a
// hyperbolical sheet, as they are supposed to be.  pederb.

#include <Inventor/projectors/SbSphereSheetProjector.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>

/*!
  \var SbSphereSheetProjector::workingProjPoint

  Last projected point, in the working space coordinate system.
*/
/*!
  \var SbSphereSheetProjector::planePoint

  Position of the center of the sphere in the plane of the hyberbolic
  sheet.
*/
/*!
  \var SbSphereSheetProjector::planeDir

  Normal vector of the plane defining the orientation of the sheet.
*/
//   FIXME: planeDist is not used, probably not needed until sheet
//   code is in place. 20000308 mortene.
/*!
  \var SbSphereSheetProjector::planeDist
  \COININTERNAL
*/
/*!
  \var SbSphereSheetProjector::tolPlane

  The tolerance value specifying how much of the sphere is "above"
  the sheet.
*/


/*!
  Constructor. Uses default sphere defintion, see
  SbSphereProjector::SbSphereProjector().

  \a orienttoeye decides whether or not the sheet should always be
  oriented towards the viewer.
*/
SbSphereSheetProjector::SbSphereSheetProjector(const SbBool orienttoeye)
  : SbSphereProjector(orienttoeye)
{
}

/*!
  Constructor with explicit definition of projection sphere.
*/
SbSphereSheetProjector::SbSphereSheetProjector(const SbSphere & sph,
                                               const SbBool orienttoeye)
  : SbSphereProjector(sph, orienttoeye)
{
}

// Documented in superclass.
SbProjector *
SbSphereSheetProjector::copy(void) const
{
  return new SbSphereSheetProjector(*this);
}

// Documented in superclass.
SbVec3f
SbSphereSheetProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupPlane();

  SbLine projline = this->getWorkingLine(point);

  SbVec3f spherehit;
  SbBool atsphere = this->intersectSphereFront(projline, spherehit);

  if (atsphere) { projline.setValue(spherehit, spherehit + -(this->planeDir)); }

  SbVec3f planehit;
  SbBool atplane = this->tolPlane.intersect(projline, planehit);

  SbVec3f projpt;

  float planardist, meetdist;

  if (!atsphere && !atplane) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbSphereSectionProjector::project",
                              "line is perpendicular to plane direction.");
#endif // COIN_DEBUG
    // set to <0, 0, 0> to avoid crazy rotations. lastPoint will then
    // never change, and there will be no rotation from getRotation()
    projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    goto done;
  }

  // distance from plane hit point to plane center in the projector
  planardist = (planehit - this->planePoint).length();
  // let sphere and hyperbolic sheet meet at 45°
  meetdist =  this->sphere.getRadius() * cos(M_PI / 4.0f);

  if (planardist < meetdist) {
    assert(atsphere && "intersection ray missed sphere!?");
    projpt = spherehit;
  }
  else {
    // By Pythagoras' we know that the value of the sphere at 45°
    // angle from the groundplane will be (radius^2 * 0.5).
    float v = (this->sphere.getRadius() * this->sphere.getRadius()) * 0.5f;

    // A hyperbolic function is given by y = 1 / x, where x in our
    // case is the "radial" distance from the plane centerpoint to the
    // plane intersection point.
    float hyperbval = (1.0f / planardist) * v;
	    
    // Now, find the direction of the hyperbolic value vector.
    SbVec3f adddir(0.0f, 0.0f, 1.0f); // if orient-to-eye is FALSE
    if (this->isOrientToEye()) { adddir = -projline.getDirection(); }
    if (!this->intersectFront) { adddir.negate(); }
	    
    projpt = planehit + (adddir * hyperbval);
  }

 done:
  this->lastPoint = projpt;
  this->workingProjPoint = projpt; // FIXME: investigate (pederb)
  return projpt;
}

// Documented in superclass.
SbRotation
SbSphereSheetProjector::getRotation(const SbVec3f & point1,
                                    const SbVec3f & point2)
{
  if (this->needSetup) this->setupPlane();
  return SbRotation(point1-this->planePoint, point2-this->planePoint);
}

/*!
  Recalculates projection surface settings after changes to the
  parameters.
*/
void
SbSphereSheetProjector::setupPlane(void)
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

  this->planeDist = 0.0f;
  this->planePoint = this->sphere.getCenter();
  this->tolPlane = SbPlane(this->planeDir, this->planePoint);
  this->needSetup = FALSE;
}
