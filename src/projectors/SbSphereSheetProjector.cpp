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

  //
  // FIXME: need to intersect with a hyperbolic sheet if intersection
  // hits a point on the sphere with an angle bigger than 45 or something
  // degrees from the planeDir (or if line doesn't intersect at all).
  // mortene suggest a formula like this: z = a*x^2 + b*y^2 + c, where sign
  // of a differs from sign of b. Will investigate later.
  //
  // pederb, 19991210
  //

  // UPDATE mortene 20020822: here's a complete, stand-alone example
  // that can be used while taking care of the FIXME above. It
  // projects a grid on top of the SbSphereSheetProjector and spits
  // out an iv-file with an SoPointSet that shows off how the sheet
  // will look:
  //
  //
  // -----8<--- [snip] -----8<--- [snip] -----8<--- [snip] ---
  // #include <stdio.h>
  // #include <Inventor/SbLinear.h>
  // #include <Inventor/projectors/SbSphereSheetProjector.h>
  // #include <Inventor/SoDB.h>
  //
  // int
  // main(void)
  // {
  //   SoDB::init();
  //
  //   const float START = 0.0f;
  //   const float END = 1.0f;
  //   const float STEPS = 50.0f;
  //   const float STEPSIZE = ((END - START) / STEPS);
  //
  //   SbSphere s(SbVec3f(0, 0, 0), 0.8);
  //   SbSphereSheetProjector ssp(s, /* orientToEye: */ TRUE);
  //
  //   SbViewVolume volume;
  //   volume.ortho(-1, 1, -1, 1, -1, 1);
  //   ssp.setViewVolume(volume);
  //
  //   (void)fprintf(stdout, "#Inventor V2.1 ascii\n\n"
  //                 "Separator {\n"
  //                 "  Coordinate3 {\n"
  //                 "    point [\n");
  //
  //   for (float i=START; i <= END; i += STEPSIZE) {
  //     for (float j=START; j <= END; j += STEPSIZE) {
  //       SbVec3f v = ssp.project(SbVec2f(j, i));
  //       (void)fprintf(stdout, "\t%f %f %f,\n", v[0], v[1], v[2]);
  //     }
  //   }
  //
  //   (void)fprintf(stdout, "      ]\n"
  //                 "    }\n"
  //                 "  DrawStyle { pointSize 2 }\n"
  //                 "  PointSet { }\n"
  //                 "}\n");
  //
  //   return 0;
  // }
  // -----8<--- [snip] -----8<--- [snip] -----8<--- [snip] ---


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
