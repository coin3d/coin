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
  \class SoFaceDetail SoFaceDetail.h Inventor/details/SoFaceDetail.h
  \brief The SoFaceDetail class is for storing detailed polygon information.
  \ingroup details

  Instances of this class are used among other things for storing
  information about polygons after pick operations, and for storing
  information returned to tessellation callbacks.

  Note that a SoFaceDetail instance consists of a set of SoPointDetail
  instances, one for each vertex of the polygon it represents.

  \sa SoRayPickAction, SoPickedPoint, SoCallbackAction
*/

#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/SbName.h>
#include <stddef.h>

SO_DETAIL_SOURCE(SoFaceDetail);

/*!
  Default constructor sets up an empty, non-valid detail
  specification.
 */
SoFaceDetail::SoFaceDetail(void)
  : pointsarray(NULL),
    numallocated(0),
    numpoints(0),
    faceindex(0),
    partindex(0)
{
}

/*!
  Destructor, free internal resources used for storing the polygon
  vertices.
 */
SoFaceDetail::~SoFaceDetail()
{
  delete [] this->pointsarray;
}

// doc in super
void
SoFaceDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoFaceDetail, SoDetail);
}

// doc in super
SoDetail *
SoFaceDetail::copy(void) const
{
  SoFaceDetail *copy = new SoFaceDetail();
  if (this->numpoints) {
    copy->setNumPoints(this->numpoints);
    for (int i = 0; i < this->numpoints; i++) {
      copy->setPoint(i, this->getPoint(i));
    }
  }
  copy->faceindex = this->faceindex;
  copy->partindex = this->partindex;
  return copy;
}

/*!
  Number of vertices making up the polygon.
 */
int
SoFaceDetail::getNumPoints(void) const
{
  return this->numpoints;
}

/*!
  Returns a pointer into the array of vertices, starting at the \a
  idx'th vertice of the polygon.

  The array will contain (SoFaceDetail::getNumPoints() - \a idx)
  elements.
 */
const SoPointDetail *
SoFaceDetail::getPoint(const int idx) const
{
  assert(idx >= 0 && idx < this->numpoints);
  return &this->pointsarray[idx];
}

/*!
  Returns the full array of vertice details for the polygon. The array
  will contain SoFaceDetail::getNumPoints() elements.
 */
SoPointDetail *
SoFaceDetail::getPoints(void)
{
  return this->pointsarray;
}

/*!
  Returns the index of this polygon within the faceset node it is part
  of.
 */
int
SoFaceDetail::getFaceIndex(void) const
{
  return this->faceindex;
}

/*!
  If this SoFaceDetail represents a triangle tessellated from a complex
  shape, this method returns the index of the part of the complex
  shape it was tessellated from.
 */
int
SoFaceDetail::getPartIndex(void) const
{
  return this->partindex;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.

  \sa getNumPoints()
 */
void
SoFaceDetail::setNumPoints(const int num)
{
  if (num > this->numallocated) {
    this->numallocated = num;
    delete [] this->pointsarray;
    this->pointsarray = new SoPointDetail[this->numallocated];
  }
  this->numpoints = num;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.

  \sa getPoint(), getPoints()
 */
void
SoFaceDetail::setPoint(const int idx, const SoPointDetail * const detail)
{
  assert(idx >= 0 && idx <= this->numpoints);
  this->pointsarray[idx] = *detail;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.

  \sa getFaceIndex()
 */
void
SoFaceDetail::setFaceIndex(const int idx)
{
  this->faceindex = idx;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.

  \sa getPartIndex()
 */
void
SoFaceDetail::setPartIndex(const int idx)
{
  this->partindex = idx;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.
 */
void
SoFaceDetail::incFaceIndex(void)
{
  this->faceindex++;
}

/*!
  Used internally from library client code setting up a SoFaceDetail
  instance.
 */
void
SoFaceDetail::incPartIndex(void)
{
  this->partindex++;
}
