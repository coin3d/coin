/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
