/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SbBox3d SbBox.h Inventor/SbBox.h
  \brief The SbBox3d class is an abstraction for an axis aligned 3 dimensional box.
  \ingroup base

  This box abstraction class is used by other entities in the Coin
  library for data exchange and storage. It provides a representation
  of the defining corners of a box in 3D space, with the sides aligned
  with the 3 principal axes.

  \sa SbBox2s, SbBox2f, SbBox2d, SbBox3s, SbBox3f, SbXfBox3f.
*/


#include <float.h> // DBL_MAX

#include <Inventor/SbBox3d.h>
#include <Inventor/SbDPMatrix.h>
#include <Inventor/errors/SoDebugError.h>


/*!
  The default constructor makes an empty box.
 */
SbBox3d::SbBox3d(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corners.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.
*/
SbBox3d::SbBox3d(const double minx, const double miny, const double minz,
                 const double maxx, const double maxy, const double maxz)
{
  this->setBounds(minx, miny, minz, maxx, maxy, maxz);
}

/*!
  Constructs a box with the given corners.
  
  The coordinates of \a min should be less than the coordinates of \a
  max if you want to make a valid box.
*/
SbBox3d::SbBox3d(const SbVec3d & minval, const SbVec3d & maxval)
{
  this->min = minval;
  this->max = maxval;
}

/*!
  Default destructor does nothing.
 */
SbBox3d::~SbBox3d(void)
{
}

/*!
  Returns the minimum point. This should usually be the lower left
  corner point of the box.

  \sa getOrigin(), getMax().
*/
const SbVec3d &
SbBox3d::getMin(void) const
{
  return this->min;
}

/*!
  Returns the maximum point. This should usually be the upper right
  corner point of the box.

  \sa getMin().
*/
const SbVec3d &
SbBox3d::getMax(void) const
{
  return this->max;
}

/*!
  Returns a modifiable reference the minimum point.
*/
SbVec3d &
SbBox3d::getMin(void)
{
  return this->min;
}

/*!
  Returns a modifiable reference the maximum point.
*/
SbVec3d &
SbBox3d::getMax(void)
{
  return this->max;
}

/*!
  Returns the center point of the box.
 */
SbVec3d
SbBox3d::getCenter(void) const
{
#if COIN_DEBUG
  if (this->isEmpty())
    SoDebugError::postWarning("SbBox3d::getCenter",
                              "The box is empty.");
#endif // COIN_DEBUG
  return SbVec3d((this->max[0] + this->min[0]) * 0.5,
                 (this->max[1] + this->min[1]) * 0.5,
                 (this->max[2] + this->min[2]) * 0.5);
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.
 */
void
SbBox3d::extendBy(const SbVec3d & point)
{
  if (this->isEmpty()) {
    this->setBounds(point, point);
  }
  else {
    // The explicit casts are done to humour the HPUX aCC compiler,
    // which will otherwise say ``Template deduction failed to find a
    // match for the call to 'SbMin'''. mortene.
    this->min.setValue(SbMin((double)point[0], (double)this->min[0]),
                       SbMin((double)point[1], (double)this->min[1]),
                       SbMin((double)point[2], (double)this->min[2]));
    this->max.setValue(SbMax((double)point[0], (double)this->max[0]),
                       SbMax((double)point[1], (double)this->max[1]),
                       SbMax((double)point[2], (double)this->max[2]));
  }
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
 */
void
SbBox3d::extendBy(const SbBox3d & box)
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SbBox3d::extendBy",
                              "The box is not valid.");
    return;
  }
#endif // COIN_DEBUG

  if (this->isEmpty()) {
    *this = box;
  }
  else {
    this->extendBy(box.min);
    this->extendBy(box.max);
  }
}

/*!
  Check if the given point lies within the boundaries of this box.
 */
SbBool
SbBox3d::intersect(const SbVec3d & point) const
{
  return !(point[0] < this->min[0] ||
           point[0] > this->max[0] ||
           point[1] < this->min[1] ||
           point[1] > this->max[1] ||
           point[2] < this->min[2] ||
           point[2] > this->max[2]);
}

/*!
  Check if the given \a box lies wholly or partly within the boundaries
  of this box.
 */
SbBool
SbBox3d::intersect(const SbBox3d & box) const
{
  if ((box.max[0] < this->min[0]) ||
     (box.max[1] < this->min[1]) ||
     (box.max[2] < this->min[2]) ||
     (box.min[0] > this->max[0]) ||
     (box.min[1] > this->max[1]) ||
     (box.min[2] > this->max[2])) return FALSE;
  return TRUE;
}

