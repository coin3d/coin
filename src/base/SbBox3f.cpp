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
  \class SbBox3f SbBox.h Inventor/SbBox.h
  \brief The SbBox3f class is a 3 dimensional box with floating
  point coordinates.
  \ingroup base

  This box class is used by other classes in Coin for data exchange
  and storage. It provides representation of the defining corners of a
  box in 3D space.

  \sa SbBox2f, SbBox2s, SbXfBox3f 
*/


#include <Inventor/SbBox3f.h>
#include <Inventor/SbMatrix.h>
#include <iostream.h>
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <iostream.h> // For print() functionality.
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
SbBox3f::SbBox3f(const SbVec3f& min, const SbVec3f& max)
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
  Returns the minimum point. This should usually be the lower left corner
  point of the box.

  \sa getOrigin(), getMax().
*/
const SbVec3f& 
SbBox3f::getMin(void) const
{
  return min;
}

/*!
  Returns the maximum point. This should usually be the upper right corner
  point of the box.

  \sa getMin().
*/
const SbVec3f& 
SbBox3f::getMax(void) const
{
  return max;
}

/*!
  Returns the center point of the box.
 */
SbVec3f 
SbBox3f::getCenter(void) const
{
#if COIN_DEBUG
  if(this->isEmpty())
    SoDebugError::postWarning("SbBox3f::getCenter",
			      "The box is empty.");
#endif // COIN_DEBUG
  return SbVec3f((max[0] + min[0])*0.5f,
		 (max[1] + min[1])*0.5f,
		 (max[2] + min[2])*0.5f);
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.
 */
void 
SbBox3f::extendBy(const SbVec3f& point)
{
  if (this->isEmpty()) {
    this->setBounds(point, point);
  }
  else {
    this->min.setValue(SbMin(point[0], this->min[0]),
		       SbMin(point[1], this->min[1]),
		       SbMin(point[2], this->min[2]));
    this->max.setValue(SbMax(point[0], this->max[0]),
		       SbMax(point[1], this->max[1]),
		       SbMax(point[2], this->max[2]));
  }
}

/*!
  Extend the boundaries of the box by the given \a box parameter. This
  is equal to calling extendBy() twice with the corner points.
 */
void 
SbBox3f::extendBy(const SbBox3f& box)
{
#if COIN_DEBUG
  if(box.isEmpty()) {
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
SbBox3f::intersect(const SbVec3f& point) const
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
SbBox3f::intersect(const SbBox3f& box) const
{
  if((box.max[0] < this->min[0]) ||
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
  if(!(minx<=maxx && miny<=maxy && minz<= maxz)) {
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
SbBox3f::setBounds(const SbVec3f& min, const SbVec3f& max)
{
#if COIN_DEBUG
  if(!(min[0]<=max[0] && min[1]<=max[1] && min[2]<=max[2])) {
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
SbBox3f::getBounds(float& minx, float& miny, float& minz, 
			float& maxx, float& maxy, float& maxz) const
{
  this->min.getValue(minx, miny, minz);
  this->max.getValue(maxx, maxy, maxz);
}

/*!
  Returns the box corner points.

  \sa setBounds().
*/
void
SbBox3f::getBounds(SbVec3f& min, SbVec3f& max) const
{
  min = this->min;
  max = this->max;
}

/*!
  Returns the coordinates of the box origin (i.e. the lower left corner).

  \sa getMin().
*/
void
SbBox3f::getOrigin(float& x0, float& y0, float& z0) const
{
  this->min.getValue(x0,y0,z0);
}

/*!
  Returns width, height and depth of box.
 */
void 
SbBox3f::getSize(float& dx, float& dy, float& dz) const
{
#if COIN_DEBUG
  if(this->isEmpty()) SoDebugError::postWarning("SbBox3f::getSize",
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
  // NB: the volume will not be correct if the box has been
  // rotated. This is (oddly enough) the way it works in Open
  // Inventor (?). Should probably be fixed. --mortene

  // FIXME: a better method has been implemented in
  // SbXfBox3f::getVolume(). Tear out the necessary code from
  // there. 19990316 mortene.

  if (this->hasVolume())
    return ((this->max[0]-this->min[0]) * 
	    (this->max[1]-this->min[1]) * 
	    (this->max[2]-this->min[2]));
  else return 0.0f;
}

/*!
  Find the span of the box in the given direction (i.e. how much room in
  the given direction the box needs). The distance is returned as the minimum
  and maximum distance from origo to the closest and furthest plane defined 
  by the direction vector and each of the box' corners. The difference 
  between these values gives the span.
*/
void
SbBox3f::getSpan(const SbVec3f& dir, float& dMin, float& dMax) const
{
  float dist,mindist=FLT_MAX,maxdist=-FLT_MAX;
  SbVec3f points[2]={this->min,this->max};
  SbVec3f corner;
  SbVec3f normdir(dir);
  normdir.normalize(); //To save a division
  
  for (int i=0;i<8;i++) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0],points[(i&2)>>1][1],points[i&1][2]);

    // a = dir
    // b = corner-[0,0,0]
    // v = dir/|dir|
    //
    // The dot product formula (1) gives the formula we use (2).
    //
    // (1)  a * b = |a|*|b| * cos(angle(a,b))
    //
    // (2)  distance = v * b
    //
    // This dot product (2) gives the distance from origo to the plane
    // defined by normdir and corner.
    dist=normdir.dot(corner);
    if (dist<mindist) mindist=dist;
    if (dist>maxdist) maxdist=dist;
  }

  dMin=mindist;
  dMax=maxdist;
}

/*!
  Transform the box by the matrix, and change its boundaries to contain
  the transformed box.

  Doesn't touch illegal/empty boxes.
 */
void
SbBox3f::transform(const SbMatrix& matrix)
{
#if COIN_DEBUG
  if(this->isEmpty()) {
    SoDebugError::postWarning("SbBox3f::transform",
			      "The box is not valid!");
    return;
  }
#endif // COIN_DEBUG

  SbVec3f dst;
  SbVec3f points[2]={this->min,this->max};
  SbVec3f corner;
  SbBox3f newbox;

  //transform all the corners and include them into the new box.
  for (int i=0;i<8;i++) {
    //Find all corners the "binary" way :-)
    corner.setValue(points[(i&4)>>2][0],points[(i&2)>>1][1],points[i&1][2]);
    matrix.multVecMatrix(corner,dst);
    newbox.extendBy(dst);
  }
  this->setBounds(newbox.min,newbox.max);
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbBox3f::print(ostream & file) const
{
#if COIN_DEBUG
  SbVec3f minv, maxv;
  this->getBounds(minv, maxv);
  minv.print(file); cout << " "; maxv.print(file);
#endif // COIN_DEBUG
}
