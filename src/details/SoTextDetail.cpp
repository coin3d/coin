/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
