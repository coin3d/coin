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
  \class SoCylinderDetail SoCylinderDetail.h Inventor/details/SoCylinderDetail.h
  \brief The SoCylinderDetail class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/SbName.h>

//$ BEGIN TEMPLATE DetailSource( SoCylinderDetail )

SoType SoCylinderDetail::classTypeId;


SoType 
SoCylinderDetail::getTypeId() const
{
  return SoCylinderDetail::classTypeId;
}

SoType 
SoCylinderDetail::getClassTypeId()
{
  return SoCylinderDetail::classTypeId;
}

void 
SoCylinderDetail::initClass()
{
  SoCylinderDetail::classTypeId = 
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoCylinderDetail"));
}
//$ END TEMPLATE DetailSource

SoCylinderDetail::SoCylinderDetail()
  : part(0)
{
}

SoCylinderDetail::~SoCylinderDetail()
{
}

SoDetail *
SoCylinderDetail::copy() const
{
  SoCylinderDetail *copy = new SoCylinderDetail();
  copy->part = this->part;
  return copy;
}

void 
SoCylinderDetail::setPart(const int part)
{
  this->part = part;
}

int
SoCylinderDetail::getPart() const
{
  return this->part;
}
