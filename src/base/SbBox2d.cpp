/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
  \class SbBox2d Inventor/SbBox2d.h
  \brief The SbBox2d class is a 2 dimensional box with double precision
  corner coordinates.
  \ingroup base

  This box class is used by many other classes in Coin for data
  exchange and storage. It provides two box corners with double
  precision coordinates, which is among other things useful for
  representing screen or canvas dimensions in normalized coordinates.

  This class is a Coin extension.

  \since 2002-03-09

  \sa SbBox2f
*/


#include <Inventor/SbBox2d.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <float.h>  // DBL_MAX

/*!
  The default constructor makes an empty box.
 */
SbBox2d::SbBox2d(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corners.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.
 */
SbBox2d::SbBox2d(double xmin, double ymin, double xmax, double ymax)
{
  this->setBounds(xmin, ymin, xmax, ymax);
}

/*!
  Constructs a box with the given lower left and upper right corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbBox2d::SbBox2d(const SbVec2d & min, const SbVec2d & max)
{
  this->setBounds(min, max);
}

/*!
  Destructor.
 */
SbBox2d::~SbBox2d(void)
{
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
 */
void
SbBox2d::makeEmpty(void)
{
  this->minpt.setValue(DBL_MAX, DBL_MAX);
  this->maxpt.setValue(-DBL_MAX, -DBL_MAX);
}

/*!
  Check if this has been marked as an empty box.

  \sa makeEmpty().
 */
SbBool
SbBox2d::isEmpty(void) const
{
  if (this->minpt[0] > this->maxpt[0]) return TRUE;
  else return FALSE;
}

/*!
  Check if the box has "positive" area, i.e. the lower left corner is
  actually lower and more to the left than the other corner point.
 */
SbBool
SbBox2d::hasArea(void) const
{
  return (this->minpt[0] < this->maxpt[0] && this->minpt[1] < this->maxpt[1]);
}

/*!
  Returns the lower left corner of the box.

  \sa getOrigin(), getMax().
*/
const SbVec2d&
SbBox2d::getMin(void) const
{
  return this->minpt;
}

/*!
  Returns the upper right corner of the box.

  \sa getMin().
*/
const SbVec2d&
SbBox2d::getMax(void) const
{
  return this->maxpt;
}

/*!
  Returns the center point of the box.
 */
SbVec2d
SbBox2d::getCenter(void) const
{
  return SbVec2d((this->maxpt[0] + this->minpt[0])/2.0f,
                 (this->maxpt[1] + this->minpt[1])/2.0f);
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  box fit around the \a point if it isn't already situated within it.
 */
void
SbBox2d::extendBy(const SbVec2d & point)
{
  // The explicit cast to double is done to humour the HPUX aCC
  // compiler, which will otherwise say ``Template deduction failed to
  // find a match for the call to 'SbMin'''. mortene.
  this->minpt.setValue(SbMin((double)point[0], (double)this->minpt[0]),
                       SbMin((double)point[1], (double)this->minpt[1]));
  this->maxpt.setValue(SbMax((double)point[0], (double)this->maxpt[0]),
                       SbMax((double)point[1], (double)this->maxpt[1]));
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling the above method twice with the corner points.
 */
void
SbBox2d::extendBy(const SbBox2d & box)
{
#if COIN_DEBUG
  if (box.minpt[0] > box.maxpt[0] || box.minpt[1] > box.maxpt[1])
    SoDebugError::postWarning("SbBox2d::extendBy",
                              "Extension box has negative area.");
#endif // COIN_DEBUG
  this->extendBy(box.getMin());
  this->extendBy(box.getMax());
}

/*!
  Check if \a point lies within the boundaries of this box.
 */
SbBool
SbBox2d::intersect(const SbVec2d & point) const
{
  if ((point[0] >= this->minpt[0]) && (point[0] <= this->maxpt[0]) &&
     (point[1] >= this->minpt[1]) && (point[1] <= this->maxpt[1])) return TRUE;
  return FALSE;
}

/*!
  Check if \a box lies wholly or partly within the boundaries
  of this box.
 */
SbBool
SbBox2d::intersect(const SbBox2d & box) const
{
  if ((box.getMax()[0] < this->getMin()[0]) ||
     (box.getMax()[1] < this->getMin()[1]) ||
     (box.getMin()[0] > this->getMax()[0]) ||
     (box.getMin()[1] > this->getMax()[1])) return FALSE;
  return TRUE;
}

/*!
  Check if a a line from \a a to \a b intersects the box, and return the
  coordinates of the union line in \a ia and \a ib.

  This function is a Coin extension.

  \since 2002-03-09 larsa
*/
SbBool
SbBox2d::findIntersection(const SbVec2d & a, const SbVec2d & b, SbVec2d & ia, SbVec2d & ib) const
{
  // FIXME: this function should be tested thoroughly...

  // we place point a and b in their respective blocks, and handle cases accordingly
  //
  //   block-ids     intersection
  //   6 | 7 | 8      candidates
  //  ---+---+---       +-8-+
  //   3 | 4 | 5        2   4
  //  ---+---+---       +-1-+
  //   0 | 1 | 2
  static int candidates[9] = { 1|2, 1, 1|4, 2, 0, 4, 2|8, 8, 4|8 };

  int blocka = 0, blockb = 0;
  if ( a[0] < this->minpt[0] ) blocka += 0;
  else if ( a[0] <= this->maxpt[0] ) blocka += 1;
  else blocka += 2;
  if ( a[1] < this->minpt[1] ) blocka = +0;
  else if ( a[1] <= this->maxpt[1] ) blocka += 3;
  else blocka = +6;
  if ( b[0] < this->minpt[0] ) blockb += 0;
  else if ( b[0] <= this->maxpt[0] ) blockb += 1;
  else blockb += 2;
  if ( b[1] < this->minpt[1] ) blockb += 0;
  else if ( b[1] <= this->maxpt[1] ) blockb += 3;
  else blockb += 6;
  int enterwalls = candidates[blocka];
  int leavewalls = candidates[blockb];
  // both a and b can be outside box in the same way
  if ( (enterwalls & leavewalls) != 0 ) return FALSE;

  SbBool foundia = FALSE;
  if ( blocka == 4 ) {
    ia = a;
    foundia = TRUE;
  }
  if ( !foundia && (enterwalls & 1) ) {
    do {
      if ( blockb == 0 || blockb == 1 || blockb == 2 ) break;
      SbVec2d vec = b - a;
      double t = (this->minpt[1] - a[1]) / vec[1];
      if ( t < 0.0 || t > 1.0 ) break;
      ia = a + vec * t;
      if ( ia[0] < this->minpt[0] || ia[0] > this->maxpt[0] ) break;
      foundia = TRUE;
    } while ( FALSE );
  }
  if ( !foundia && (enterwalls & 2) ) {
    do {
      if ( blockb == 0 || blockb == 3 || blockb == 6 ) break;
      SbVec2d vec = b - a;
      double t = (this->minpt[0] - a[0]) / vec[0];
      if ( t < 0.0 || t > 1.0 ) break;
      ia = a + vec * t;
      if ( ia[1] < this->minpt[1] || ia[1] > this->maxpt[1] ) break;
      foundia = TRUE;
    } while ( FALSE );
  }
  if ( !foundia && (enterwalls & 4) ) {
    do {
      if ( blockb == 2 || blockb == 5 || blockb == 8 ) break;
      SbVec2d vec = b - a;
      double t = (this->maxpt[0] - a[0]) / vec[0];
      if ( t < 0.0 || t > 1.0 ) break;
      ia = a + vec * t;
      if ( ia[1] < this->minpt[1] || ia[1] > this->maxpt[1] ) break;
      foundia = TRUE;
    } while ( FALSE );
  }
  if ( !foundia && (enterwalls & 8) ) {
    do {
      if ( blockb == 6 || blockb == 7 || blockb == 8 ) break;
      SbVec2d vec = b - a;
      double t = (this->maxpt[1] - a[1]) / vec[1];
      if ( t < 0.0 || t > 1.0 ) break;
      ia = a + vec * t;
      if ( ia[0] < this->minpt[0] || ia[0] > this->maxpt[0] ) break;
      foundia = TRUE;
    } while ( FALSE );
  }
  if ( !foundia ) return FALSE;

  SbBool foundib = FALSE;
  if ( blockb == 4 ) {
    ib = b;
    foundib = TRUE;
  }
  if ( !foundib && (leavewalls & 1) ) {
    do {
      if ( blocka == 0 || blocka == 1 || blocka == 2 ) break;
      SbVec2d vec = a - b;
      double t = (this->minpt[1] - b[1]) / vec[1];
      if ( t < 0.0 || t > 1.0 ) break;
      ib = b + vec * t;
      if ( ib[0] < this->minpt[0] || ib[0] > this->maxpt[0] ) break;
      foundib = TRUE;
    } while ( FALSE );
  }
  if ( !foundib && (leavewalls & 2) ) {
    do {
      if ( blocka == 0 || blocka == 3 || blocka == 6 ) break;
      SbVec2d vec = a - b;
      double t = (this->minpt[0] - b[0]) / vec[0];
      if ( t < 0.0 || t > 1.0 ) break;
      ib = b + vec * t;
      if ( ib[1] < this->minpt[1] || ib[1] > this->maxpt[1] ) break;
      foundib = TRUE;
    } while ( FALSE );
  }
  if ( !foundib && (leavewalls & 4) ) {
    do {
      if ( blocka == 2 || blocka == 5 || blocka == 8 ) break;
      SbVec2d vec = a - b;
      double t = (this->maxpt[0] - b[0]) / vec[0];
      if ( t < 0.0 || t > 1.0 ) break;
      ib = b + vec * t;
      if ( ib[1] < this->minpt[1] || ib[1] > this->maxpt[1] ) break;
      foundib = TRUE;
    } while ( FALSE );
  }
  if ( !foundib && (leavewalls & 8) ) {
    do {
      if ( blocka == 6 || blocka == 7 || blocka == 8 ) break;
      SbVec2d vec = a - b;
      double t = (this->maxpt[1] - b[1]) / vec[1];
      if ( t < 0.0 || t > 1.0 ) break;
      ib = b + vec * t;
      if ( ib[0] < this->minpt[0] || ib[0] > this->maxpt[0] ) break;
      foundib = TRUE;
    } while ( FALSE );
  }
  if ( !foundib ) return FALSE;

  return TRUE;
} // findIntersection()

/*!
  Return the point on the box closest to the given point \a p.
 */
SbVec2d
SbBox2d::getClosestPoint(const SbVec2d & p) const
{
  SbVec2d closest = p;

  SbVec2d center = this->getCenter();
  double devx = closest[0] - center[0];
  double devy = closest[1] - center[1];
  double halfwidth = this->width() / 2.0f;
  double halfheight = this->height() / 2.0f;

  // Move point to be on the nearest line of the box.
  if (fabs(devx) > fabs(devy))
    closest[0] = center[0] + halfwidth * ((devx < 0.0f) ? -1.0f : 1.0f);
  else
    closest[1] = center[1] + halfheight * ((devy < 0.0f) ? -1.0f : 1.0f);

  // Clamp to be inside box.
  closest[0] = SbMin(SbMax(closest[0], this->minpt[0]), this->maxpt[0]);
  closest[1] = SbMin(SbMax(closest[1], this->minpt[1]), this->maxpt[1]);

  return closest;
}

/*!
  Reset the boundaries of the box.

  \a xmin should be less than \a xmax and \a ymin should be less than
  \a ymax if you want to make a valid box.

  \sa getBounds().
*/
void
SbBox2d::setBounds(double xmin, double ymin, double xmax, double ymax)
{
#if COIN_DEBUG
  if (!(xmin<=xmax && ymin<=ymax))
    SoDebugError::postWarning("SbBox2d::setBounds",
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
SbBox2d::setBounds(const SbVec2d & min, const SbVec2d & max)
{
#if COIN_DEBUG
  if (!(min[0]<=max[0] && min[1]<=max[1]))
    SoDebugError::postWarning("SbBox2d::setBounds",
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
SbBox2d::getBounds(double & xmin, double & ymin, double & xmax, double & ymax) const
{
  this->minpt.getValue(xmin, ymin);
  this->maxpt.getValue(xmax, ymax);
}

/*!
  Returns the box corner points.

  \sa setBounds(), getMin(), getMax().
*/
void
SbBox2d::getBounds(SbVec2d & min, SbVec2d & max) const
{
  min = this->minpt;
  max = this->maxpt;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox2d::getOrigin(double & x0, double & y0) const
{
  x0 = this->minpt[0];
  y0 = this->minpt[1];
}

/*!
  Returns width and height of box.
 */
void
SbBox2d::getSize(double & w, double & h) const
{
#if COIN_DEBUG
  if (!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2d::getSize",
                              "The box has negative area.");
#endif // COIN_DEBUG

  w = this->width();
  h = this->height();
}

/*!
  Returns aspect ratio of box, which is defined as box width divided on
  box height.
 */
double
SbBox2d::getAspectRatio(void) const
{
#if COIN_DEBUG
  if (!(minpt[0]<=maxpt[0] && minpt[1]<=maxpt[1]))
    SoDebugError::postWarning("SbBox2d::getAspectRatio",
                              "The box has negative area.");
  if (this->height()==0.0f)
    SoDebugError::postWarning("SbBox2d::getAspectRatio",
                              "The box has zero width=>infinite aspectRatio.");
#endif // COIN_DEBUG

  return (this->width()/this->height());
}

/*!
  \COININTERNAL
 */
double
SbBox2d::width(void) const
{
  return this->maxpt[0] - this->minpt[0];
}

/*!
  \COININTERNAL
*/
double
SbBox2d::height(void) const
{
  return this->maxpt[1] - this->minpt[1];
}

/*!
  \relates SbBox2d

  Check \a b1 and \a b2 for equality.
*/
int
operator ==(const SbBox2d & b1, const SbBox2d & b2)
{
  return b1.getMin() == b2.getMin() && b1.getMax() == b2.getMax();
}

/*!
  \relates SbBox2d

  Check \a b1 and \a b2 for inequality.
*/
int
operator !=(const SbBox2d & b1, const SbBox2d & b2)
{
  return !(b1 == b2);
}
