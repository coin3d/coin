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
  \class SbBox3s SbBox.h Inventor/SbBox.h
  \brief The SbBox3s class is a 3 dimensional box with short
  integer coordinates.
  \ingroup base

  This box class is used by other classes in Coin for data
  exchange. It provides storage for two box corners with short integer
  coordinates, which is among other things useful for representing
  screen or canvas areas in absolute window coordinates.

  \sa SbBox2f, SbBox3f  
*/


#include <Inventor/SbBox3s.h>
#include <assert.h>
#include <limits.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor makes an empty box.
 */
SbBox3s::SbBox3s(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corner coordinates.

  \a xmin should be less than \a xmax, \a ymin should be less than \a
  ymax, and \a zmin should be less than \a zmax if you want to make a
  valid box.
 
*/
SbBox3s::SbBox3s(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax)
{
  this->setBounds(xmin, ymin, zmin, xmax, ymax, zmax);
}

/*!
  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbBox3s::SbBox3s(const SbVec3s & minvec, const SbVec3s & maxvec)
{
  this->setBounds(minvec, maxvec);
}

/*!
  Default destructor does nothing.
 */
SbBox3s::~SbBox3s(void)
{
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
 */
void
SbBox3s::makeEmpty(void)
{
  this->minpt.setValue(SHRT_MAX, SHRT_MAX, SHRT_MAX);
  this->maxpt.setValue(SHRT_MIN, SHRT_MIN, SHRT_MIN);
}

/*!
  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/
const SbVec3s&
SbBox3s::getMin(void) const
{
  return this->minpt;
}

/*!
  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/
const SbVec3s&
SbBox3s::getMax(void) const
{
  return this->maxpt;
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already within it.
 */
void
SbBox3s::extendBy(const SbVec3s & point)
{
  // The explicit casts are done to humour the HPUX aCC compiler,
  // which will otherwise say ``Template deduction failed to find a
  // match for the call to 'SbMin'''. mortene.
  this->minpt.setValue(SbMin((short)point[0], (short)this->minpt[0]),
                       SbMin((short)point[1], (short)this->minpt[1]),
                       SbMin((short)point[2], (short)this->minpt[2]));
  this->maxpt.setValue(SbMax((short)point[0], (short)this->maxpt[0]),
                       SbMax((short)point[1], (short)this->maxpt[1]),
                       SbMax((short)point[2], (short)this->maxpt[2]));
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
 */
void
SbBox3s::extendBy(const SbBox3s & box)
{
#if COIN_DEBUG
  if (box.minpt[0] > box.maxpt[0] ||
      box.minpt[1] > box.maxpt[1] ||
      box.minpt[2] > box.maxpt[2] )
    SoDebugError::postWarning("SbBox3s::extendBy",
                              "Extending box has negative area.");
#endif // COIN_DEBUG

  this->extendBy(box.getMin());
  this->extendBy(box.getMax());
}

/*!
  Check if the given point lies within the boundaries of this box.
 */
SbBool
SbBox3s::intersect(const SbVec3s & point) const
{
  if((point[0] >= this->minpt[0]) && (point[0] <= this->maxpt[0]) &&
     (point[1] >= this->minpt[1]) && (point[1] <= this->maxpt[1]) &&
     (point[2] >= this->minpt[2]) && (point[2] <= this->maxpt[2])) return TRUE;
  return FALSE;
}

/*!
  Check if \a box lies wholly or partly within the boundaries
  of this box.
 */
SbBool
SbBox3s::intersect(const SbBox3s & box) const
{
  if((box.getMax()[0] < this->getMin()[0]) ||
     (box.getMax()[1] < this->getMin()[1]) ||
     (box.getMax()[2] < this->getMin()[2]) ||
     (box.getMin()[0] > this->getMax()[0]) ||
     (box.getMin()[1] > this->getMax()[1]) ||
     (box.getMin()[2] > this->getMax()[2])) return FALSE;
  return TRUE;
}

/*!
  Reset the boundaries of the box.

  \a xmin should be less than \a xmax, \a ymin should be less than \a
  ymax, and \a zmin should be less than \a xmax if you want to make a
  valid box.

  \sa getBounds().  
*/
void
SbBox3s::setBounds(short xmin, short ymin, short zmin, short xmax, short ymax, short zmax)
{
#if COIN_DEBUG
  if(!(xmin<=xmax && ymin<=ymax && zmin<=zmax))
    SoDebugError::postWarning("SbBox3s::setBounds",
                              "The bounds will give the box negative area.");
#endif // COIN_DEBUG
  this->minpt.setValue(xmin, ymin, zmin);
  this->maxpt.setValue(xmax, ymax, zmax);
}

/*!
  Reset the boundaries of the box with the given corners.

  The coordinates of \a minvec should be less than the coordinates of
  \a maxvec if you want to make a valid box.

  \sa getBounds().
 */
void
SbBox3s::setBounds(const SbVec3s & minvec, const SbVec3s & maxvec)
{
#if COIN_DEBUG
  if(!(minvec[0]<=maxvec[0] && minvec[1]<=maxvec[1] && minvec[2]<=maxvec[2]))
    SoDebugError::postWarning("SbBox3s::setBounds",
                              "The bounds will give the box negative area.");
#endif // COIN_DEBUG
  this->minpt = minvec;
  this->maxpt = maxvec;
}

/*!
  Returns the box boundary coordinates.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox3s::getBounds(short& xmin, short& ymin, short& zmin, short& xmax, short& ymax, short& zmax) const
{
  xmin = this->minpt[0];
  ymin = this->minpt[1];
  zmin = this->minpt[2];
  xmax = this->maxpt[0];
  ymax = this->maxpt[1];
  zmax = this->maxpt[2];
}

/*!
  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox3s::getBounds(SbVec3s & minvec, SbVec3s & maxvec) const
{
  minvec = this->minpt;
  maxvec = this->maxpt;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox3s::getOrigin(short & originX, short & originY, short & originZ) const
{
  originX = this->minpt[0];
  originY = this->minpt[1];
  originZ = this->minpt[2];
}

/*!
  Returns width and height of box.
 */
void
SbBox3s::getSize(short & sizeX, short & sizeY, short & sizeZ) const
{
#if COIN_DEBUG
  if(!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1] && minpt[2]<=maxpt[2]))
    SoDebugError::postWarning("SbBox3s::getSize",
                              "The box has negative area.");
#endif // COIN_DEBUG
  sizeX = this->width();
  sizeY = this->height();
  sizeZ = this->depth();
}


/*!
  \relates SbBox3s

  Check \a b1 and \a b2 for equality.
*/
int
operator ==(const SbBox3s & b1, const SbBox3s & b2)
{
  return b1.getMin()==b2.getMin() && b1.getMax()==b2.getMax();
}

/*!
  \relates SbBox3s

  Check \a b1 and \a b2 for inequality.
*/
int
operator !=(const SbBox3s & b1, const SbBox3s & b2)
{
  return !(b1==b2);
}

/*!
  \COININTERNAL
*/
short
SbBox3s::width(void) const
{
  return this->maxpt[0] - this->minpt[0];
}

/*!
  \COININTERNAL
*/
short
SbBox3s::height(void) const
{
  return this->maxpt[1] - this->minpt[1];
}

/*!
  \COININTERNAL
*/
short
SbBox3s::depth(void) const
{
  return this->maxpt[2] - this->minpt[2];
}


/*!
  \COININTERNAL
*/
SbBool
SbBox3s::hasArea(void) const
{
  return (this->minpt[0] < this->maxpt[0] && 
          this->minpt[1] < this->maxpt[1] && 
          this->minpt[2] < this->maxpt[2]);
}
