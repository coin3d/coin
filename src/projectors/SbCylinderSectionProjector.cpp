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
  \class SbCylinderSectionProjector SbCylinderSectionProjector.h Inventor/projectors/SbCylinderSectionProjector.h
  \brief The SbCylinderSectionProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderSectionProjector.h>
#include <assert.h>

/*! \var SbCylinderSectionProjector::tolerance
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::tolDist
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeDir
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeLine
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::planeDist
  FIXME: write doc
*/
/*! \var SbCylinderSectionProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(float /* edgeTol */,
						       SbBool orientToEye)
  : inherited(orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbCylinderSectionProjector::SbCylinderSectionProjector(const SbCylinder & cyl,
						       float /* edgeTol */,
						       SbBool orientToEye)
  : inherited(cyl, orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbProjector *
SbCylinderSectionProjector::copy(void) const
{
  assert(0 && "FIXME: implementation missing");
  return NULL;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderSectionProjector::project(const SbVec2f & /* point */)
{
  assert(0 && "FIXME: implementation missing");
  return SbVec3f();
}

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderSectionProjector::getRotation(const SbVec3f & /* point1 */,
					const SbVec3f & /* point2 */)
{
  assert(0 && "FIXME: implementation missing");
  return SbRotation::identity();
}

/*!
  FIXME: write doc
*/
void
SbCylinderSectionProjector::setTolerance(float /* edgeTol */)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
float
SbCylinderSectionProjector::getTolerance(void) const
{
  assert(0 && "FIXME: implementation missing");
  return 0.0f;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderSectionProjector::isWithinTolerance(const SbVec3f & /* point */)
{
  assert(0 && "FIXME: implementation missing");
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SbCylinderSectionProjector::setupTolerance(void)
{
  assert(0 && "FIXME: implementation missing");
}
