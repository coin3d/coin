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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
