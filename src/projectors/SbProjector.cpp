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
  \class SbProjector SbProjector.h Inventor/projectors/SbProjector.h
  \brief The SbProjector class is the abstract base projector class.
  \ingroup projectors

  Projectors are used in the Coin library for mapping 2D coordinates
  (typically from the position of the mouse cursor in the rendering
  window) to 3D "world" coordinates.

  Mapping 2D coordinates to 3D coordinates is something which is done
  extensively in the dragger classes, to provide the user with a
  convenient and natural way of interacting with the 3D geometry of
  scenes.

  The application programmer should normally not need to care about
  the projector classes, unless there are special needs in the
  application.

  \sa SoDragger
*/


#include <Inventor/projectors/SbProjector.h>


/*!
  \fn SbProjector::~SbProjector()

  Destructor is protected, as this is an abstract class.
 */

/*!
  \fn virtual SbVec3f SbProjector::project(const SbVec2f & point)

  Project the 2D \a point from normalized viewport coordinates to a 3D
  point. The mapping will be done in accordance with the type of the
  projector.
 */
/*!
  \fn virtual SbProjector * SbProjector::copy(void) const

  Construct and return a copy of this projector. The caller is
  responsible for destructing the new instance.
 */

/*!
  \var SbProjector::viewVol

  The viewVol definition.
*/
/*!
  \var SbProjector::worldToWorking

  The matrix which converts from world coordinates to coordinates in
  the projector's local coordinate system.
*/
/*!
  \var SbProjector::workingToWorld

  The matrix which converts from coordinates in the projector's local
  coordinate system to world coordinates.
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

  \sa getViewVolume()
 */
void
SbProjector::setViewVolume(const SbViewVolume & vol)
{
  this->viewVol = vol;
}

/*!
  Return the current viewing volume used by the projections.

  \sa setViewVolume()
 */
const SbViewVolume &
SbProjector::getViewVolume(void) const
{
  return this->viewVol;
}

/*!
  Sets the matrix used for converting from the projector's coordinate
  system to the world coordinate system.
 */
void
SbProjector::setWorkingSpace(const SbMatrix & space)
{
  this->workingToWorld = space;
  this->worldToWorking = space.inverse();
}

/*!
  Returns projector-to-world matrix.

  \sa setWorkingSpace()
 */
const SbMatrix &
SbProjector::getWorkingSpace(void) const
{
  return this->workingToWorld;
}

/*!
  From the 2D \a point in normalized screenspace coordinates,
  calculate the line passing through the scene.

  Typically used for tracking intersection points for the mouse
  cursor.
 */
SbLine
SbProjector::getWorkingLine(const SbVec2f & point) const
{
  SbLine l;
  this->viewVol.projectPointToLine(point, l);
  this->worldToWorking.multLineMatrix(l, l);
  return l;
}
