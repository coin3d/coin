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
  \class SbSphereProjector SbSphereProjector.h Inventor/projectors/SbSphereProjector.h
  \brief The SbSphereProjector class is the abstract base class for mapping to spherical surfaces.
  \ingroup projectors

  The sphere projectors map 2D points to various surface types based
  on spherical shapes.

  \sa SbCylinderProjector
 */

#include <Inventor/projectors/SbSphereProjector.h>

/*!
  \fn SbRotation SbSphereProjector::getRotation(const SbVec3f & point1, const SbVec3f & point2)

  Returns rotation on the projection surface which re-orients \a
  point1 to \a point2.
*/

/*!
  \var SbSphereProjector::intersectFront

  Flag which says whether or not we should map to the outside or
  inside of the sphere surface.
*/
/*!
  \var SbSphereProjector::sphere

  Projection sphere.
*/
/*!
  \var SbSphereProjector::orientToEye

  Which direction the spherical surface is oriented.
*/
/*!
  \var SbSphereProjector::needSetup

  Set to \c TRUE whenever the projection surface needs to be
  recalculated according to the setting of the
  SbSphereProjector::orientToEye flag.
*/
/*!
  \var SbSphereProjector::lastPoint

  Stores the previously projected 3D point.
*/



/*!
  Default constructor sets up a sphere at the origin with radius 1.
*/
SbSphereProjector::SbSphereProjector(const SbBool orienttoeye)
  : intersectFront(TRUE),
    sphere(SbVec3f(0.0f, 0.0f, 0.0f), 1.0f),
    orientToEye(orienttoeye),
    needSetup(TRUE),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

/*!
  Constructor taking an explicit \a sphere projection definition.
*/
SbSphereProjector::SbSphereProjector(const SbSphere & s,
                                     const SbBool orienttoeye)
  : intersectFront(TRUE),
    sphere(s),
    orientToEye(orienttoeye),
    needSetup(TRUE),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

/*!
  Project the 2D point to a 3D coordinate on the spherical surface,
  and find the rotation from the last projection to this one.

  \sa project(), getRotation()
*/
SbVec3f
SbSphereProjector::projectAndGetRotation(const SbVec2f & point,
                                         SbRotation & rot)
{
  SbVec3f lastpt = this->lastPoint;
  SbVec3f newpt = this->project(point);
  this->lastPoint = newpt;
  rot = this->getRotation(lastpt, newpt);
  return newpt;
}

/*!
  Set \a sphere to project onto.
*/
void
SbSphereProjector::setSphere(const SbSphere & sph)
{
  this->sphere = sph;
  this->needSetup = TRUE;
}

/*!
  Returns projection sphere.
*/
const SbSphere &
SbSphereProjector::getSphere(void) const
{
  return this->sphere;
}

/*!
  Sets whether or not the projection surface should be oriented
  towards the eye of the viewer. Default is \c TRUE.
*/
void
SbSphereProjector::setOrientToEye(const SbBool orienttoeye)
{
  this->orientToEye = orienttoeye;
  this->needSetup = TRUE;
}

/*!
  Returns the state of the sphere orientation flag.
*/
SbBool
SbSphereProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  Set whether to intersect with the outside of the sphere (\a infront
  equal to \c TRUE), or the inside.
*/
void
SbSphereProjector::setFront(const SbBool infront)
{
  this->intersectFront = infront;
  this->needSetup = TRUE;
}

/*!
  Returns value of the flag which decides whether to intersect with
  the outside or inside of the sphere.
*/
SbBool
SbSphereProjector::isFront(void) const
{
  return this->intersectFront;
}

/*!
  Check if \a point is on the frontside or the backside of the
  cylinder.
*/
SbBool
SbSphereProjector::isPointInFront(const SbVec3f & point) const
{
  const SbViewVolume & vv = this->getViewVolume();
  SbVec3f camdir;
  if (vv.getProjectionType() == SbViewVolume::PERSPECTIVE) {
    SbVec3f campos;
    this->worldToWorking.multVecMatrix(vv.getProjectionPoint(), campos);
    camdir = campos - this->sphere.getCenter();
  }
  else {
    this->worldToWorking.multDirMatrix( vv.zVector(), camdir);
  }
  SbVec3f ptdir = point - this->sphere.getCenter();
  return ptdir.dot(camdir) >= 0.0f;
}

/*!
  Intersect \a line with the SbSphereProjector::sphere and place the
  intersection point (if any) in \a result. Considers setFront()
  settings.

  Returns \c TRUE if \a line actually hits the sphere, \c FALSE if it
  doesn't intersect with it.
*/
SbBool
SbSphereProjector::intersectSphereFront(const SbLine & l, SbVec3f & result)
{
  SbVec3f i0, i1;
  if (this->sphere.intersect(l, i0, i1)) {
    if (this->isFront()) result = i0;
    else result = i1;
    return TRUE;
  }
  return FALSE;
}

// Documented in superclass.
void
SbSphereProjector::setWorkingSpace(const SbMatrix & space)
{
  this->needSetup = TRUE;
  inherited::setWorkingSpace(space);
}
