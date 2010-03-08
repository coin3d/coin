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
  \class SbBox2s SbBox.h Inventor/SbBox.h
  \brief The SbBox2s class is a 2 dimensional box with short
  integer coordinates.
  \ingroup base

  This box class is used by other classes in Coin for data
  exchange. It provides storage for two box corners with short integer
  coordinates, which is among other things useful for representing
  screen or canvas areas in absolute window coordinates.

  \sa SbBox2f, SbBox2d, SbBox3s, SbBox3f, SbBox3d, SbXfBox3f.
*/

// *************************************************************************

#include <Inventor/SbBox2i32.h>

#include <limits>
#include <cassert>

#include <Inventor/SbBox2s.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbBox2d.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  \fn SbBox2i32::SbBox2i32(void)

  The default constructor makes an empty box.
*/

/*!
  \fn SbBox2i32::SbBox2i32(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)

  Constructs a box with the given corner coordinates.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.
*/

/*!
  \fn SbBox2i32::SbBox2i32(const SbVec2i32 & minpt, const SbVec2i32 & maxpt)

  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
*/

/*!
  \fn SbBox2i32 &  SbBox2i32::setBounds(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)

  Reset the boundaries of the box.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.

  Returns reference to self.

  \sa getBounds().
*/

/*!
  \fn SbBox2i32 & SbBox2i32::setBounds(const SbVec2i32 & boxmin, const SbVec2i32 & boxmax)

  Reset the boundaries of the box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.

  Returns reference to self.

  \sa getBounds().
*/

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds()
*/

SbBox2i32 &
SbBox2i32::setBounds(const SbBox2s & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds()
*/

SbBox2i32 &
SbBox2i32::setBounds(const SbBox2f & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Reset the boundaries to the boundaries of the given \a box.

  Returns reference to self.

  \sa getBounds()
*/

SbBox2i32 &
SbBox2i32::setBounds(const SbBox2d & box)
{
  if (box.isEmpty()) {
    makeEmpty();
  } else {
    minpt.setValue(box.getMin());
    maxpt.setValue(box.getMax());
  }
  return *this;
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
*/
void
SbBox2i32::makeEmpty(void)
{
  minpt.setValue(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());
  maxpt.setValue(-std::numeric_limits<int32_t>::max(), -std::numeric_limits<int32_t>::max());
}

/*!
  \fn const SbVec2i32 & SbBox2i32::getMin(void) const

  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/

/*!
  \fn const SbVec2i32 & SbBox2i32::getMax(void) const

  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already within it.
*/
void
SbBox2i32::extendBy(const SbVec2i32 & point)
{
  this->minpt.setValue(SbMin(point[0], this->minpt[0]),
                       SbMin(point[1], this->minpt[1]));
  this->maxpt.setValue(SbMax(point[0], this->maxpt[0]),
                       SbMax(point[1], this->maxpt[1]));
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
*/
void
SbBox2i32::extendBy(const SbBox2i32 & box)
{
  if (box.isEmpty()) { return; }

  this->extendBy(box.getMin());
  this->extendBy(box.getMax());
}

/*!
  Check if the given point lies within the boundaries of this box.
*/
SbBool
SbBox2i32::intersect(const SbVec2i32 & point) const
{
  if((point[0] >= this->minpt[0]) && (point[0] <= this->maxpt[0]) &&
     (point[1] >= this->minpt[1]) && (point[1] <= this->maxpt[1])) return TRUE;
  return FALSE;
}

/*!
  Check if \a box lies wholly or partly within the boundaries
  of this box.
*/
SbBool
SbBox2i32::intersect(const SbBox2i32 & box) const
{
  if((box.getMax()[0] < this->getMin()[0]) ||
     (box.getMax()[1] < this->getMin()[1]) ||
     (box.getMin()[0] > this->getMax()[0]) ||
     (box.getMin()[1] > this->getMax()[1])) return FALSE;
  return TRUE;
}

/*!
  \fn void SbBox2i32::getBounds(int32_t & xmin, int32_t & ymin, int32_t & xmax, int32_t & ymax) const

  Returns the box boundary coordinates.

  \sa setBounds(), getMin(), getMax().
*/

/*!
  \fn void SbBox2i32::getBounds(SbVec2i32 & boxmin, SbVec2i32 & boxmax) const

  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/

/*!
  \fn void SbBox2i32::getOrigin(int32_t & originX, int32_t & originY) const

  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/

/*!
  \fn void SbBox2i32::getSize(int32_t & sizeX, int32_t & sizeY) const

  Returns width and height of box.
*/

/*!
  \fn float SbBox2i32::getAspectRatio(void) const

  Returns aspect ratio of box, which is defined as box width divided on
  box height.
*/

/*!
  \fn SbBool SbBox2i32::hasArea(void) const
*/

/*!
  \fn int operator == (const SbBox2i32 & b1, const SbBox2i32 & b2)
  \relates SbBox2i32

  Check \a b1 and \a b2 for equality.
*/

/*!
  \fn int operator != (const SbBox2i32 & b1, const SbBox2i32 & b2)
  \relates SbBox2i32

  Check \a b1 and \a b2 for inequality.
*/

#ifdef COIN_TEST_SUITE
BOOST_AUTO_TEST_CASE(checkSize) {
  SbVec2i32 min(1,2);
  SbVec2i32 max(3,4);

  SbVec2i32 diff = max - min;

  
  SbBox2i32 box(min, max);

  BOOST_CHECK_MESSAGE(box.getSize() == diff,
                      "Box has incorrect size");

}
#endif //COIN_TEST_SUITE
