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
  \class SoCubeDetail SoCubeDetail.h Inventor/details/SoCubeDetail.h
  \brief The SoCubeDetail class is yet to be documented.
  \ingroup details

  FIXME: write doc.
*/

#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoCubeDetail);


SoCubeDetail::SoCubeDetail(void)
  : part(0)
{
}

SoCubeDetail::~SoCubeDetail()
{
}


void
SoCubeDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoCubeDetail, SoDetail);
}

SoDetail *
SoCubeDetail::copy(void) const
{
  SoCubeDetail * copy = new SoCubeDetail;
  copy->part = this->part;
  return copy;
}

void
SoCubeDetail::setPart(const int part)
{
  this->part = part;
}

int
SoCubeDetail::getPart(void) const
{
  return this->part;
}
