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
  \class SoPrimitiveVertex SoPrimitiveVertex.h Inventor/SoPrimitiveVertex.h
  \brief The SoPrimitiveVertex class FIXME: doc
  \ingroup general

  FIXME: document properly
*/

#include <Inventor/SoPrimitiveVertex.h>
#include <stdlib.h>

/*!
  FIXME: write doc
 */
SoPrimitiveVertex::SoPrimitiveVertex(void)
  : point(0.0f, 0.0f, 0.0f),
    normal(0.0f, 0.0f, 1.0f),
    textureCoords(0.0f, 0.0f, 0.0f, 1.0f),
    materialIndex(0),
    detail(NULL)
{
}

/*!
  FIXME: write doc
 */
SoPrimitiveVertex &
SoPrimitiveVertex::operator =(const SoPrimitiveVertex & pv)
{
  this->point = pv.point;
  this->normal = pv.normal;
  this->textureCoords = pv.textureCoords;
  this->materialIndex = pv.materialIndex;
  this->detail = pv.detail;
  return *this;
}

/*!
  FIXME: write doc
 */
SoPrimitiveVertex::SoPrimitiveVertex(const SoPrimitiveVertex & pv)
{
  *this = pv;
}

/*!
  FIXME: write doc
 */
SoPrimitiveVertex::~SoPrimitiveVertex()
{
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPrimitiveVertex::getPoint(void) const
{
  return this->point;
}

/*!
  FIXME: write doc
 */
const SbVec3f &
SoPrimitiveVertex::getNormal(void) const
{
  return this->normal;
}

/*!
  FIXME: write doc
 */
const SbVec4f &
SoPrimitiveVertex::getTextureCoords(void) const
{
  return this->textureCoords;
}

/*!
  FIXME: write doc
 */
int
SoPrimitiveVertex::getMaterialIndex(void) const
{
  return this->materialIndex;
}

/*!
  FIXME: write doc
 */
const SoDetail *
SoPrimitiveVertex::getDetail(void) const
{
  return this->detail;
}

/*!
  FIXME: write doc
 */
void
SoPrimitiveVertex::setPoint(const SbVec3f & point)
{
  this->point = point;
}

/*!
  FIXME: write doc
 */
void
SoPrimitiveVertex::setNormal(const SbVec3f & normal)
{
  this->normal = normal;
}

/*!
  FIXME: write doc
 */
void
SoPrimitiveVertex::setTextureCoords(const SbVec4f & texcoords)
{
  this->textureCoords = texcoords;
}

/*!
  Covenience function. Will fill in 0 and 1 in the last two coords
*/
void
SoPrimitiveVertex::setTextureCoords(const SbVec2f & texcoords)
{
  this->textureCoords = SbVec4f(texcoords[0], texcoords[1], 0.0f, 1.0f);
}

/*!
  Covenience function. Will fill in 1 in the last coord

  \since 2001-11-26
*/
void
SoPrimitiveVertex::setTextureCoords(const SbVec3f & texcoords)
{
  this->textureCoords = SbVec4f(texcoords[0], texcoords[1], texcoords[2],
                                1.0f);
}

/*!
  FIXME: write doc
 */
void
SoPrimitiveVertex::setMaterialIndex(const int index)
{
  this->materialIndex = index;
}

/*!
  FIXME: write doc
 */
void
SoPrimitiveVertex::setDetail(SoDetail * const detail)
{
  this->detail = detail;
}