/*!
  Reset the boundaries of the box.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.

  \sa getBounds().
*/
void
SbBox3d::setBounds(const double minx, const double miny, const double minz,
                   const double maxx, const double maxy, const double maxz)
{
#if COIN_DEBUG
  if (minx>maxx || miny>maxy || minz>maxz) {
    SoDebugError::postWarning("SbBox3d::setBounds",
                              "invalid box specification: "
                              "min==<%f, %f, %f> max==<%f, %f, %f>",
                              minx, miny, minz, maxx, maxy, maxz);
  }
#endif // COIN_DEBUG
  
  this->min.setValue(minx, miny, minz);
  this->max.setValue(maxx, maxy, maxz);
}

/*!
  Reset the boundaries of the box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.

  \sa getBounds().
 */
void
SbBox3d::setBounds(const SbVec3d & minval, const SbVec3d & maxval)
{
#if COIN_DEBUG
  if (minval[0]>maxval[0] || minval[1]>maxval[1] || minval[2]>maxval[2]) {
    SoDebugError::postWarning("SbBox3d::setBounds",
                              "invalid box specification: "
                              "min==<%f, %f, %f> max==<%f, %f, %f>",
                              minval[0], minval[1], minval[2],
                              maxval[0], maxval[1], maxval[2]);
  }
#endif // COIN_DEBUG

  this->min = minval;
  this->max = maxval;
}

/*!
  Returns the box boundaries.

  \sa setBounds().
*/
void
SbBox3d::getBounds(double & minx, double & miny, double & minz,
                   double & maxx, double & maxy, double & maxz) const
{
  this->min.getValue(minx, miny, minz);
  this->max.getValue(maxx, maxy, maxz);
}

/*!
  Returns the box corner points.

  \sa setBounds().
*/
void
SbBox3d::getBounds(SbVec3d & minobj, SbVec3d & maxobj) const
{
  minobj = this->min;
  maxobj = this->max;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox3d::getOrigin(double & x0, double & y0, double & z0) const
{
  this->min.getValue(x0, y0, z0);
}

/*!
  Returns width, height and depth of box.
 */
void
SbBox3d::getSize(double & dx, double & dy, double & dz) const
{
#if COIN_DEBUG
  if (this->isEmpty()) {
    SoDebugError::postWarning("SbBox3d::getSize", "The box is empty.");
  }
#endif // COIN_DEBUG

  dx = this->max[0] - this->min[0];
  dy = this->max[1] - this->min[1];
  dz = this->max[2] - this->min[2];
}

/*!
  Marks this as an empty box.

  \sa isEmpty().
 */
void
SbBox3d::makeEmpty(void)
{
  this->min.setValue(DBL_MAX, DBL_MAX, DBL_MAX);
  this->max.setValue(-DBL_MAX, -DBL_MAX, -DBL_MAX);
}

/*!
  Check if this has been marked as an empty box.

  \sa makeEmpty().
 */
SbBool
SbBox3d::isEmpty(void) const
{
  return this->max[0] < this->min[0];
}

/*!
  Check if the box has been correctly specified and by that virtue
  has volume.
 */
SbBool
SbBox3d::hasVolume(void) const
{
  return (this->max[0] > this->min[0] &&
          this->max[1] > this->min[1] &&
          this->max[2] > this->min[2]);
}

/*!
  Check if the box has "positive" volume, i.e. the lower left corner is
  actually lower and more left than the maximum point.
 */
double
SbBox3d::getVolume(void) const
{
  if (!this->hasVolume()) { return 0.0; }

  return
    (this->max[0] - this->min[0]) *
    (this->max[1] - this->min[1]) *
    (this->max[2] - this->min[2]);
}

/*!
  Find the span of the box in the given direction (i.e. how much room in
  the given direction the box needs). The distance is returned as the minimum
  and maximum distance from origo to the closest and furthest plane defined
  by the direction vector and each of the box' corners. The difference
  between these values gives the span.
*/
void
SbBox3d::getSpan(const SbVec3d & dir, double & dmin, double & dmax) const
{
  double dist, mindist=DBL_MAX, maxdist=-DBL_MAX;
  SbVec3d points[2]={this->min, this->max};
  SbVec3d corner;
  SbVec3d normdir(dir);
  if (normdir.normalize() == 0.0) {
#if COIN_DEBUG
    SoDebugError::postWarning("SbBox3d::getSpan",
                              "The direction is a null vector.");
#endif // COIN_DEBUG

    dmin = dmax = 0.0;
    return;
  }

  for (int i=0;i<8;i++) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0], points[(i&2)>>1][1], points[i&1][2]);

    // a = dir
    // b = corner-[0, 0, 0]
    // v = dir/|dir|
    //
    // The dot product formula (1) gives the formula we use (2).
    //
    // (1)  a * b = |a|*|b| * cos(angle(a, b))
    //
    // (2)  distance = v * b
    //
    // This dot product (2) gives the distance from origo to the plane
    // defined by normdir and corner.
    dist=normdir.dot(corner);
    if (dist < mindist) mindist = dist;
    if (dist > maxdist) maxdist = dist;
  }

  dmin = mindist;
  dmax = maxdist;
}

