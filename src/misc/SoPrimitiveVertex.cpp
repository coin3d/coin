/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoPrimitiveVertex SoPrimitiveVertex.h Inventor/SoPrimitiveVertex.h
  \brief The SoPrimitiveVertex class represents a single vertex of a generated primitive.
  \ingroup general

  Instances of SoPrimitiveVertex are constructed when generating
  primitive data, primarily during an SoCallbackAction traversal.
  Depending on the context the vertex could represent a single 3D
  point, one of the two vertices in a line or one of the three
  vertices in a triangle.
*/

#include <Inventor/SoPrimitiveVertex.h>
#include <stdlib.h>

/*!
  Default constructor, sets up a "void" instance.
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
  Copy operator.

  When \a pv is copied into this instance, a \e shallow copy is
  made. Ie, only the reference to the detail instance is copied (if
  any), not the detail itself.
*/
SoPrimitiveVertex &
SoPrimitiveVertex::operator=(const SoPrimitiveVertex & pv)
{
  this->point = pv.point;
  this->normal = pv.normal;
  this->textureCoords = pv.textureCoords;
  this->materialIndex = pv.materialIndex;
  this->detail = pv.detail;
  return *this;
}

/*!
  Copy constructor. Does a shallow copy.

  \sa SoPrimitiveVertex::operator=()
*/
SoPrimitiveVertex::SoPrimitiveVertex(const SoPrimitiveVertex & pv)
{
  *this = pv;
}

/*!
  Destructor. The detail instance is owned by client code and will not
  be destructed here.
*/
SoPrimitiveVertex::~SoPrimitiveVertex()
{
}

/*!
  Returns vertex coordinates, positioned in object space.
*/
const SbVec3f &
SoPrimitiveVertex::getPoint(void) const
{
  return this->point;
}

/*!
  Returns normal vector, oriented in object space.
*/
const SbVec3f &
SoPrimitiveVertex::getNormal(void) const
{
  return this->normal;
}

/*!
  Returns texture coordinates for vertex, specified in object space.
*/
const SbVec4f &
SoPrimitiveVertex::getTextureCoords(void) const
{
  return this->textureCoords;
}

/*!
  Returns index of the vertex into the currently active material, if
  any.
*/
int
SoPrimitiveVertex::getMaterialIndex(void) const
{
  return this->materialIndex;
}

/*!
  Returns pointer to detail instance with more information about the
  vertex. A detail instance may not be available, and if so \c NULL is
  returned.
*/
const SoDetail *
SoPrimitiveVertex::getDetail(void) const
{
  return this->detail;
}

/*!
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.
*/
void
SoPrimitiveVertex::setPoint(const SbVec3f & point)
{
  this->point = point;
}

/*!
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.
*/
void
SoPrimitiveVertex::setNormal(const SbVec3f & normal)
{
  this->normal = normal;
}

/*!
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.
*/
void
SoPrimitiveVertex::setTextureCoords(const SbVec4f & texcoords)
{
  this->textureCoords = texcoords;
}

/*!
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.

  Covenience function. Will fill in 0 and 1 in the last two texture
  coords in the internal SbVec4f texture coordinate instance.
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
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.
*/
void
SoPrimitiveVertex::setMaterialIndex(const int index)
{
  this->materialIndex = index;
}

/*!
  Used internally from library client code setting up an
  SoPrimitiveVertex instance.

  Note that it's the client's responsibility to do the deallocation of
  the detail instance after the SoPrimitiveVertex instance has gone
  out of scope.
*/
void
SoPrimitiveVertex::setDetail(SoDetail * const detail)
{
  this->detail = detail;
}
