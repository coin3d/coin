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
  \class SbPlaneProjector Inventor/projectors/SbPlaneProjector.h
  \brief The SbPlaneProjector class is ... blablabla TODO.
  \ingroup projectors

  TODO: class doc.
*/

#include <assert.h>
#include <Inventor/projectors/SbPlaneProjector.h>

/*!
  \var SbPlaneProjector::plane
  FIXME: write doc
*/
/*!
  \var SbPlaneProjector::orientToEye
  FIXME: write doc
*/
/*!
  \var SbPlaneProjector::lastPoint
  FIXME: write doc
*/



/*!
  Constructor.
 */
SbPlaneProjector::SbPlaneProjector(SbBool orient)
  : plane(SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(0.0f, 0.0f, 0.0f)),
    lastPoint(0.0f, 0.0f, 0.0f),
    orientToEye(orient)
{
  // FIXME: orientToEye==TRUE not handled. 19990329 mortene.
}

/*!
  FIXME: write doc
 */
SbPlaneProjector::SbPlaneProjector(const SbPlane & plane, SbBool orient)
  : plane(plane),
    lastPoint(0.0f, 0.0f, 0.0f),
    orientToEye(orient)
{
  // FIXME: orientToEye==TRUE not handled. 19990329 mortene.
}

/*!
  Destructor.
 */
SbPlaneProjector::~SbPlaneProjector(void)
{
}

/*!
  FIXME: write doc
 */
SbVec3f
SbPlaneProjector::project(const SbVec2f & point)
{
  // FIXME: orientToEye==TRUE not handled. 19990329 mortene.

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;
  SbBool isect = this->plane.intersect(projline, projpt);
  // FIXME: is this a good way of handling the case of parallel lines?
  // 19990329 mortene.
  if (!isect) projpt = this->lastPoint;
  else this->lastPoint = projpt;
  return projpt;
}

/*!
  FIXME: write doc
 */
void
SbPlaneProjector::setOrientToEye(SbBool orientToEye)
{
  this->orientToEye = orientToEye;
  // FIXME: orientToEye==TRUE not handled. 19990329 mortene.
}

/*!
  FIXME: write doc
 */
SbBool
SbPlaneProjector::isOrientToEye(void) const
{
  return this->orientToEye;
}

/*!
  Set a new plane.
 */
void
SbPlaneProjector::setPlane(const SbPlane & plane)
{
  this->plane = plane;
}

/*!
  Returns the currently set plane.
 */
const SbPlane &
SbPlaneProjector::getPlane(void) const
{
  return this->plane;
}

/*!
  FIXME: write doc
 */
SbVec3f
SbPlaneProjector::getVector(const SbVec2f& mousePosition1,
			    const SbVec2f& mousePosition2)
{
  // Must precalculate these, so lastPoint is finally set to the value
  // of the projected mousePosition2.
  SbVec3f mp1 = this->project(mousePosition1);
  SbVec3f mp2 = this->project(mousePosition2);

  return mp2 - mp1;
}

/*!
  FIXME: write doc
 */
SbVec3f
SbPlaneProjector::getVector(const SbVec2f& mousePosition)
{
  SbVec3f lp = this->lastPoint; // lastPoint is updated in project()
  return (this->project(mousePosition) - lp);
}

/*!
  FIXME: write doc
 */
void
SbPlaneProjector::setStartPosition(const SbVec2f& mousePosition)
{
  this->lastPoint = this->project(mousePosition);
}

/*!
  FIXME: write doc
 */
void
SbPlaneProjector::setStartPosition(const SbVec3f& point)
{
  this->lastPoint = point;
}

/*!
  Make an exact copy of the SbPlaneProjector instance. The caller will be
  responsible for destroying the new instance.
 */
SbProjector *
SbPlaneProjector::copy(void) const
{
  return new SbPlaneProjector(*this);
}

/*!
  FIXME: write doc
 */
void
SbPlaneProjector::setupPlane(void)
{
  // FIXME: implement
  assert(0);
}
