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
  \class SoFaceDetail SoFaceDetail.h Inventor/details/SoFaceDetail.h
  \brief The SoFaceDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/SbName.h>
#include <assert.h>
#include <stddef.h>

//$ BEGIN TEMPLATE DetailSource(SoFaceDetail)

SoType SoFaceDetail::classTypeId;

SoType 
SoFaceDetail::getTypeId(void) const
{
  return SoFaceDetail::classTypeId;
}

SoType 
SoFaceDetail::getClassTypeId(void)
{
  return SoFaceDetail::classTypeId;
}

void 
SoFaceDetail::initClass(void)
{
  SoFaceDetail::classTypeId = 
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoFaceDetail"));
}
//$ END TEMPLATE DetailSource

SoFaceDetail::SoFaceDetail()
  : pointsArray(NULL),
    numPoints(0),
    faceIndex(-1),
    partIndex(-1)
{
}

SoFaceDetail::~SoFaceDetail()
{
  delete [] this->pointsArray;
}

SoDetail *
SoFaceDetail::copy() const
{
  SoFaceDetail *copy = new SoFaceDetail();
  if (this->numPoints) {
    copy->setNumPoints(this->numPoints);
    for (int i = 0; i < this->numPoints; i++) {
      copy->setPoint(i, this->getPoint(i));
    }
  }
  copy->faceIndex = this->faceIndex;
  copy->partIndex = this->partIndex;
  return copy;
}

int 
SoFaceDetail::getNumPoints() const
{
  return this->numPoints;
}

const SoPointDetail *
SoFaceDetail::getPoint(const int idx) const
{
  assert(idx >= 0 && idx < this->numPoints);
  return &this->pointsArray[idx];
}

SoPointDetail *
SoFaceDetail::getPoints()
{
  return this->pointsArray;
}

int 
SoFaceDetail::getFaceIndex() const
{
  return this->faceIndex;
}

int 
SoFaceDetail::getPartIndex() const
{
  return this->partIndex;
}
  
void 
SoFaceDetail::setNumPoints(const int num)
{
  this->numPoints = num;
  delete [] this->pointsArray;
  if (this->numPoints)
    this->pointsArray = new SoPointDetail[num];
  else
    this->pointsArray = NULL;
}

void 
SoFaceDetail::setPoint(const int idx, const SoPointDetail * const detail)
{
  assert(idx >= 0 && idx <= this->numPoints);
  this->pointsArray[idx] = *detail;
}

void 
SoFaceDetail::setFaceIndex(const int idx)
{
  this->faceIndex = idx;
}

void 
SoFaceDetail::setPartIndex(const int idx)
{
  this->partIndex = idx;
}
