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
  \class SoCache include/Inventor/caches/SoCache.h
  \brief The SoCache class is the supoerclass for all caches.
  It organizes reference counting to make it possible to share
  cache instances. It also organizes a list of elements that
  will affect the cache. If any of the elements have changed
  since the cache was created, the cache is invalid.
*/

#include <Inventor/caches/SoCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoElement.h>
#include <string.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Constructor with \a state being the current state.
*/
SoCache::SoCache(SoState * const state)
  : elements(),
  elementflags(NULL),
  refcount(0),
  invalidated(FALSE),
  statedepth(state ? state->getDepth() : 0)
{
  int numidx = SoElement::getNumStackIndices();
  int numbytes = (numidx >> 3) + 1;
  // one bit per element is used to quickly determine whether an
  // element of a given type already has been added.
  this->elementflags = new unsigned char[numbytes];
  memset(this->elementflags, 0, numbytes);
}

/*!
  Destructor
*/
SoCache::~SoCache()
{
  delete [] this->elementflags;
}

/*!
  Increases the reference count by one.
*/
void
SoCache::ref(void)
{
  this->refcount++;
}

/*!
  Decreases the reference count by one. When the reference count reaches
  zero, the cache is deleted. The SoCache::destroy() method is called
  before the destructor is called.
*/
void
SoCache::unref(SoState *state)
{
  if (--this->refcount == 0) {
    this->destroy(state);
    delete this;
  }
  assert(this->refcount > 0);
}

/*!
  Adds \a elem to the list of elements this cache depends on.
*/
void
SoCache::addElement(const SoElement * const elem)
{
  if (elem->getDepth() < this->statedepth) {
    int idx = elem->getStackIndex();
    int flag = 0x1 << (idx & 0x7);
    idx >>= 3; // get byte number
    if (!(this->elementflags[idx] & flag)) {
      SoElement * copy = elem->copyMatchInfo();
      if (copy) this->elements.append(copy);
      this->elementflags[idx] |= flag;
    }
  }
}

/*!
  Adds dependencies from \a cache to this cache.
*/
void
SoCache::addCacheDependency(const SoState * state, SoCache * cache)
{
  if (cache == this) return;

  // local variables for speed
  int n = cache->elements.getLength();
  const SoElement * const * ptr = cache->elements.getArrayPtr();
  for (int i = 0; i < n; i++) {
    this->addElement(ptr[i]);
  }
}

/*!
  Return \e TRUE if this cache is valid, \e FALSE otherwise.
*/
SbBool
SoCache::isValid(const SoState * state) const
{
  if (this->invalidated) return FALSE;
  return this->getInvalidElement(state) == NULL;
}

/*!
  Returns the element that caused the invalidation. Returns \e NULL
  if the cache is valid, or if the cache was not invalidated
  bacause of an element.
*/
const SoElement *
SoCache::getInvalidElement(const SoState * const state) const
{
  if (this->invalidated) return NULL;

  // use local variables for speed
  int n = this->elements.getLength();
  const SoElement * const * ptr = this->elements.getArrayPtr();
  const SoElement * elem;
  for (int i = 0; i < n; i++) {
    elem = ptr[i];
    if (!elem->matches(state->getConstElement(elem->getStackIndex()))) {
#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoCache::getInvalidElement",
                             "invalid element: %s",
                             elem->getTypeId().getName().getString());
#endif // debug
      return elem;
    }
  }
  return NULL;
}

/*!
  Forces a cache to be invalid.
*/
void
SoCache::invalidate(void)
{
  this->invalidated = TRUE;
}

/*!
  Can be overloaded by subclasses to clean up before they are
  deleted. Default method does nothing.
*/
void
SoCache::destroy(SoState *)
{
}
