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
  \class SbLineProjector SbLineProjector.h Inventor/projectors/SbLineProjector.h
  \brief The SbLineProjector class projects 2D points to 3D points along a line.
  \ingroup projectors

  The 3D projection of the 2D coordinates is for this projector class
  constrained to lie along a pre-defined line.

  Among other places, this is useful within the translation draggers,
  like for instance SoTranslate1Dragger, where we want to move
  "pieces" along one or more axes.
*/

#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbVec2f.h>
#include <assert.h>

/*!
  \var SbLineProjector::line

  The projection line. Projected 3D points will be constrained to be
  on this line.
*/
/*!
  \var SbLineProjector::lastPoint

  The last projected point.
*/


/*!
  Constructor. Intializes the projector instance to use a line from
  <0, 0, 0> to <0, 1, 0>.
 */
SbLineProjector::SbLineProjector(void)
  : line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f)),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

// Documented in superclass.
SbVec3f
SbLineProjector::project(const SbVec2f & point)
{
  // first project the line into screen space to find the 2D point
  // closest to the projection line there. Then use that 2D point to
  // find the best projection point.
  SbLine wrldline;
  this->workingToWorld.multLineMatrix(this->line, wrldline);

  SbVec3f pt1 = wrldline.getPosition();
  SbVec3f pt2 = pt1 + wrldline.getDirection();
  
  SbVec3f lineorigin = wrldline.getPosition();

  this->viewVol.projectToScreen(pt1, pt1);
  this->viewVol.projectToScreen(pt2, pt2);

  pt1[2] = 0.0f;
  pt2[2] = 0.0f;

  SbVec2f newpt = point;
  
  if (pt1 == pt2) newpt = SbVec2f(pt1[0], pt2[0]);
  else {
    SbLine scrline(pt1, pt2);
    SbVec3f dummy = scrline.getClosestPoint(SbVec3f(point[0], point[1], 0.0f));
    newpt = SbVec2f(dummy[0], dummy[1]);
  }

  SbLine projline = this->getWorkingLine(newpt);
  SbVec3f projpt, dummy;
  SbBool nonparallel = this->line.getClosestPoints(projline, projpt, dummy);

  // if lines are parallel, we will never get an intersection, and
  // we set projection point to the middle of the view volume
  if (!nonparallel) {
    float depth = this->viewVol.getNearDist() +
      this->viewVol.getDepth() * 0.5f;
    SbPlane plane = this->viewVol.getPlane(depth);
    if (!plane.intersect(wrldline, projpt)) {
      assert(0 && "should never happen");
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
    else this->worldToWorking.multVecMatrix(projpt, projpt);
  }
  else if (!this->verifyProjection(projpt)) {
    float depth = this->findVanishingDistance();
    SbPlane plane = this->viewVol.getPlane(depth);
    if (!plane.intersect(wrldline, projpt)) {
      assert(0 && "should never happen");
      projpt = SbVec3f(0.0f, 0.0f, 0.0f);
    }
    else {
      this->worldToWorking.multVecMatrix(projpt, projpt);
    }
  }
  this->lastPoint = projpt;
  return projpt;
}

/*!
  Set a new projection line. 3D points will be mapped to be on this
  line.
 */
void
SbLineProjector::setLine(const SbLine & line)
{
  this->line = line;
}

/*!
  Returns the currently set projection line.
 */
const SbLine&
SbLineProjector::getLine(void) const
{
  return this->line;
}

/*!
  Calculates and returns a vector between the projected 3D position of
  \a viewpos1 and \a viewpos2.
*/
SbVec3f
SbLineProjector::getVector(const SbVec2f & viewpos1, const SbVec2f & viewpos2)
{
  SbVec3f mp1 = this->project(viewpos1);
  SbVec3f mp2 = this->project(viewpos2);
  this->lastPoint = mp2;
  return mp2 - mp1;
}

/*!
  Returns the 3D vector between the last projection and the one
  calculated for \a viewpos.
*/
SbVec3f
SbLineProjector::getVector(const SbVec2f & viewpos)
{
  SbVec3f lp = this->lastPoint; // lastPoint is updated in project()
  return (this->project(viewpos) - lp);
}

/*!
  Explicitly set position of initial projection, so we get correct
  values for later calls to getVector() etc.
*/
void
SbLineProjector::setStartPosition(const SbVec2f & viewpos)
{
  this->lastPoint = this->project(viewpos);
}

/*!
  Explicitly set position of initial projection, so we get correct
  values for later calls to getVector() etc.
*/
void
SbLineProjector::setStartPosition(const SbVec3f & point)
{
  this->lastPoint = point;
}

// Documented in superclass.
SbProjector *
SbLineProjector::copy(void) const
{
  return new SbLineProjector(*this);
}
