/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

SO_DETAIL_SOURCE(SoPointDetail);


SoPointDetail::SoPointDetail(void)
  : coordindex(-1),
    matindex(-1),
    normindex(-1),
    texcoordindex(-1)
{
}

SoPointDetail::~SoPointDetail()
{
}

void
SoPointDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoPointDetail, SoDetail);
}

SoDetail *
SoPointDetail::copy(void) const
{
  SoPointDetail * copy = new SoPointDetail;
  *copy = *this;
  return copy;
}

int
SoPointDetail::getCoordinateIndex(void) const
{
  return this->coordindex;
}

int
SoPointDetail::getMaterialIndex(void) const
{
  return this->matindex;
}

int
SoPointDetail::getNormalIndex(void) const
{
  return this->normindex;
}

int
SoPointDetail::getTextureCoordIndex(void)
{
  return this->texcoordindex;
}

void
SoPointDetail::setCoordinateIndex(const int idx)
{
  this->coordindex = idx;
}

void
SoPointDetail::setMaterialIndex(const int idx)
{
  this->matindex = idx;
}

void
SoPointDetail::setNormalIndex(const int idx)
{
  this->normindex = idx;
}

void
SoPointDetail::setTextureCoordIndex(const int idx)
{
  this->texcoordindex = idx;
}
