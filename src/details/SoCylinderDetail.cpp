/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoCylinderDetail SoCylinderDetail.h Inventor/details/SoCylinderDetail.h
  \brief The SoCylinderDetail class contains information about the parts of a SoCylinder shape.
  \ingroup details

  Instances of this class are used for storing information about hit
  points on cylinder geometry after pick operations, and for storing
  information returned to tessellation callbacks.

  \sa SoCylinder, SoRayPickAction, SoCallbackAction
*/

#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/SbName.h>

/*!
  \var int SoCylinderDetail::part
  \COININTERNAL
*/


SO_DETAIL_SOURCE(SoCylinderDetail);

/*!
  Constructor.
 */
SoCylinderDetail::SoCylinderDetail(void)
  : part(0)
{
}

/*!
  Destructor.
 */
SoCylinderDetail::~SoCylinderDetail()
{
}

// doc in super
void
SoCylinderDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoCylinderDetail, SoDetail);
}

// doc in super
SoDetail *
SoCylinderDetail::copy(void) const
{
  SoCylinderDetail * copy = new SoCylinderDetail();
  copy->part = this->part;
  return copy;
}

/*!
  Set the part of a cylinder which was selected. A cylinder has
  three different conceptual parts; SoCylinder::SIDES, SoCylinder::TOP
  or SoCylinder::BOTTOM.
  */
void
SoCylinderDetail::setPart(const int part)
{
  this->part = part;
}

/*!
  Returns selected cylinder part.
 */
int
SoCylinderDetail::getPart(void) const
{
  return this->part;
}
