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

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


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
SbPlaneProjector::SbPlaneProjector(const SbBool orient)
  : plane(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f),
    nonOrientPlane(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f),
    orientToEye(orient),
    needSetup(orient) // will need setup if orient-to-plane
{
}

/*!
  FIXME: write doc
*/
SbPlaneProjector::SbPlaneProjector(const SbPlane &plane, const SbBool orient)
  : plane(plane),
    nonOrientPlane(plane),
    orientToEye(orient),
    needSetup(orient) // will need setup if orient-to-plane
{
}

/*!
  FIXME: write doc
*/
SbVec3f
SbPlaneProjector::project(const SbVec2f & point)
{
  if (this->needSetup) this->setupPlane();

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;
  if (!this->plane.intersect(projline, projpt)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbPlaneProjector::project",
                              "working line is parallel to plane.");
#endif // COIN_DEBUG
    // set to (0,0,0) to avoid crazy rotations
    projpt = SbVec3f(0.0f, 0.0f, 0.0f);
  }
  return projpt;
}

/*!
  FIXME: write doc
*/
void
SbPlaneProjector::setOrientToEye(const SbBool orientToEye)
{
  if (orientToEye != this->orientToEye) {
    this->orientToEye = orientToEye;
    this->needSetup = TRUE;
  }
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
SbPlaneProjector::setPlane(const SbPlane &plane)
{
  this->plane = plane;
  this->needSetup = TRUE;
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
SbPlaneProjector::getVector(const SbVec2f &mousePosition1,
                            const SbVec2f &mousePosition2)
{
  SbVec3f mp1 = this->project(mousePosition1);
  SbVec3f mp2 = this->project(mousePosition2);
  this->lastPoint = mp2; // FIXME: correct? pederb, 1999-12-06
  return mp2 - mp1;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbPlaneProjector::getVector(const SbVec2f &mousePosition)
{
  SbVec3f lp = this->lastPoint; // lastPoint is updated in project()
  return (this->project(mousePosition) - lp);
}

/*!
  FIXME: write doc
*/
void
SbPlaneProjector::setStartPosition(const SbVec2f &mousePosition)
{
  this->lastPoint = this->project(mousePosition);
}

/*!
  FIXME: write doc
*/
void
SbPlaneProjector::setStartPosition(const SbVec3f &point)
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
  Should be called whenever needSetup is \a TRUE. Will calculate \e plane.
*/
void
SbPlaneProjector::setupPlane(void)
{
  if (this->orientToEye) {
    SbVec3f pnt = -this->nonOrientPlane.getNormal() *
      this->nonOrientPlane.getDistanceFromOrigin();
    SbVec3f dir = -this->viewVol.getProjectionDirection();
    this->worldToWorking.multDirMatrix(dir, dir);
    this->plane = SbPlane(dir, pnt);
  }
  else {
    this->plane = this->nonOrientPlane;
  }
  this->needSetup = FALSE;
}
