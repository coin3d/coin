/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoBoundingBoxCache Inventor/caches/SoBoundingBoxCache.h
  \brief The SoBoundingBoxCache class is used to cache bounding boxes.
  \ingroup caches
*/

// *************************************************************************

#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/errors/SoDebugError.h>

#include "tidbitsp.h"

// *************************************************************************

class SoBoundingBoxCacheP {
public:
  SbXfBox3f bbox;
  SbBox3f localbbox;
  SbVec3f centerpoint;
  unsigned int centerset : 1;
  unsigned int linesorpoints : 1;
};

#define PRIVATE(p) ((p)->pimpl)

// *************************************************************************

/*!
  Constructor with \a state being the current state.
*/
SoBoundingBoxCache::SoBoundingBoxCache(SoState *state)
  : SoCache(state)
{
  PRIVATE(this) = new SoBoundingBoxCacheP;
  PRIVATE(this)->centerset = 0;
  PRIVATE(this)->linesorpoints = 0;

#if COIN_DEBUG
  if (coin_debug_caching_level() > 0) {
    SoDebugError::postInfo("SoBoundingBoxCache::SoBoundingBoxCache",
                           "Cache created: %p", this);
    
  }
#endif // debug
}

/*!
  Destructor.
*/
SoBoundingBoxCache::~SoBoundingBoxCache()
{
#if COIN_DEBUG
  if (coin_debug_caching_level() > 0) {
    SoDebugError::postInfo("SoBoundingBoxCache::~SoBoundingBoxCache",
                           "Cache destructed: %p", this);
  }
#endif // debug

  delete PRIVATE(this);
}

/*!
  Sets the data for this cache. \a boundingBox is the node's bounding
  box, \a centerSet and \a centerPoints specifies the center of the
  geometry inside \a boundingBox.
*/
void
SoBoundingBoxCache::set(const SbXfBox3f & boundingbox,
                        SbBool centerset,
                        const SbVec3f & centerpoint)
{
  PRIVATE(this)->bbox = boundingbox;
  PRIVATE(this)->localbbox = boundingbox.project();
  PRIVATE(this)->centerset = centerset ? 1 : 0;
  if (centerset) { PRIVATE(this)->centerpoint = centerpoint; }
}

/*!
  Returns the bounding box for this cache.
*/
const SbXfBox3f &
SoBoundingBoxCache::getBox(void) const
{
  return PRIVATE(this)->bbox;
}

/*!
  Returns the projected bounding box for this cache.
*/
const SbBox3f &
SoBoundingBoxCache::getProjectedBox(void) const
{
  return PRIVATE(this)->localbbox;
}

/*!
  Returns whether the center of the bounding box was set in the
  SoBoundingBoxCache::set() method.

  \sa SoBoundingBoxCache::getCenter()
*/
SbBool
SoBoundingBoxCache::isCenterSet(void) const
{
  return PRIVATE(this)->centerset == 1;
}

/*!
  Returns the center of the bounding box. Should only be used if
  SoBoundingBoxCache::isCenterSet() returns \c TRUE.
*/
const SbVec3f &
SoBoundingBoxCache::getCenter(void) const
{
  return PRIVATE(this)->centerpoint;
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

  This method is a static method on the class. It will upon invocation
  scan through the state stack and set the flag for all open
  SoBoundingBoxCache elements. It has been made to work like this so
  it can easily be invoked on all current bounding box cache instances
  from the SoShape-type nodes using lines and / or point primitives.

  \sa hasLinesOrPoints()
*/
void
SoBoundingBoxCache::setHasLinesOrPoints(SoState * state)
{
  SoCacheElement * elem = static_cast<SoCacheElement *>(
    state->getElementNoPush(SoCacheElement::getClassStackIndex())
    );

  while (elem) {
    SoBoundingBoxCache * cache = static_cast<SoBoundingBoxCache *>(elem->getCache());
    if (cache) { PRIVATE(cache)->linesorpoints = TRUE; }
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
  return PRIVATE(this)->linesorpoints == 1;
}

#undef PRIVATE
