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
