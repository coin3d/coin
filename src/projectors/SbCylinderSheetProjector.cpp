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
  \class SbCylinderSheetProjector SbCylinderSheetProjector.h Inventor/projectors/SbCylinderSheetProjector.h
  \brief The SbCylinderSheetProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderSheetProjector.h>
#include <assert.h>

/*! \var SbCylinderSheetProjector::workingProjPoint
  FIXME: write doc
*/
/*! \var SbCylinderSheetProjector::planeDir
  FIXME: write doc
*/
/*! \var SbCylinderSheetProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(SbBool orientToEye)
  : inherited(orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbCylinderSheetProjector::SbCylinderSheetProjector(const SbCylinder & cyl,
                                                   SbBool orientToEye)
  : inherited(cyl, orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbProjector *
SbCylinderSheetProjector::copy(void) const
{
  assert(0 && "FIXME: implementation missing");
  return NULL;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderSheetProjector::project(const SbVec2f & /* point */)
{
  assert(0 && "FIXME: implementation missing");
  return SbVec3f();
}

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderSheetProjector::getRotation(const SbVec3f & /* point1 */,
                                      const SbVec3f & /* point2 */)
{
  assert(0 && "FIXME: implementation missing");
  return SbRotation::identity();
}

/*!
  FIXME: write doc
*/
void
SbCylinderSheetProjector::setupPlane(void)
{
  assert(0 && "FIXME: implementation missing");
}
