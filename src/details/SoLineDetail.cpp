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
  \class SoLineDetail SoLineDetail.h Inventor/details/SoLineDetail.h
  \brief The SoLineDetail class is for storing detailed 3D line information.
  \ingroup details

  Instances of this class are used among other things for storing
  information about lines after pick operations, and for storing
  information returned to tessellation callbacks.

  \sa SoRayPickAction, SoPickedPoint, SoCallbackAction
*/

#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoLineDetail);


/*!
  Default constructor sets up an empty, non-valid detail
  specification.
*/
SoLineDetail::SoLineDetail(void)
  : lineindex(0), partindex(0)
{
}

/*!
  Destructor.
 */
SoLineDetail::~SoLineDetail()
{
}

// Doc in superclass.
void
SoLineDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoLineDetail, SoDetail);
}

// Doc in superclass.
SoDetail *
SoLineDetail::copy(void) const
{
  SoLineDetail *copy = new SoLineDetail();
  *copy = *this;
  return copy;

}

/*!
  Returns SoPointDetail describing the line start point.
 */
const SoPointDetail *
SoLineDetail::getPoint0(void) const
{
  return &this->points[0];
}

/*!
  Returns SoPointDetail describing the line end point.
 */
const SoPointDetail *
SoLineDetail::getPoint1(void) const
{
  return &this->points[1];
}

/*!
  Returns the index of this line within the lineset node it is part
  of.
 */
int
SoLineDetail::getLineIndex(void) const
{
  return this->lineindex;
}

/*!
  Returns the index of this line within the complex shape node it is
  part of.
 */
int
SoLineDetail::getPartIndex(void) const
{
  return this->partindex;
}

/*!
  Stores information about line's start point. Used internally from
  library client code setting up a SoLineDetail instance.

  \sa getPoint0()
 */
void
SoLineDetail::setPoint0(const SoPointDetail * const detail)
{
  this->points[0] = *detail;
}

/*!
  Stores information about line's end point. Used internally from
  library client code setting up a SoLineDetail instance.

  \sa getPoint1()
 */
void
SoLineDetail::setPoint1(const SoPointDetail * const detail)
{
  this->points[1] = *detail;
}

/*!
  Used internally from library client code setting up a SoLineDetail
  instance.

  \sa getLineIndex()
 */
void
SoLineDetail::setLineIndex(const int idx)
{
  this->lineindex = idx;
}

/*!
  Used internally from library client code setting up a SoLineDetail
  instance.

  \sa getPartIndex()
 */
void
SoLineDetail::setPartIndex(const int idx)
{
  this->partindex = idx;
}

/*!
  Convenience method for library client code when setting up a
  SoLineDetail instance to use the line index as a counter.
*/
void
SoLineDetail::incLineIndex(void)
{
  this->lineindex++;
}

/*!
  Convenience method for library client code when setting up a
  SoLineDetail instance to use the part index as a counter.
*/
void
SoLineDetail::incPartIndex(void)
{
  this->partindex++;
}
