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
  \class SbBox3f SbBox.h Inventor/SbBox.h
  \brief The SbBox3f class is an abstraction for an axis aligned 3 dimensional box.
  \ingroup base

  This box abstraction class is used by other entities in the Coin
  library for data exchange and storage. It provides a representation
  of the defining corners of a box in 3D space, with the sides aligned
  with the 3 principal axes.

  \sa SbBox2f, SbBox2s, SbXfBox3f
*/


#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>
#include <assert.h>
#include <float.h> // FLT_MAX

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  The default constructor makes an empty box.
 */
SbBox3f::SbBox3f(void)
{
  this->makeEmpty();
}

/*!
  Constructs a box with the given corners.

  \a minx should be less than \a maxx, \a miny should be less than
  \a maxy and \a minz should be less than \a maxz if you want to make
  a valid box.
 */
SbBox3f::SbBox3f(const float minx, const float miny, const float minz,
                 const float maxx, const float maxy, const float maxz)
{
  this->min.setValue(minx, miny, minz);
  this->max.setValue(maxx, maxy, maxz);
}

/*!
  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbBox3f::SbBox3f(const SbVec3f & min, const SbVec3f & max)
{
  this->min = min;
  this->max = max;
}

/*!
  Default destructor does nothing.
 */
SbBox3f::~SbBox3f(void)
{
}

/*!
  Returns the minimum point. This should usually be the lower left
  corner point of the box.

  \sa getOrigin(), getMax().
*/
const SbVec3f &
SbBox3f::getMin(void) const
{
  return this->min;
}

/*!
  Returns the maximum point. This should usually be the upper right
  corner point of the box.

  \sa getMin().
*/
const SbVec3f &
SbBox3f::getMax(void) const
{
  return this->max;
}

/*!
  Returns a modifiable reference the minimum point.
*/
SbVec3f &
SbBox3f::getMin(void)
{
  return this->min;
}

/*!
  Returns a modifiable reference the maximum point.
*/
SbVec3f &
SbBox3f::getMax(void)
{
  return this->max;
}

/*!
  Returns the center point of the box.
 */
