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
  \class SoLineDetail SoLineDetail.h Inventor/details/SoLineDetail.h
  \brief The SoLineDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SbName.h>

//$ BEGIN TEMPLATE DetailSource(SoLineDetail)

SoType SoLineDetail::classTypeId;

SoType
SoLineDetail::getTypeId(void) const
{
  return SoLineDetail::classTypeId;
}

SoType
SoLineDetail::getClassTypeId(void)
{
  return SoLineDetail::classTypeId;
}

void
SoLineDetail::initClass(void)
{
  SoLineDetail::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoLineDetail"));
}
//$ END TEMPLATE DetailSource

SoLineDetail::SoLineDetail()
  : lineIndex(0),
    partIndex(0)
{
}

SoLineDetail::~SoLineDetail()
{
}

SoDetail *
SoLineDetail::copy() const
{
  SoLineDetail *copy = new SoLineDetail();
  *copy = *this;
  return copy;

}

const SoPointDetail *
SoLineDetail::getPoint0() const
{
  return &this->points[0];
}

const SoPointDetail *
SoLineDetail::getPoint1() const
{
  return &this->points[1];
}

int
SoLineDetail::getLineIndex() const
{
  return this->lineIndex;
}

int
SoLineDetail::getPartIndex() const
{
  return this->partIndex;
}

void
SoLineDetail::setPoint0(const SoPointDetail * const detail)
{
  this->points[0] = *detail;
}

void
SoLineDetail::setPoint1(const SoPointDetail * const detail)
{
  this->points[1] = *detail;
}

void
SoLineDetail::setLineIndex(const int idx)
{
  this->lineIndex = idx;
}

void
SoLineDetail::setPartIndex(const int idx)
{
  this->partIndex = idx;
}

void
SoLineDetail::incLineIndex()
{
  this->lineIndex++;
}

void
SoLineDetail::incPartIndex()
{
  this->partIndex++;
}
