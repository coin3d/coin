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
