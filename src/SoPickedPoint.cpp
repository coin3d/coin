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
  \class SoPickedPoint SoPickedPoint.h Inventor/SoPickedPoint.h
  \brief The SoPickedPoint class ... blablabla ...

  FIXME: write doc
 */

#include <Inventor/SoPickedPoint.h>
#include <assert.h>

/*!
  FIXME: write doc
 */
SoPickedPoint::SoPickedPoint(const SoPickedPoint &pp)
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
SoPickedPoint::SoPickedPoint(const SoPath * const path, SoState * const state,
			     const SbVec3f &objSpacePoint)
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
SoPickedPoint::~SoPickedPoint()
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
SoPickedPoint *
SoPickedPoint::copy(void) const
{
  // FIXME: implement.
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPickedPoint::getPoint(void) const
{
  // FIXME: implement.
  assert(0);
  static SbVec3f v(0,0,0);
  return v;
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPickedPoint::getNormal(void) const
{
  // FIXME: implement.
  assert(0);
  static SbVec3f v(0,0,0);
  return v;
}

/*!
  FIXME: write doc
 */
const SbVec4f &
SoPickedPoint::getTextureCoords(void) const
{
  // FIXME: implement.
  assert(0);
  static SbVec4f v(0,0,0,0);
  return v;
}

/*!
  FIXME: write doc
 */
int
SoPickedPoint::getMaterialIndex(void) const
{
  // FIXME: implement.
  assert(0);
  return 0;
}

/*!
  FIXME: write doc
 */
SoPath *
SoPickedPoint::getPath(void) const
{
  // FIXME: implement.
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
 */
SbBool 
SoPickedPoint::isOnGeometry(void) const
{
  // FIXME: implement.
  assert(0);
  return FALSE;
}

/*!
  FIXME: write doc
 */
const SoDetail *
SoPickedPoint::getDetail(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return NULL;
}

/*!
  FIXME: write doc
 */
const SbMatrix
SoPickedPoint::getObjectToWorld(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbMatrix::identity();
}

/*!
  FIXME: write doc
 */
const SbMatrix
SoPickedPoint::getWorldToObject(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbMatrix::identity();
}

/*!
  FIXME: write doc
 */
const SbMatrix
SoPickedPoint::getObjectToImage(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbMatrix::identity();
}

/*!
  FIXME: write doc
 */
const SbMatrix
SoPickedPoint::getImageToObject(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbMatrix::identity();
}

/*!
  FIXME: write doc
 */
const SbVec3f
SoPickedPoint::getObjectPoint(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbVec3f(0,0,0);
}

/*!
  FIXME: write doc
 */
const SbVec3f
SoPickedPoint::getObjectNormal(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbVec3f(0,0,0);
}

/*!
  FIXME: write doc
 */
const SbVec4f
SoPickedPoint::getObjectTextureCoords(const SoNode * const node) const
{
  // FIXME: implement.
  assert(0);
  return SbVec4f(0,0,0,0);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setObjectNormal(const SbVec3f &normal)
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setObjectTextureCoords(const SbVec4f &texCoords)
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setMaterialIndex(const int index)
{
  // FIXME: implement.
  assert(0);
}

/*!
  FIXME: write doc
 */
void
SoPickedPoint::setDetail(SoDetail *detail, SoNode *node)
{
  // FIXME: implement.
  assert(0);
}
