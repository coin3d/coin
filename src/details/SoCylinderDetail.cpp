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
  \class SoCylinderDetail SoCylinderDetail.h Inventor/details/SoCylinderDetail.h
  \brief The SoCylinderDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoCylinderDetail);


SoCylinderDetail::SoCylinderDetail(void)
  : part(0)
{
}

SoCylinderDetail::~SoCylinderDetail()
{
}


void
SoCylinderDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoCylinderDetail, SoDetail);
}

SoDetail *
SoCylinderDetail::copy(void) const
{
  SoCylinderDetail * copy = new SoCylinderDetail();
  copy->part = this->part;
  return copy;
}

void
SoCylinderDetail::setPart(const int part)
{
  this->part = part;
}

int
SoCylinderDetail::getPart(void) const
{
  return this->part;
}
