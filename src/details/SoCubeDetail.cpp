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
  \class SoCubeDetail SoCubeDetail.h Inventor/details/SoCubeDetail.h
  \brief The SoCubeDetail class contains information about the parts of a SoCube shape.
  \ingroup details

  The part indices are ordered from 0 to 5 as [ front, back, left,
  right, top, bottom ].

 */

/*!
  \var int SoCubeDetail::part
  \internal
*/

#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoCubeDetail);

/*!
  Constructor.
 */
SoCubeDetail::SoCubeDetail(void)
  : part(0)
{
}

/*!
  Destructor.
 */
SoCubeDetail::~SoCubeDetail()
{
}

// doc in super
void
SoCubeDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoCubeDetail, SoDetail);
}

// doc in super
SoDetail *
SoCubeDetail::copy(void) const
{
  SoCubeDetail * copy = new SoCubeDetail;
  copy->part = this->part;
  return copy;
}

/*!
  Set the part of a cube which was selected. A cube has of course six
  different conceptual parts -- it's sides.
  */
void
SoCubeDetail::setPart(const int part)
{
  this->part = part;
}

/*!
  Returns selected cube part.
 */
int
SoCubeDetail::getPart(void) const
{
  return this->part;
}