SbVec3f
SbBox3f::getCenter(void) const
{
#if COIN_DEBUG
  if (this->isEmpty())
    SoDebugError::postWarning("SbBox3f::getCenter",
                              "The box is empty.");
#endif // COIN_DEBUG
  return SbVec3f((this->max[0] + this->min[0]) * 0.5f,
                 (this->max[1] + this->min[1]) * 0.5f,
                 (this->max[2] + this->min[2]) * 0.5f);
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.
 */
void
SbBox3f::extendBy(const SbVec3f & point)
{
  if (this->isEmpty()) {
    this->setBounds(point, point);
  }
  else {
    // The explicit casts are done to humour the HPUX aCC compiler,
    // which will otherwise say ``Template deduction failed to find a
    // match for the call to 'SbMin'''. mortene.
    this->min.setValue(SbMin((float)point[0], (float)this->min[0]),
                       SbMin((float)point[1], (float)this->min[1]),
                       SbMin((float)point[2], (float)this->min[2]));
    this->max.setValue(SbMax((float)point[0], (float)this->max[0]),
                       SbMax((float)point[1], (float)this->max[1]),
                       SbMax((float)point[2], (float)this->max[2]));
  }
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
 */
void
SbBox3f::extendBy(const SbBox3f & box)
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SbBox3f::extendBy",
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
SbBox3f::intersect(const SbVec3f & point) const
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
SbBox3f::intersect(const SbBox3f & box) const
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
SbBox3f::setBounds(const float minx, const float miny, const float minz,
                   const float maxx, const float maxy, const float maxz)
{
#if COIN_DEBUG
  if (!(minx<=maxx && miny<=maxy && minz<= maxz)) {
    SoDebugError::postWarning("SbBox3f::setBounds",
                              "The box is not valid.");
    assert(0);
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
SbBox3f::setBounds(const SbVec3f & min, const SbVec3f & max)
{
#if COIN_DEBUG
  if (!(min[0]<=max[0] && min[1]<=max[1] && min[2]<=max[2])) {
    SoDebugError::postWarning("SbBox3f::setBounds",
                              "The box is not valid.");
    assert(0);
  }
#endif // COIN_DEBUG

  this->min = min;
  this->max = max;
}

/*!
  Returns the box boundaries.

  \sa setBounds().
*/
void
SbBox3f::getBounds(float & minx, float & miny, float & minz,
                   float & maxx, float & maxy, float & maxz) const
{
  this->min.getValue(minx, miny, minz);
  this->max.getValue(maxx, maxy, maxz);
}

/*!
  Returns the box corner points.

  \sa setBounds().
*/
void
SbBox3f::getBounds(SbVec3f & min, SbVec3f & max) const
{
  min = this->min;
  max = this->max;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox3f::getOrigin(float & x0, float & y0, float & z0) const
{
  this->min.getValue(x0, y0, z0);
}

/*!
  Returns width, height and depth of box.
 */
void
SbBox3f::getSize(float & dx, float & dy, float & dz) const
{
#if COIN_DEBUG
  if (this->isEmpty()) SoDebugError::postWarning("SbBox3f::getSize",
                                                "The box is not valid.");
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
SbBox3f::makeEmpty(void)
{
  this->min.setValue(FLT_MAX, FLT_MAX, FLT_MAX);
  this->max.setValue(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

/*!
  Check if this has been marked as an empty box.

  \sa makeEmpty().
 */
SbBool
SbBox3f::isEmpty(void) const
{
  return this->max[0] < this->min[0];
}

/*!
  Check if the box has been correctly specified and by that virtue
  has volume.
 */
SbBool
SbBox3f::hasVolume(void) const
{
  return (this->max[0] > this->min[0] &&
          this->max[1] > this->min[1] &&
          this->max[2] > this->min[2]);
}

/*!
  Check if the box has "positive" volume, i.e. the lower left corner is
  actually lower and more left than the maximum point.
 */
float
SbBox3f::getVolume(void) const
{
  if (!this->hasVolume()) { return 0.0f; }

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
SbBox3f::getSpan(const SbVec3f & dir, float & dmin, float & dmax) const
{
  float dist, mindist=FLT_MAX, maxdist=-FLT_MAX;
  SbVec3f points[2]={this->min, this->max};
  SbVec3f corner;
  SbVec3f normdir(dir);
  normdir.normalize(); // To avoid a division

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
SbBox3f::transform(const SbMatrix & matrix)
{
#if COIN_DEBUG
  if (this->isEmpty()) {
    SoDebugError::postWarning("SbBox3f::transform",
                              "The box is not valid!");
    return;
  }
#endif // COIN_DEBUG

  SbVec3f dst;
  SbVec3f points[2]={this->min, this->max};
  SbVec3f corner;
  SbBox3f newbox;

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
SbBox3f::print(FILE * fp) const
{
#if COIN_DEBUG
  SbVec3f minv, maxv;
  this->getBounds(minv, maxv);
  minv.print(fp);
  fprintf(fp, " ");
  maxv.print(fp);
#endif // COIN_DEBUG
}

/*!
  \relates SbBox3f

  Check \a b1 and \a b2 for equality.
*/
int
operator ==(const SbBox3f & b1, const SbBox3f & b2)
{
  return b1.getMin() == b2.getMin() && b1.getMax() == b2.getMax();
}

/*!
  \relates SbBox3f

  Check \a b1 and \a b2 for inequality.
*/
int
operator !=(const SbBox3f & b1, const SbBox3f & b2)
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
SbBox3f::outside(const SbMatrix & mvp, int & cullbits) const
{
  // FIXME: this function is untested (code written by
  // pederb). 20000615 mortene.

  int i;
  SbVec3f tmp;
  SbVec3f clip[8];
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
        float val = clip[i][j];
        if (val < -1.0f) outsideneg++;
        else if (val > 1.0f) outsidepos++;
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
SbVec3f
SbBox3f::getClosestPoint(const SbVec3f & point) const
{
  SbVec3f closest = point;

  SbVec3f center = this->getCenter();
  float devx = closest[0] - center[0];
  float devy = closest[1] - center[1];
  float devz = closest[2] - center[2];
  float halfwidth = (this->max[0] - this->min[0]) / 2.0f;
  float halfheight = (this->max[1] - this->min[1]) / 2.0f;
  float halfdepth = (this->max[2] - this->min[2]) / 2.0f;

  // Move point to be on the nearest plane of the box.
  if ((fabs(devx) > fabs(devy)) && (fabs(devx) > fabs(devz)))
    closest[0] = center[0] + halfwidth * ((devx < 0.0f) ? -1.0f : 1.0f);
  else if (fabs(devy) > fabs(devz))
    closest[1] = center[1] + halfheight * ((devy < 0.0f) ? -1.0f : 1.0f);
  else
    closest[2] = center[2] + halfdepth * ((devz < 0.0f) ? -1.0f : 1.0f);

  // Clamp to be inside box.
  closest[0] = SbMin(SbMax(closest[0], this->min[0]), this->max[0]);
  closest[1] = SbMin(SbMax(closest[1], this->min[1]), this->max[1]);
  closest[2] = SbMin(SbMax(closest[2], this->min[2]), this->max[2]);

  return closest;
}
