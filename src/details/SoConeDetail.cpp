/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
  \COININTERNAL
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
