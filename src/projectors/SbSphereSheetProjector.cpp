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
  \class SbSphereSheetProjector SbSphereSheetProjector.h Inventor/projectors/SbSphereSheetProjector.h
  \brief The SbSphereSheetProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: doc
 */

/*¡
  Outside the sphere, points will map to a plane -- not a hyperbolical
  sheet, as they are supposed to be.
*/

#include <Inventor/projectors/SbSphereSheetProjector.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>


/*!
  \var SbSphereSheetProjector::workingProjPoint
*/
/*!
  \var SbSphereSheetProjector::planePoint
*/
/*!
  \var SbSphereSheetProjector::planeDir
*/
/*!
  \var SbSphereSheetProjector::planeDist
*/
/*!
  \var SbSphereSheetProjector::tolPlane
*/


/*!
  FIXME: write doc
*/
SbSphereSheetProjector::SbSphereSheetProjector(SbBool orientToEye)
  : SbSphereProjector(orientToEye)
{
}

/*!
  FIXME: write doc
*/
SbSphereSheetProjector::SbSphereSheetProjector(const SbSphere &sph,
					       SbBool orientToEye)
  : SbSphereProjector(sph, orientToEye)
{
}
    
/*!
  FIXME: write doc
*/
SbProjector *
SbSphereSheetProjector::copy(void) const
{
  // TODO: implement
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbSphereSheetProjector::project(const SbVec2f &point)
{
  // FIXME: orientToEye==TRUE not handled. 19990329 mortene.
  // FIXME: setFront==FALSE not handled. 19990329 mortene.

  SbLine projline = this->getWorkingLine(point);
  SbVec3f projpt;

  SbBool isect = this->sphere.intersect(projline, projpt);
  if (!isect) {
    // FIXME: make code for calculating off-sphere intersections
    // against a hyperbolic sheet instead of a straight
    // plane. Formula: z = a*x^2 + b*y^2 + c, where sign of a differs
    // from sign of b (???). 19990401 mortene.
    SbPlane plane(SbVec3f(0.0f, 0.0f, 1.0f), SbVec3f(0.0f, 0.0f, 0.0f));
    isect = plane.intersect(projline, projpt);
    if (!isect) projpt = this->lastPoint;
    else this->lastPoint = projpt;
  }
  else this->lastPoint = projpt;
  return projpt;
}

/*!
  FIXME: write doc
*/
SbRotation
SbSphereSheetProjector::getRotation(const SbVec3f & point1,
				    const SbVec3f & point2)
{
#if 0 // debug
  SoDebugError::post("SbSphereSheetProjector::getRotation",
		     "point1: <%f, %f, %f>, point2: <%f, %f, %f>",
		     point1[0], point1[1], point1[2],
		     point2[0], point2[1], point2[2]);
#endif // debug
  return SbRotation(point1, point2);
}

/*!
  FIXME: write doc
*/
void
SbSphereSheetProjector::setupPlane(void)
{
  // FIXME: implement
  assert(0);
}
