/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoBoundingBoxCache include/Inventor/caches/SoBoundingBoxCache.h
  \brief The SoBoundingBoxCache class is used to cache bounding boxes.

*/

#include <Inventor/caches/SoBoundingBoxCache.h>
#include <coindefs.h> // COIN_STUB()

/*!
  Constructor with \a state being the current state.
*/
SoBoundingBoxCache::SoBoundingBoxCache(SoState *state)
  : SoCache(state),
    centerSet(0),
    linesOrPoints(0)
{
}

/*!
  Destructor.
*/
SoBoundingBoxCache::~SoBoundingBoxCache()
{
}

/*!
  Sets the data for this cache. \a boundingBox is the node's bounding
  box, \a centerSet and \a centerPoints specifies the center of the
  geometry inside \a boundingBox.
*/
void
SoBoundingBoxCache::set(const SbXfBox3f &boundingBox,
                        SbBool centerSet,
                        const SbVec3f &centerPoint)
{
  this->bbox = boundingBox;
  this->localBBox = boundingBox.project();
  this->centerSet = centerSet ? 1 : 0;
  this->centerPoint = centerPoint;
}

/*!
  Returns the bounding box for this cache.
*/
const SbXfBox3f &
SoBoundingBoxCache::getBox() const
{
  return this->bbox;
}

/*!
  Returns the projected bounding box for this cache.
*/
const SbBox3f &
SoBoundingBoxCache::getProjectedBox() const
{
  return this->localBBox;
}

/*!
  Returns whether the center of the bounding box was set in the
  SoBoundingBoxCache::set() method.

  \sa SoBoundingBoxCache::getCenter()
*/
SbBool
SoBoundingBoxCache::isCenterSet() const
{
  return this->centerSet == 1;
}

/*!
  Returns the center of the bounding box. Should only be used if
  SoBoundingBoxCache::isCenterSet() returns \c TRUE.
*/
const SbVec3f &
SoBoundingBoxCache::getCenter() const
{
  return this->centerPoint;
}

/*!
  Sets the hasLinesOrPoints flag for all open bounding box caches.

  Ignored for now, but still provided for Open Inventor compliance.

  \sa SoBoundingBoxCache::hasLinesOrPoints()
*/
void
SoBoundingBoxCache::setHasLinesOrPoints(SoState *state)
{
  // FIXME: as far as I can tell, the idea is that bounding boxes
  // should be given a little bit of slack if they contain lines
  // and/or points (as those primitives doesn't really have any
  // volume). 20000424 mortene.

  if (state) { }
}

/*!
  Return \c TRUE if the hasLinesOrPoints flag has been set.
  Not used for the moment. Provided for OIV compliance.
*/
SbBool
SoBoundingBoxCache::hasLinesOrPoints() const
{
  return this->linesOrPoints == 1;
}
