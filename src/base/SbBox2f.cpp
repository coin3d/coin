/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SbBox2f SbBox.h Inventor/SbBox.h
  \brief The SbBox2f class is a 2 dimensional box with floating
  point corner coordinates.
  \ingroup base

  This box class is used by many other classes in Coin for data
  exchange and storage. It provides two box corners with floating
  point coordinates, which is among other things useful for
  representing screen or canvas dimensions in normalized coordinates.

  \sa SbBox2s, SbBox3f  */


#include <Inventor/SbBox2f.h>
#include <assert.h>
#include <iostream.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  The default constructor makes an empty box.
 */
SbBox2f::SbBox2f(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corners.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.
 */
SbBox2f::SbBox2f(float xmin, float ymin, float xmax, float ymax)
{
  this->setBounds(xmin, ymin, xmax, ymax);
}

/*!
  Constructs a box with the given lower left and upper right corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbBox2f::SbBox2f(const SbVec2f& min, const SbVec2f& max)
{
  this->setBounds(min, max);
}

/*!
  Destructor.
 */
SbBox2f::~SbBox2f(void)
{
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
 */
void
SbBox2f::makeEmpty(void)
{
  this->minpt.setValue(FLT_MAX, FLT_MAX);
  this->maxpt.setValue(-FLT_MAX, -FLT_MAX);
}

/*!
  Check if this has been marked as an empty box.

  \sa makeEmpty().
 */
SbBool
SbBox2f::isEmpty(void) const
{
  if(this->minpt[0] > this->maxpt[0]) return TRUE;
  else return FALSE;
}

/*!
  Check if the box has "positive" area, i.e. the lower left corner is
  actually lower and more to the left than the other corner point.
 */
SbBool
SbBox2f::hasArea(void) const
{
  return (this->minpt[0] < this->maxpt[0] && this->minpt[1] < this->maxpt[1]);
}

/*!
  Returns the lower left corner of the box.

  \sa getOrigin(), getMax().
*/
const SbVec2f&
SbBox2f::getMin(void) const
{
  return this->minpt;
}

/*!
  Returns the upper right corner of the box.

  \sa getMin().
*/
const SbVec2f&
SbBox2f::getMax(void) const
{
  return this->maxpt;
}

/*!
  Returns the center point of the box.
 */
SbVec2f
SbBox2f::getCenter(void) const
{
  return SbVec2f((this->maxpt[0] + this->minpt[0])/2.0f,
                 (this->maxpt[1] + this->minpt[1])/2.0f);
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  box fit around the \a point if it isn't already situated within it.
 */
void
SbBox2f::extendBy(const SbVec2f& point)
{
  this->minpt.setValue(SbMin(point[0], this->minpt[0]),
                       SbMin(point[1], this->minpt[1]));
  this->maxpt.setValue(SbMax(point[0], this->maxpt[0]),
                       SbMax(point[1], this->maxpt[1]));
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling the above method twice with the corner points.
 */
void
SbBox2f::extendBy(const SbBox2f& box)
{
#if COIN_DEBUG
  if (box.minpt[0] > box.maxpt[0] || box.minpt[1] > box.maxpt[1])
    SoDebugError::postWarning("SbBox2f::extendBy",
                              "Extension box has negative area.");
#endif // COIN_DEBUG
  this->extendBy(box.getMin());
  this->extendBy(box.getMax());
}

/*!
  Check if \a point lies within the boundaries of this box.
 */
SbBool
SbBox2f::intersect(const SbVec2f& point) const
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
SbBox2f::intersect(const SbBox2f& box) const
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
SbBox2f::setBounds(float xmin, float ymin, float xmax, float ymax)
{
#if COIN_DEBUG
  if(!(xmin<=xmax && ymin<=ymax))
    SoDebugError::postWarning("SbBox2f::setBounds",
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
SbBox2f::setBounds(const SbVec2f& min, const SbVec2f& max)
{
#if COIN_DEBUG
  if(!(min[0]<=max[0] && min[1]<=max[1]))
    SoDebugError::postWarning("SbBox2f::setBounds",
                              "The bounds will give the box negative area.");
#endif // COIN_DEBUG

  this->minpt = min;
  this->maxpt = max;
}

/*!
  Returns the box boundaries.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox2f::getBounds(float& xmin, float& ymin, float& xmax, float& ymax) const
{
  this->minpt.getValue(xmin,ymin);
  this->maxpt.getValue(xmax,ymax);
}

/*!
  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox2f::getBounds(SbVec2f& min, SbVec2f& max) const
{
  min = this->minpt;
  max = this->maxpt;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox2f::getOrigin(float& x0, float& y0) const
{
  x0 = this->minpt[0];
  y0 = this->minpt[1];
}

/*!
  Returns width and height of box.
 */
void
SbBox2f::getSize(float& w, float& h) const
{
#if COIN_DEBUG
  if(!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2f::getSize",
                              "The box has negative area.");
#endif // COIN_DEBUG

  w = this->width();
  h = this->height();
}

/*!
  Returns aspect ratio of box, which is defined as box width divided on
  box height.
 */
float
SbBox2f::getAspectRatio(void) const
{
#if COIN_DEBUG
  if(!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2f::getAspectRatio",
                              "The box has negative area.");
  if (this->height()==0.0f)
    SoDebugError::postWarning("SbBox2f::getAspectRatio",
                              "The box has zero width=>infinite aspectRatio.");
#endif // COIN_DEBUG

  return (this->width()/this->height());
}

/*!
  \internal
 */
float
SbBox2f::width(void) const
{
  return this->maxpt[0] - this->minpt[0];
}

/*!
  \internal
*/
float
SbBox2f::height(void) const
{
  return this->maxpt[1] - this->minpt[1];
}
