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
  \class SbSphereSectionProjector SbSphereSectionProjector.h Inventor/projectors/SbSphereSectionProjector.h
  \brief The SbSphereSectionProjector class is ... blablabla FIXME.
  \ingroup projectors

  FIXME: write doc
 */

#include <Inventor/projectors/SbSphereSectionProjector.h>
#include <assert.h>

/*! \var SbSphereSectionProjector::tolerance
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::tolDist
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::radialFactor
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planePoint
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planeDir
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::planeDist
  FIXME: write doc
*/
/*! \var SbSphereSectionProjector::tolPlane
  FIXME: write doc
*/


/*!
  FIXME: write doc
*/
SbSphereSectionProjector::SbSphereSectionProjector(float edgeTol,
						   SbBool orientToEye)
  : inherited(orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbSphereSectionProjector::SbSphereSectionProjector(const SbSphere & sph,
						   float edgeTol,
						   SbBool orientToEye)
  : inherited(sph, orientToEye)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbSphereSectionProjector::~SbSphereSectionProjector()
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
SbProjector *
SbSphereSectionProjector::copy(void) const
{
  assert(0 && "FIXME: implementation missing");
  return NULL;
}

/*!
  FIXME: write doc
*/
SbVec3f
SbSphereSectionProjector::project(const SbVec2f & point)
{
  assert(0 && "FIXME: implementation missing");
  return SbVec3f();
}

/*!
  FIXME: write doc
*/
SbRotation
SbSphereSectionProjector::getRotation(const SbVec3f & point1,
				      const SbVec3f & point2)
{
  assert(0 && "FIXME: implementation missing");
  return SbRotation::identity();
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setTolerance(float edgeTol)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
float
SbSphereSectionProjector::getTolerance(void) const
{
  assert(0 && "FIXME: implementation missing");
  return 0.0f;
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setRadialFactor(float rad)
{
  assert(0 && "FIXME: implementation missing");
}

/*!
  FIXME: write doc
*/
float 
SbSphereSectionProjector::getRadialFactor(void) const
{
  assert(0 && "FIXME: implementation missing");
  return 0.0f;
}

/*!
  FIXME: write doc
*/
SbBool
SbSphereSectionProjector::isWithinTolerance(const SbVec3f & point)
{
  assert(0 && "FIXME: implementation missing");
  return FALSE;
}

/*!
  FIXME: write doc
*/
void
SbSphereSectionProjector::setupTolerance(void)
{
  assert(0 && "FIXME: implementation missing");
}
