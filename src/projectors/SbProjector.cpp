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
  \class SbProjector SbProjector.h Inventor/projectors/SbProjector.h
  \brief The SbProjector class is the base class for all other projector
  classes.
  \ingroup projectors

  FIXME: write class doc.
*/

#include <Inventor/projectors/SbProjector.h>

/*!
  \fn virtual SbVec3f SbProjector::project(const SbVec2f & point)
  FIXME: write doc
 */
/*!
  \fn virtual SbProjector * SbProjector::copy(void) const
  FIXME: write doc
 */

/*!
  \var SbProjector::viewVol
  FIXME: write doc
*/
/*!
  \var SbProjector::worldToWorking
  FIXME: write doc
*/
/*!
  \var SbProjector::workingToWorld
  FIXME: write doc
*/


/*!
  The constructor initializes the workingspace matrix to an identity
  matrix.
 */
SbProjector::SbProjector(void)
{
  this->worldToWorking.makeIdentity();
  this->workingToWorld.makeIdentity();
}

/*!
  Set the viewing volume the projections will take place in.

  \sa getViewVolume().
 */
void
SbProjector::setViewVolume(const SbViewVolume &vol)
{
  this->viewVol = vol;
}

/*!
  Return the current viewing volume used by the projections.

  \sa setViewVolume().
 */
const SbViewVolume &
SbProjector::getViewVolume(void) const
{
  return this->viewVol;
}

/*!
  FIXME: write doc
 */
void
SbProjector::setWorkingSpace(const SbMatrix &space)
{
  this->workingToWorld = space;
  this->worldToWorking = space.inverse();
}

/*!
  FIXME: write doc
 */
const SbMatrix &
SbProjector::getWorkingSpace(void) const
{
  return this->workingToWorld;
}

/*!
  FIXME: write doc
 */
SbLine
SbProjector::getWorkingLine(const SbVec2f &point) const
{
  SbLine l;
  this->viewVol.projectPointToLine(point, l);
  this->worldToWorking.multLineMatrix(l, l);
  return l;
}