/*!
  Transform the box by the matrix, and change its boundaries to contain
  the transformed box.

  Doesn't touch illegal/empty boxes.
 */
void
SbBox3d::transform(const SbDPMatrix & matrix)
{
#if COIN_DEBUG
  if (this->isEmpty()) {
    SoDebugError::postWarning("SbBox3d::transform",
                              "The box is not valid!");
    return;
  }
#endif // COIN_DEBUG

  SbVec3d dst;
  SbVec3d points[2]={this->min, this->max};
  SbVec3d corner;
  SbBox3d newbox;

  //transform all the corners and include them into the new box.
  for (int i=0;i<8;i++) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0], points[(i&2)>>1][1], points[i&1][2]);
    matrix.multVecMatrix(corner, dst);
    newbox.extendBy(dst);
  }
  this->setBounds(newbox.min, newbox.max);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbBox3d::print(FILE * fp) const
{
#if COIN_DEBUG
  SbVec3d minv, maxv;
  this->getBounds(minv, maxv);
  minv.print(fp);
  fprintf(fp, " ");
  maxv.print(fp);
#endif // COIN_DEBUG
}

/*!
  \relates SbBox3d

  Check \a b1 and \a b2 for equality.
*/
int
operator ==(const SbBox3d & b1, const SbBox3d & b2)
{
  return b1.getMin() == b2.getMin() && b1.getMax() == b2.getMax();
}

/*!
  \relates SbBox3d

  Check \a b1 and \a b2 for inequality.
*/
int
operator !=(const SbBox3d & b1, const SbBox3d & b2)
{
  return !(b1 == b2);
}

/*!
  Check if the box is outside the view volume defined by the \a mvp
  matrix. Sets \a cullbits according to which planes we're inside or
  outside. Bit 0 (0x1) is cleared when box is completely inside left
  and right clipping planes. Bit 1 (0x2) is cleared when box is inside
  top and bottom clipping planes. Bit 2 (0x4) is cleared when box is
  inside near and far clipping planes.

  Returns \c TRUE if box is completely outside one of the clipping
  planes. \c FALSE otherwise.
*/
SbBool
SbBox3d::outside(const SbDPMatrix & mvp, int & cullbits) const
{
  // FIXME: this function is untested (code written by
  // pederb). 20000615 mortene.

  int i;
  SbVec3d tmp;
  SbVec3d clip[8];
  for (i = 0; i < 8; i++) {
    tmp[0] = i & 4 ? this->min[0] : this->max[0];
    tmp[1] = i & 2 ? this->min[1] : this->max[1];
    tmp[2] = i & 1 ? this->min[2] : this->max[2];
    mvp.multVecMatrix(tmp, clip[i]);
  }
  for (int j = 0; j < 3; j++) {
    if (cullbits & (1<<j)) {
      int inside = 0;
      int outsideneg = 0;
      int outsidepos = 0;
      for (i = 0; i < 8; i++) {
        double val = clip[i][j];
        if (val < -1.0) outsideneg++;
        else if (val > 1.0) outsidepos++;
        else inside++;
      }
      if (outsidepos == 8) return TRUE;
      if (outsideneg == 8) return TRUE;
      if (inside == 8) cullbits ^= (1<<j);
    }
  }
  return FALSE;
}

/*!
  Return the point on the box closest to the given \a point.
 */
SbVec3d
SbBox3d::getClosestPoint(const SbVec3d & point) const
{
  SbVec3d closest = point;

  SbVec3d center = this->getCenter();
  double devx = closest[0] - center[0];
  double devy = closest[1] - center[1];
  double devz = closest[2] - center[2];
  double halfwidth = (this->max[0] - this->min[0]) / 2.0;
  double halfheight = (this->max[1] - this->min[1]) / 2.0;
  double halfdepth = (this->max[2] - this->min[2]) / 2.0;

  // Move point to be on the nearest plane of the box.
  if ((fabs(devx) > fabs(devy)) && (fabs(devx) > fabs(devz)))
    closest[0] = center[0] + halfwidth * ((devx < 0.0) ? -1.0 : 1.0);
  else if (fabs(devy) > fabs(devz))
    closest[1] = center[1] + halfheight * ((devy < 0.0) ? -1.0 : 1.0);
  else
    closest[2] = center[2] + halfdepth * ((devz < 0.0) ? -1.0 : 1.0);

  // Clamp to be inside box.
  closest[0] = SbMin(SbMax(closest[0], this->min[0]), this->max[0]);
  closest[1] = SbMin(SbMax(closest[1], this->min[1]), this->max[1]);
  closest[2] = SbMin(SbMax(closest[2], this->min[2]), this->max[2]);

  return closest;
}
