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
  \class SoLineDetail SoLineDetail.h Inventor/details/SoLineDetail.h
  \brief The SoLineDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoLineDetail);


SoLineDetail::SoLineDetail(void)
  : lineindex(0),
    partindex(0)
{
}

SoLineDetail::~SoLineDetail()
{
}


void
SoLineDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoLineDetail, SoDetail);
}

SoDetail *
SoLineDetail::copy(void) const
{
  SoLineDetail *copy = new SoLineDetail();
  *copy = *this;
  return copy;

}

const SoPointDetail *
SoLineDetail::getPoint0(void) const
{
  return &this->points[0];
}

const SoPointDetail *
SoLineDetail::getPoint1(void) const
{
  return &this->points[1];
}

int
SoLineDetail::getLineIndex(void) const
{
  return this->lineindex;
}

int
SoLineDetail::getPartIndex(void) const
{
  return this->partindex;
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
  this->lineindex = idx;
}

void
SoLineDetail::setPartIndex(const int idx)
{
  this->partindex = idx;
}

void
SoLineDetail::incLineIndex(void)
{
  this->lineindex++;
}

void
SoLineDetail::incPartIndex(void)
{
  this->partindex++;
}
