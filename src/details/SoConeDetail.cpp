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
  \class SoConeDetail SoConeDetail.h Inventor/details/SoConeDetail.h
  \brief The SoConeDetail class contains information about the parts of a SoCone shape.
  \ingroup details

  Instances of this class are used for storing information about hit
  points on cone geometry after pick operations, and for storing
  information returned to tessellation callbacks.

  \sa SoCone, SoRayPickAction, SoCallbackAction
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
