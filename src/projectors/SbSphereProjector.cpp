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
  \class SbSphereProjector SbSphereProjector.h Inventor/projectors/SbSphereProjector.h
  \brief The SbSphereProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbSphereProjector.h>
#include <assert.h>

/*!
  \fn SbRotation SbSphereProjector::getRotation(const SbVec3f & point1, const SbVec3f & point2)
  FIXME: write doc
*/

/*!
  \var SbSphereProjector::intersectFront
  FIXME: write doc
*/
/*!
  \var SbSphereProjector::sphere
  FIXME: write doc
*/
/*!
  \var SbSphereProjector::orientToEye
  FIXME: write doc
*/
/*!
  \var SbSphereProjector::needSetup
  FIXME: write doc
*/
/*!
  \var SbSphereProjector::lastPoint
  FIXME: write doc
*/



/*!
  FIXME: write doc
*/
SbSphereProjector::SbSphereProjector(const SbBool orient)
  : intersectFront(TRUE),
    sphere(SbVec3f(0.0f, 0.0f, 0.0f), 1.0f),
    orientToEye(orient),
    needSetup(TRUE),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

/*!
  FIXME: write doc
*/
SbSphereProjector::SbSphereProjector(const SbSphere &s,
                                     const SbBool orient)
  : intersectFront(TRUE),
    sphere(s),
    orientToEye(orient),
    needSetup(TRUE),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

/*!
  FIXME: write doc
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
  FIXME: write doc
*/
void
SbSphereProjector::setSphere(const SbSphere & sph)
{
  this->sphere = sph;
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
const SbSphere &
SbSphereProjector::getSphere(void) const
{
  return this->sphere;
}

/*!
  FIXME: write doc
*/
void
SbSphereProjector::setOrientToEye(const SbBool orientToEye)
{
  this->orientToEye = orientToEye;
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  FIXME: write doc
*/
void
SbSphereProjector::setFront(const SbBool inFront)
{
  this->intersectFront = inFront;
  this->needSetup = TRUE;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereProjector::isFront(void) const
{
  return this->intersectFront;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereProjector::isPointInFront(const SbVec3f &point) const
{
  SbVec3f dir;
  if (this->orientToEye) {
    dir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(dir, dir);
  }
  else dir = SbVec3f(0.0f, 0.0f, 1.0f);

  SbVec3f vec = point - this->sphere.getCenter();;

  float dot = vec.dot(dir);
  if (this->intersectFront) return dot >= 0.0f;
  else return dot < 0.0f;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereProjector::intersectSphereFront(const SbLine &l,
                                        SbVec3f &result)
{
  SbVec3f i0, i1;
  if (this->sphere.intersect(l, i0, i1)) {
    if (this->isPointInFront(i0)) result = i0;
    else result = i1;
    return TRUE;
  }
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SbSphereProjector::setWorkingSpace(const SbMatrix &space)
{
  this->needSetup = TRUE;
  inherited::setWorkingSpace(space);
}
