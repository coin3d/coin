/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoFaceDetail SoFaceDetail.h Inventor/details/SoFaceDetail.h
  \brief The SoFaceDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/SbName.h>
#include <stddef.h>

SO_DETAIL_SOURCE(SoFaceDetail);


SoFaceDetail::SoFaceDetail(void)
  : pointsarray(NULL),
    numallocated(0),
    numpoints(0),
    faceindex(0),
    partindex(0)
{
}

SoFaceDetail::~SoFaceDetail()
{
  delete [] this->pointsarray;
}

void
SoFaceDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoFaceDetail, SoDetail);
}

SoDetail *
SoFaceDetail::copy(void) const
{
  SoFaceDetail *copy = new SoFaceDetail();
  if (this->numpoints) {
    copy->setNumPoints(this->numpoints);
    for (int i = 0; i < this->numpoints; i++) {
      copy->setPoint(i, this->getPoint(i));
    }
  }
  copy->faceindex = this->faceindex;
  copy->partindex = this->partindex;
  return copy;
}

int
SoFaceDetail::getNumPoints(void) const
{
  return this->numpoints;
}

const SoPointDetail *
SoFaceDetail::getPoint(const int idx) const
{
  assert(idx >= 0 && idx < this->numpoints);
  return &this->pointsarray[idx];
}

SoPointDetail *
SoFaceDetail::getPoints(void)
{
  return this->pointsarray;
}

int
SoFaceDetail::getFaceIndex(void) const
{
  return this->faceindex;
}

int
SoFaceDetail::getPartIndex(void) const
{
  return this->partindex;
}

void
SoFaceDetail::setNumPoints(const int num)
{
  if (num > this->numallocated) {
    this->numallocated = num;
    delete [] this->pointsarray;
    this->pointsarray = new SoPointDetail[this->numallocated];
  }
  this->numpoints = num;
}

void
SoFaceDetail::setPoint(const int idx, const SoPointDetail * const detail)
{
  assert(idx >= 0 && idx <= this->numpoints);
  this->pointsarray[idx] = *detail;
}

void
SoFaceDetail::setFaceIndex(const int idx)
{
  this->faceindex = idx;
}

void
SoFaceDetail::setPartIndex(const int idx)
{
  this->partindex = idx;
}

void
SoFaceDetail::incFaceIndex(void)
{
  this->faceindex++;
}

void
SoFaceDetail::incPartIndex(void)
{
  this->partindex++;
}
