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
  \brief The SbCylinderProjector class is the abstract base class for mapping to cylindrical surfaces.
  \ingroup projectors

  The cylinder projectors map 2D points to various surface types based
  on cylindrical shapes.

  \sa SbSphereProjector
 */

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbRotation.h>
#include <assert.h>


/*!
  \var SbCylinderProjector::intersectFront

  Flag which says whether or not we should map to the outside or
  inside of the cylinder surface.
*/
/*!
  \var SbCylinderProjector::cylinder

  Specification of the projection cylinder.
*/
/*!
  \var SbCylinderProjector::orientToEye

  Which direction the cylindrical surface is oriented.
*/
/*!
  \var SbCylinderProjector::needSetup

  Set to \c TRUE whenever the projection surface needs to be
  recalculated according to the setting of the
  SbCylinderProjector::orientToEye flag.
*/
/*!
  \var SbCylinderProjector::lastPoint

  Stores the previously projected 3D point.
*/
/*!
  \fn virtual SbRotation SbCylinderProjector::getRotation(const SbVec3f & point1, const SbVec3f & point2)

  Returns rotation on the projection surface which re-orients \a
  point1 to \a point2.
*/



/*!
  Default constructor sets up a cylinder along the Y axis with height
  1.
*/
SbCylinderProjector::SbCylinderProjector(const SbBool orienttoeye)
  : intersectFront(TRUE),
    cylinder(SbLine(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f)), 1.0f),
    orientToEye(orienttoeye),
    needSetup(TRUE)
{
}

/*!
  Constructor taking an explicit \a cylinder projection definition.
*/
SbCylinderProjector::SbCylinderProjector(const SbCylinder & cylinder,
                                         const SbBool orienttoeye)
  : intersectFront(TRUE),
    cylinder(cylinder),
    orientToEye(orienttoeye),
    needSetup(TRUE)
{
}

/*!
  Project the 2D point to a 3D coordinate on the cylindrical surface,
  and find the rotation from the last projection to this one.

  \sa project(), getRotation()
*/
SbVec3f
SbCylinderProjector::projectAndGetRotation(const SbVec2f & point,
                                           SbRotation & rot)
{
  SbVec3f lastpt = this->lastPoint;
  SbVec3f newpt = this->project(point);
  this->lastPoint = newpt;
  rot = this->getRotation(lastpt, newpt);
  return newpt;
}

/*!
  Set \a cylinder to project onto.
*/
void
SbCylinderProjector::setCylinder(const SbCylinder & cylinder)
{
  this->cylinder = cylinder;
  this->needSetup = TRUE;
}

/*!
  Returns projection cylinder.
*/
const SbCylinder &
SbCylinderProjector::getCylinder(void) const
{
  return this->cylinder;
}

/*!
  Sets whether or not the projection surface should be oriented
  towards the eye of the viewer.
*/
void
SbCylinderProjector::setOrientToEye(const SbBool orienttoeye)
{
  if (this->orientToEye != orienttoeye) {
    this->orientToEye = orienttoeye;
    this->needSetup = TRUE;
  }
}

/*!
  Returns the state of the cylinder orientation flag.
*/
SbBool
SbCylinderProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  Set whether to intersect with the outside of the cylinder (\a
  isfront equal to \c TRUE), or the inside.
*/
void
SbCylinderProjector::setFront(const SbBool infront)
{
  if (this->intersectFront != infront) {
    this->intersectFront = infront;
    this->needSetup = TRUE;
  }
}

/*!
  Returns value of the flag which decides whether to intersect with
  the outside or inside of the cylinder.
*/
SbBool
SbCylinderProjector::isFront(void) const
{
  return this->intersectFront;
}

/*!
  Check if \a point is on the frontside or the backside of the
  cylinder.
*/
SbBool
SbCylinderProjector::isPointInFront(const SbVec3f & point) const
{
  // FIXME: not quite sure about this one.
  // 19991207, pederb

  SbVec3f refdir;
  if (this->orientToEye) {
    refdir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(refdir, refdir);
  }
  else {
    refdir = SbVec3f(0.0f, 0.0f, 1.0f);
  }
  const SbLine & axis = this->cylinder.getAxis();
  SbVec3f somept = axis.getPosition() + refdir;
  SbVec3f ptonaxis = axis.getClosestPoint(somept);

  // find plane direction perpendicular to line
  SbVec3f planeDir = somept - ptonaxis;
  planeDir.normalize();

  ptonaxis = axis.getClosestPoint(point);
  SbVec3f ptDir = point - ptonaxis;
  ptDir.normalize();

  float dot = ptDir.dot(planeDir);
  if (intersectFront) return dot >= 0.0f;
  else return dot < 0.0f;
}

/*!
  Intersect \a line with the SbCylinderProjector::cylinder and place
  the intersection point (if any) in \a result.

  Returns \c TRUE if \a line actually hits the cylinder, \c FALSE if
  it doesn't intersect with it.
*/
SbBool
SbCylinderProjector::intersectCylinderFront(const SbLine & line,
                                            SbVec3f & result)
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
  Overloaded from parent to set \a needSetup to \c TRUE.
*/
void
SbCylinderProjector::setWorkingSpace(const SbMatrix & space)
{
  this->needSetup = TRUE;
  inherited::setWorkingSpace(space);
}
