/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
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
  \class SoBoundingBoxCache include/Inventor/caches/SoBoundingBoxCache.h
  \brief The SoBoundingBoxCache class is used to cache bounding boxes.
  \ingroup caches
*/

#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoCacheElement.h>

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
  Sets the flag returned from SoBoundingBoxCache::hasLinesOrPoints()
  to \c TRUE for all open bounding box caches.

  The reason bounding box caches keep a lines-or-points flag is to
  make it known to client code if the shape(s) they contain have any
  of these primitives -- or are rendered with these primitives. The
  reason this is important to know for the client code is because it
  might need to add an "epsilon" slack value to the calculated
  bounding box to account for smoothing / anti-aliasing effects in the
  renderer, so lines and points graphics is not accidently clipped by
  near and far clipping planes, for instance.

  \sa hasLinesOrPoints()
*/
void
SoBoundingBoxCache::setHasLinesOrPoints(SoState * state)
{
  SoCacheElement * elem = (SoCacheElement*)
    state->getElementNoPush(SoCacheElement::getClassStackIndex());

  while (elem) {
    SoBoundingBoxCache * cache = (SoBoundingBoxCache*) elem->getCache();
    if (cache) cache->linesOrPoints = TRUE;
    elem = elem->getNextCacheElement();
  }
}

/*!
  Return \c TRUE if the hasLinesOrPoints flag has been set.

  \sa setHasLinesOrPoints()
*/
SbBool
SoBoundingBoxCache::hasLinesOrPoints(void) const
{
  return this->linesOrPoints == 1;
}
