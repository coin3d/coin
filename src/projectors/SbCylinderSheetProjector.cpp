/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SbCylinderSheetProjector SbCylinderSheetProjector.h Inventor/projectors/SbCylinderSheetProjector.h
  \brief The SbCylinderSheetProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderSheetProjector.h>
#include <assert.h>
#include <math.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*! \var SbCylinderSheetProjector::workingProjPoint
  FIXME: write doc
*/
/*! \var SbCylinderSheetProjector::planeDir
  FIXME: write doc
*/
/*! \var SbCylinderSheetProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(const SbBool orientToEye)
  : inherited(orientToEye)
{
}

/*!
  FIXME: write doc
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(const SbCylinder & cyl,
                                                   const SbBool orientToEye)
  : inherited(cyl, orientToEye)
{
}

/*!
  FIXME: write doc
*/
SbProjector *
SbCylinderSheetProjector::copy(void) const
{
  return new SbCylinderSheetProjector(*this);
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderSheetProjector::project(const SbVec2f &point)
{
  if (this->needSetup) this->setupPlane();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  //
  // FIXME: add code to intersect hyperbolic sheet (see comments in
  // SbSphereSheetProjector).
  //

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

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderSheetProjector::getRotation(const SbVec3f &point1,
                                      const SbVec3f &point2)
{
  const SbLine &axis = this->cylinder.getAxis();
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
  FIXME: write doc
*/
void
SbCylinderSheetProjector::setupPlane(void)
{
  const SbLine &axis = this->cylinder.getAxis();
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
