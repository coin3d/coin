/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
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

#include <assert.h>
#include <limits.h>
#include <Inventor/SbBox2i32.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor makes an empty box.
 */
SbBox2i32::SbBox2i32(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corner coordinates.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.
 */
SbBox2i32::SbBox2i32(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)
{
  this->setBounds(xmin, ymin, xmax, ymax);
}

/*!
  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbBox2i32::SbBox2i32(const SbVec2i32 & boxmin, const SbVec2i32 & boxmax)
{
  this->setBounds(boxmin, boxmax);
}

/*!
  Default destructor does nothing.
 */
SbBox2i32::~SbBox2i32(void)
{
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
 */
void
SbBox2i32::makeEmpty(void)
{
  this->minpt.setValue(SHRT_MAX, SHRT_MAX);
  this->maxpt.setValue(SHRT_MIN, SHRT_MIN);
}

/*!
  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/
const SbVec2i32&
SbBox2i32::getMin(void) const
{
  return this->minpt;
}

/*!
  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/
const SbVec2i32&
SbBox2i32::getMax(void) const
{
  return this->maxpt;
}

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
#if COIN_DEBUG
  if (box.minpt[0] > box.maxpt[0] || box.minpt[1] > box.maxpt[1])
    SoDebugError::postWarning("SbBox2i32::extendBy",
                              "Extending box has negative area.");
#endif // COIN_DEBUG

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
  Reset the boundaries of the box.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.

  \sa getBounds().
*/
void
SbBox2i32::setBounds(int32_t xmin, int32_t ymin, int32_t xmax, int32_t ymax)
{
#if COIN_DEBUG
  if(!(xmin<=xmax && ymin<=ymax))
    SoDebugError::postWarning("SbBox2i32::setBounds",
                              "The bounds will give the box negative area.");
#endif // COIN_DEBUG
  this->minpt.setValue(xmin, ymin);
  this->maxpt.setValue(xmax, ymax);
}

/*!
  Reset the boundaries of the box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.

  \sa getBounds().
 */
void
SbBox2i32::setBounds(const SbVec2i32 & boxmin, const SbVec2i32 & boxmax)
{
#if COIN_DEBUG
  if(!(boxmin[0]<=boxmax[0] && boxmin[1]<=boxmax[1]))
    SoDebugError::postWarning("SbBox2i32::setBounds",
                              "The bounds will give the box negative area.");
#endif // COIN_DEBUG
  this->minpt = boxmin;
  this->maxpt = boxmax;
}

/*!
  Returns the box boundary coordinates.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox2i32::getBounds(int32_t & xmin, int32_t & ymin, int32_t & xmax, int32_t & ymax) const
{
  xmin = this->minpt[0];
  ymin = this->minpt[1];
  xmax = this->maxpt[0];
  ymax = this->maxpt[1];
}

/*!
  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox2i32::getBounds(SbVec2i32 & boxmin, SbVec2i32 & boxmax) const
{
  boxmin = this->minpt;
  boxmax = this->maxpt;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox2i32::getOrigin(int32_t & originX, int32_t & originY) const
{
  originX = this->minpt[0];
  originY = this->minpt[1];
}

/*!
  Returns width and height of box.
 */
void
SbBox2i32::getSize(int32_t & sizeX, int32_t & sizeY) const
{
#if COIN_DEBUG
  if(!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2i32::getSize",
                              "The box has negative area.");
#endif // COIN_DEBUG
  sizeX = this->width();
  sizeY = this->height();
}

/*!
  Returns aspect ratio of box, which is defined as box width divided on
  box height.
 */
float
SbBox2i32::getAspectRatio(void) const
{
#if COIN_DEBUG
  if(!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2i32::getAspectRatio",
                              "The box has negative area.");
  if (this->height()==0.0f)
    SoDebugError::postWarning("SbBox2i32::getAspectRatio",
                              "The box has zero width=>infinite aspectRatio.");
#endif // COIN_DEBUG
  return ((float)this->width()/(float)this->height());
}

/*!
  \relates SbBox2i32

  Check \a b1 and \a b2 for equality.
*/
int
operator ==(const SbBox2i32 & b1, const SbBox2i32 & b2)
{
  return b1.getMin()==b2.getMin() && b1.getMax()==b2.getMax();
}

/*!
  \relates SbBox2i32

  Check \a b1 and \a b2 for inequality.
*/
int
operator !=(const SbBox2i32 & b1, const SbBox2i32 & b2)
{
  return !(b1==b2);
}

/*!
  \COININTERNAL
*/
int32_t
SbBox2i32::width(void) const
{
  return this->maxpt[0] - this->minpt[0];
}

/*!
  \COININTERNAL
*/
int32_t
SbBox2i32::height(void) const
{
  return this->maxpt[1] - this->minpt[1];
}

/*!
  \COININTERNAL
*/
SbBool
SbBox2i32::hasArea(void) const
{
  return (this->minpt[0] < this->maxpt[0] && this->minpt[1] < this->maxpt[1]);
}
