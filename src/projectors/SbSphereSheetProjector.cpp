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
  \class SbSphereSheetProjector SbSphereSheetProjector.h Inventor/projectors/SbSphereSheetProjector.h
  \brief The SbSphereSheetProjector class projects 2D points to 3D points on a sheet covering a spherical shape.
  \ingroup projectors
 */

// Metadon doc:
/*¡
  FIXME: Outside the sphere, points will map to a plane -- not a
  hyperbolical sheet, as they are supposed to be.  pederb.
*/

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
/*!
  \var SbSphereSheetProjector::planeDist
  \internal

  FIXME: not used, probably not needed until sheet code is in
  place. 20000308 mortene.
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

// Overloaded from parent.
SbProjector *
SbSphereSheetProjector::copy(void) const
{
  return new SbSphereSheetProjector(*this);
}

// Overloaded from parent.
SbVec3f
SbSphereSheetProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupPlane();

  //
  // FIXME: need to intersect with a hyperbolic sheet if intersection
  // hits a point on the sphere with an angle bigger than 45 or something
  // degrees from the planeDir (or if line doesn't intersect at all).
  // mortene suggest a formula like this: z = a*x^2 + b*y^2 + c, where sign
  // of a differs from sign of b. Will investigate later.
  //
  // pederb, 19991210
  //

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  SbBool tst = this->intersectSphereFront(projline, projpt);
  if (!tst) {
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
  this->workingProjPoint = projpt; // FIXME: investigate (pederb)
  return projpt;
}

// Overloaded from parent.
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
