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
  \class SoConeDetail SoConeDetail.h Inventor/details/SoConeDetail.h
  \brief The SoConeDetail class contains information about the parts of a SoCone shape.
  \ingroup details
*/

/*!
  \var int SoConeDetail::part
  \internal
*/

#include <Inventor/details/SoConeDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoConeDetail);

/*!
  Constructor.
  */
SoConeDetail::SoConeDetail(void)
  : part(0)
{
}

/*!
  Destructor.
 */
SoConeDetail::~SoConeDetail()
{
}

// doc in super
void
SoConeDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoConeDetail, SoDetail);
}

// doc in super
SoDetail *
SoConeDetail::copy(void) const
{
  SoConeDetail * copy = new SoConeDetail;
  copy->part = this->part;
  return copy;
}

/*!
  Set the part of a cone which was selected. A cone has two
  different conceptual parts; SoCone::SIDES or SoCone::BOTTOM.
 */
void
SoConeDetail::setPart(const int part)
{
  this->part = part;
}

/*!
  Returns selected cone part, either SoCone::SIDES or SoCone::BOTTOM.
 */
int
SoConeDetail::getPart(void) const
{
  return this->part;
}
