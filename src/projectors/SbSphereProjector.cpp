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
SbSphereProjector::SbSphereProjector(SbBool orientToEye)
{
  this->intersectFront = TRUE;
  this->sphere.setValue(SbVec3f(0.0f, 0.0f, 0.0f), 1.0f);
  this->orientToEye = orientToEye;
  this->needSetup = FALSE;
  this->lastPoint.setValue(0.0f, 0.0f, 0.0f);
}

/*!
  FIXME: write doc
*/
SbSphereProjector::SbSphereProjector(const SbSphere & s, SbBool orientToEye)
{
  this->intersectFront = TRUE;
  this->sphere = s;
  this->orientToEye = orientToEye;
  this->needSetup = FALSE;
  this->lastPoint.setValue(0.0f, 0.0f, 0.0f);
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
SbSphereProjector::setOrientToEye(SbBool orientToEye)
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
SbSphereProjector::setFront(SbBool inFront)
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
SbSphereProjector::isPointInFront(const SbVec3f & /* point */) const
{
  // FIXME: implement.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereProjector::intersectSphereFront(const SbLine & /* l */,
					SbVec3f & /* result */)
{
  // FIXME: implement.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SbSphereProjector::setWorkingSpace(const SbMatrix & /* space */)
{
  // FIXME: implement.
  assert(0);
}
