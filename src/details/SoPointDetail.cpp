/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoPointDetail SoPointDetail.h Inventor/details/SoPointDetail.h
  \brief The SoPointDetail class is for storing detailed information for a single 3D point.
  \ingroup details

  Instances of this class are used among other things for storing
  information about the vertices of lines and polygons after pick
  operations, and for storing information returned to tessellation
  callbacks.

  It contains indices into the vertex coordinate sets, along with
  indices into material, texture and normal coordinates for the point.
*/

#include <Inventor/details/SoPointDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoPointDetail);

/*!
  Sets up an empty detail instance (all indices are equal to -1).
 */
SoPointDetail::SoPointDetail(void)
  : coordindex(-1), matindex(-1), normindex(-1), texcoordindex(-1)
{
}

/*!
  Destructor.
 */
SoPointDetail::~SoPointDetail()
{
}

// Doc in superclass.
void
SoPointDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoPointDetail, SoDetail);
}

// Doc in superclass.
SoDetail *
SoPointDetail::copy(void) const
{
  SoPointDetail * copy = new SoPointDetail;
  *copy = *this;
  return copy;
}

/*!
  Returns index into coordinate set for the point's 3D coordinates.
 */
int
SoPointDetail::getCoordinateIndex(void) const
{
  return this->coordindex;
}

/*!
  Returns point's index into set of materials.
 */
int
SoPointDetail::getMaterialIndex(void) const
{
  return this->matindex;
}

/*!
  Returns point's index into set of normals.
 */
int
SoPointDetail::getNormalIndex(void) const
{
  return this->normindex;
}

/*!
  Returns point's index into set of texture coordinates.
 */
int
SoPointDetail::getTextureCoordIndex(void)
{
  return this->texcoordindex;
}

/*!
  Used by client code for initializing the point detail instance.
 */
void
SoPointDetail::setCoordinateIndex(const int idx)
{
  this->coordindex = idx;
}

/*!
  Used by client code for initializing the point detail instance.
 */
void
SoPointDetail::setMaterialIndex(const int idx)
{
  this->matindex = idx;
}

/*!
  Used by client code for initializing the point detail instance.
 */
void
SoPointDetail::setNormalIndex(const int idx)
{
  this->normindex = idx;
}

/*!
  Used by client code for initializing the point detail instance.
 */
void
SoPointDetail::setTextureCoordIndex(const int idx)
{
  this->texcoordindex = idx;
}
