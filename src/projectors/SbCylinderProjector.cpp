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
  \class SbCylinderProjector SbCylinderProjector.h Inventor/projectors/SbCylinderProjector.h
  \brief The SbCylinderProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbRotation.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SbCylinderProjector::intersectFront
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::cylinder
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::orientToEye
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::needSetup
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::lastPoint
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderProjector::SbCylinderProjector(const SbBool orientToEye)
  : intersectFront(TRUE),
    cylinder(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f)), 1.0f),
    orientToEye(orientToEye),
    needSetup(TRUE)
{
}

/*!
  FIXME: write doc
*/
SbCylinderProjector::SbCylinderProjector(const SbCylinder &cyl,
                                         const SbBool orientToEye)
  : intersectFront(TRUE),
    cylinder(cyl),
    orientToEye(orientToEye),
    needSetup(TRUE)
{
}

/*!
  Project the point, but also find the rotation from previous
  projection to this one.

  \sa SbCylinderProjector::project()
*/
SbVec3f
SbCylinderProjector::projectAndGetRotation(const SbVec2f &point,
                                           SbRotation &rot)
{
  SbVec3f lastpt = this->lastPoint;
  SbVec3f newpt = this->project(point);
  this->lastPoint = newpt;
  rot = this->getRotation(lastpt, newpt);
  return newpt;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setCylinder(const SbCylinder &cyl)
{
  this->cylinder = cyl;
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
const SbCylinder &
SbCylinderProjector::getCylinder(void) const
{
  return this->cylinder;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setOrientToEye(const SbBool orientToEye)
{
  if (this->orientToEye != orientToEye) {
    this->orientToEye = orientToEye;
    this->needSetup = TRUE;
  }
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setFront(const SbBool isFront)
{
  if (this->intersectFront != isFront) {
    this->intersectFront = isFront;
    this->needSetup = TRUE;
  }
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isFront(void) const
{
  return this->intersectFront;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isPointInFront(const SbVec3f &point) const
{
  // FIXME: not quite sure about this one.
  // 19991207, pederb

  SbVec3f refDir;
  if (this->orientToEye) {
    refDir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(refDir, refDir);
  }
  else {
    refDir = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  const SbLine &axis = this->cylinder.getAxis();
  SbVec3f somePt = axis.getPosition() + refDir;
  SbVec3f ptOnAxis = axis.getClosestPoint(somePt);

  // find plane direction perpendicular to line
  SbVec3f planeDir = somePt - ptOnAxis;
  planeDir.normalize();

  ptOnAxis = axis.getClosestPoint(point);
  SbVec3f ptDir = point - ptOnAxis;
  ptDir.normalize();

  float dot = ptDir.dot(planeDir);
  if (intersectFront) return dot >= 0.0f;
  else return dot < 0.0f;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::intersectCylinderFront(const SbLine &line,
                                            SbVec3f &result)
{
  SbVec3f i0, i1;
  SbBool isect = this->cylinder.intersect(line, i0, i1);
  if (isect) {
    if (this->isPointInFront(i0)) result = i0;
    else result = i1;
  }
  return isect;
}

/*!
  Overloaded just to set \e needSetup to \a TRUE.
*/
void
SbCylinderProjector::setWorkingSpace(const SbMatrix &space)
{
  this->needSetup = TRUE;
  inherited::setWorkingSpace(space);
}
