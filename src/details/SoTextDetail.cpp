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
  \class SoTextDetail SoTextDetail.h Inventor/details/SoTextDetail.h
  \brief The SoTextDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoTextDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoTextDetail);

SoTextDetail::SoTextDetail(void)
  : stringindex(-1),
    charindex(-1),
    part(-1)
{
}

SoTextDetail::~SoTextDetail()
{
}

void
SoTextDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoTextDetail, SoDetail);
}

SoDetail *
SoTextDetail::copy(void) const
{
  SoTextDetail *copy = new SoTextDetail;
  *copy = *this;
  return copy;
}

int
SoTextDetail::getStringIndex(void) const
{
  return this->stringindex;
}

int
SoTextDetail::getCharacterIndex(void) const
{
  return this->charindex;
}

int
SoTextDetail::getPart(void) const
{
  return this->part;
}

void
SoTextDetail::setStringIndex(const int idx)
{
  this->stringindex = idx;
}

void
SoTextDetail::setCharacterIndex(const int idx)
{
  this->charindex = idx;
}

void
SoTextDetail::setPart(const int part)
{
  this->part = part;
}
