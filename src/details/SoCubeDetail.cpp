/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoCubeDetail SoCubeDetail.h Inventor/details/SoCubeDetail.h
  \brief The SoCubeDetail class contains information about the parts of a SoCube shape.
  \ingroup details

  Instances of this class are used for storing information about hit
  points on cone geometry after pick operations, and for storing
  information returned to tessellation callbacks.

  The part indices are ordered from 0 to 5 as [ front, back, left,
  right, top, bottom ].

  \sa SoCube, SoRayPickAction, SoCallbackAction
*/

/*!
  \var int SoCubeDetail::part
  \COININTERNAL
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
SoCubeDetail::setPart(const int partarg)
{
  this->part = partarg;
}

/*!
  Returns selected cube part.
 */
int
SoCubeDetail::getPart(void) const
{
  return this->part;
}
