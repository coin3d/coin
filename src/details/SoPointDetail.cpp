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
  \class SoPointDetail SoPointDetail.h Inventor/details/SoPointDetail.h
  \brief The SoPointDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoPointDetail.h>
#include <Inventor/SbName.h>

//$ BEGIN TEMPLATE DetailSource(SoPointDetail)

SoType SoPointDetail::classTypeId;

SoType
SoPointDetail::getTypeId(void) const
{
  return SoPointDetail::classTypeId;
}

SoType
SoPointDetail::getClassTypeId(void)
{
  return SoPointDetail::classTypeId;
}

void
SoPointDetail::initClass(void)
{
  SoPointDetail::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoPointDetail"));
}
//$ END TEMPLATE DetailSource

SoPointDetail::SoPointDetail()
  : coordIndex(-1),
    matIndex(-1),
    normIndex(-1),
    texCoordIndex(-1)
{
}

SoPointDetail::~SoPointDetail()
{
}

SoDetail *
SoPointDetail::copy() const
{
  SoPointDetail *copy = new SoPointDetail();
  copy->coordIndex = this->coordIndex;
  copy->matIndex = this->matIndex;
  copy->normIndex = this->normIndex;
  copy->texCoordIndex = this->texCoordIndex;
  return copy;
}

int
SoPointDetail::getCoordinateIndex() const
{
  return this->coordIndex;
}

int
SoPointDetail::getMaterialIndex() const
{
  return this->matIndex;
}

int
SoPointDetail::getNormalIndex() const
{
  return this->normIndex;
}

int
SoPointDetail::getTextureCoordIndex()
{
  return this->texCoordIndex;
}

void
SoPointDetail::setCoordinateIndex(const int idx)
{
  this->coordIndex = idx;
}

void
SoPointDetail::setMaterialIndex(const int idx)
{
  this->matIndex = idx;
}

void
SoPointDetail::setNormalIndex(const int idx)
{
  this->normIndex = idx;
}

void
SoPointDetail::setTextureCoordIndex(const int idx)
{
  this->texCoordIndex = idx;
}
