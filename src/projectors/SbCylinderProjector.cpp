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
  \class SbCylinderProjector SbCylinderProjector.h Inventor/projectors/SbCylinderProjector.h
  \brief The SbCylinderProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbCylinderProjector.h>
#include <Inventor/SbRotation.h>
#include <assert.h>

/*!
  \var SbCylinderProjector::intersectFront
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::cylinder
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::orientToEye
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::needSetup
  FIXME: write doc
*/
/*!
  \var SbCylinderProjector::lastPoint
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbCylinderProjector::SbCylinderProjector(SbBool /* orientToEye */)
{
  // FIXME: not implemented.
  assert(0);
}

/*!
  FIXME: write doc
*/
SbCylinderProjector::SbCylinderProjector(const SbCylinder & /* cyl */,
					 SbBool /* orientToEye */)
{
  // FIXME: not implemented.
  assert(0);
}

/*!
  FIXME: write doc
*/
SbVec3f
SbCylinderProjector::projectAndGetRotation(const SbVec2f &/* point */,
					   SbRotation &/* rot */)
{
  // FIXME: not implemented.
  assert(0);
  static SbVec3f v;
  return v;
}

/*!
  FIXME: write doc
*/
SbRotation
SbCylinderProjector::getRotation(const SbVec3f &/* point1 */,
				 const SbVec3f &/* point2 */)
{
  // FIXME: not implemented.
  assert(0);
  static SbRotation r;
  return r;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setCylinder(const SbCylinder &/* cyl */)
{
  // FIXME: not implemented.
  assert(0);
}

/*!
  FIXME: write doc
*/
const SbCylinder &
SbCylinderProjector::getCylinder(void) const
{
  // FIXME: not implemented.
  assert(0);
  static SbCylinder c;
  return c;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setOrientToEye(SbBool /* orientToEye */)
{
  // FIXME: not implemented.
  assert(0);
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isOrientToEye(void) const
{
  // FIXME: not implemented.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setFront(SbBool /* isFront */)
{
  // FIXME: not implemented.
  assert(0);
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isFront(void) const
{
  // FIXME: not implemented.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::isPointInFront(const SbVec3f &/* point */) const
{
  // FIXME: not implemented.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
*/
SbBool
SbCylinderProjector::intersectCylinderFront(const SbLine & /* line */,
					    SbVec3f & /* result */)
{
  // FIXME: not implemented.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SbCylinderProjector::setWorkingSpace(const SbMatrix & /* space */)
{
  // FIXME: not implemented.
  assert(0);
}
