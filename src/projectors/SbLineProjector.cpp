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
  \class SbLineProjector Inventor/projectors/SbLineProjector.h
  \brief The SbLineProjector class is ... blablabla TODO.
  \ingroup projectors

  FIXME: write class doc.
*/

#include <assert.h>
#include <Inventor/projectors/SbLineProjector.h>

/*!
  \var SbLineProjector::line
  FIXME: write doc
*/
/*!
  \var SbLineProjector::lastPoint
  FIXME: write doc
*/




/*!
  Constructor.
 */
SbLineProjector::SbLineProjector(void)
  : line(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 1.0f, 0.0f)),
    lastPoint(0.0f, 0.0f, 0.0f)
{
}

/*!
  FIXME: write doc
*/
SbVec3f
SbLineProjector::project(const SbVec2f& point)
{
  SbLine projline = this->getWorkingLine(point);
  SbVec3f thispt, projpt;
  SbBool nonparallel = this->line.getClosestPoints(projline, thispt, projpt);
  // if lines are parallel, we will never get an intersection, and
  // we set projection point to (0,0,0) to avoid strange rotations
  if (!nonparallel) thispt = SbVec3f(0.0f, 0.0f, 0.0f);
  this->lastPoint = thispt;
  return thispt;
}

/*!
  Set a new line.
 */
void
SbLineProjector::setLine(const SbLine& line)
{
  this->line = line;
}

/*!
  Returns the currently set line.
 */
const SbLine&
SbLineProjector::getLine(void) const
{
  return this->line;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbLineProjector::getVector(const SbVec2f& mousePosition1,
                           const SbVec2f& mousePosition2)
{
  SbVec3f mp1 = this->project(mousePosition1);
  SbVec3f mp2 = this->project(mousePosition2);
  this->lastPoint = mp2;
  return mp2 - mp1;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbLineProjector::getVector(const SbVec2f& mousePosition)
{
  SbVec3f lp = this->lastPoint; // lastPoint is updated in project()
  return (this->project(mousePosition) - lp);
}

/*!
  FIXME: write doc
*/
void
SbLineProjector::setStartPosition(const SbVec2f& mousePosition)
{
  this->lastPoint = this->project(mousePosition);
}

/*!
  FIXME: write doc
*/
void
SbLineProjector::setStartPosition(const SbVec3f& point)
{
  this->lastPoint = point;
}

/*!
  Make an exact copy of the SbLineProjector instance. The caller will be
  responsible for destroying the new instance.
 */
SbProjector *
SbLineProjector::copy(void) const
{
  return new SbLineProjector(*this);
}
