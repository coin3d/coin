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
  \class SbCylinderSheetProjector SbCylinderSheetProjector.h Inventor/projectors/SbCylinderSheetProjector.h
  \brief The SbCylinderSheetProjector class projects 2D points to 3D points on a sheet covering a cylindrical shape.
  \ingroup projectors
 */

// FIXME: we do not use a hyperbolic sheet, as we're supposed to do,
// for this class. Instead we use a straight plane. This should hardly
// be noticable for the user, but for correctness, a hyperbolic sheet
// should of course be used. 20000308 mortene.

#include <Inventor/projectors/SbCylinderSheetProjector.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*! \var SbCylinderSheetProjector::workingProjPoint
  Last projected point, in the working space coordinate system.
*/
/*! \var SbCylinderSheetProjector::planeDir
  Normal vector of the plane defining the orientation of the sheet.
*/
/*! \var SbCylinderSheetProjector::tolPlane
  The tolerance value specifying how much of the cylinder is "above"
  the sheet.
*/


/*!
  Constructor. Uses default cylinder defintion, see
  SbCylinderProjector::SbCylinderProjector().

  \a orienttoeye decides whether or not the cylinder and sheet should
  always be oriented towards the viewer.
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(const SbBool orienttoeye)
  : inherited(orienttoeye)
{
}

/*!
  Constructor with explicit definition of projection cylinder.
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(const SbCylinder & cyl,
                                                   const SbBool orienttoeye)
  : inherited(cyl, orienttoeye)
{
}

// Documented in superclass.
SbProjector *
SbCylinderSheetProjector::copy(void) const
{
  return new SbCylinderSheetProjector(*this);
}

// Documented in superclass.
SbVec3f
SbCylinderSheetProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupPlane();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  // FIXME: add code to intersect hyperbolic sheet (see code in
  // SbSphereSheetProjector).
  //
  // Here's a complete, stand-alone example that can be used while
  // testing projection. It projects a grid on top of the
  // SbCylinderSheetProjector and spits out an iv-file with an
  // SoPointSet that shows off how the sheet will look:
  //
  //
  // -----8<--- [snip] -----8<--- [snip] -----8<--- [snip] ---
  // #include <stdio.h>
  // #include <Inventor/SbLinear.h>
  // #include <Inventor/projectors/SbCylinderSheetProjector.h>
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
  //   SbCylinderSheetProjector ssp;
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

  SbBool tst = this->intersectCylinderFront(projline, projpt);
  if (!tst) {
    if (!this->tolPlane.intersect(projline, projpt)) {
#if COIN_DEBUG
      SoDebugError::postWarning("SbCylinderSheetProjector::project",
                                "working line is parallel to cylinder axis.");
#endif // COIN_DEBUG
      return SbVec3f(0.0f, 0.0f, 0.0f);
    }
  }
  this->lastPoint = projpt;
  this->workingProjPoint = projpt; // FIXME: investigate (pederb)
  return projpt;
}

// Documented in superclass.
SbRotation
SbCylinderSheetProjector::getRotation(const SbVec3f & point1,
                                      const SbVec3f & point2)
{
  const SbLine & axis = this->cylinder.getAxis();
  SbVec3f v1 = point1 - axis.getClosestPoint(point1);
  SbVec3f v2 = point2 - axis.getClosestPoint(point2);
  SbRotation rot(v1, v2); // rotate vector v1 into vector v2

  // FIXME: add rotation from sheet (pederb)

  SbVec3f dummy;
  float angle;
  rot.getValue(dummy, angle);

  if (dummy.dot(axis.getDirection()) > 0.0f)
    return SbRotation(axis.getDirection(), angle);
  return SbRotation(axis.getDirection(), -angle);
}

/*!
  Recalculates projection surface settings after changes to the
  parameters.
*/
void
SbCylinderSheetProjector::setupPlane(void)
{
  const SbLine & axis = this->cylinder.getAxis();
  SbVec3f refDir;
  if (this->orientToEye) {
    refDir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(refDir, refDir);
  }
  else {
    refDir = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  SbVec3f somePt = axis.getPosition() + refDir;
  SbVec3f ptOnAxis = axis.getClosestPoint(somePt);

  this->planeDir = somePt - ptOnAxis;
  this->planeDir.normalize();
  if (!this->intersectFront) this->planeDir = -this->planeDir;

  this->tolPlane = SbPlane(this->planeDir, axis.getPosition());
  this->needSetup = FALSE;
}
