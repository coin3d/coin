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
  \class SoConeDetail SoConeDetail.h Inventor/details/SoConeDetail.h
  \brief The SoConeDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoConeDetail.h>
#include <Inventor/SbName.h>

//$ BEGIN TEMPLATE DetailSource(SoConeDetail)

SoType SoConeDetail::classTypeId;

SoType
SoConeDetail::getTypeId(void) const
{
  return SoConeDetail::classTypeId;
}

SoType
SoConeDetail::getClassTypeId(void)
{
  return SoConeDetail::classTypeId;
}

void
SoConeDetail::initClass(void)
{
  SoConeDetail::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoConeDetail"));
}
//$ END TEMPLATE DetailSource

SoConeDetail::SoConeDetail()
  : part(0)
{
}

SoConeDetail::~SoConeDetail()
{
}

SoDetail *
SoConeDetail::copy() const
{
  SoConeDetail *copy = new SoConeDetail();
  copy->part = this->part;
  return copy;
}

void
SoConeDetail::setPart(const int part)
{
  this->part = part;
}

int
SoConeDetail::getPart() const
{
  return this->part;
}
