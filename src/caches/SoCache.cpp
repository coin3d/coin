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
  \class SoCache include/Inventor/caches/SoCache.h
  \brief The SoCache class is the superclass for all internal cache classes.
  \ingroup caches

  It organizes reference counting to make it possible to share cache
  instances. It also organizes a list of elements that will affect the
  cache. If any of the elements have changed since the cache was
  created, the cache is invalid.
*/

#include <Inventor/caches/SoCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoElement.h>
#include <Inventor/lists/SbList.h>
#include <string.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbName.h>
#endif // COIN_DEBUG

#ifndef DOXYGEN_SKIP_THIS

class SoCacheP {
public:
  SbList <SoElement *> elements;
  unsigned char * elementflags;
  int refcount;
  SbBool invalidated;
  int statedepth;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor with \a state being the current state.
*/
SoCache::SoCache(SoState * const state)
{
  THIS = new SoCacheP;
  THIS->elementflags = NULL;
  THIS->refcount = 0;
  THIS->invalidated = FALSE;
  THIS->statedepth = state ? state->getDepth() : 0;

  int numidx = SoElement::getNumStackIndices();
  int numbytes = (numidx >> 3) + 1;
  // one bit per element is used to quickly determine whether an
  // element of a given type already has been added.
  THIS->elementflags = new unsigned char[numbytes];
  memset(THIS->elementflags, 0, numbytes);
}

/*!
  Destructor
*/
SoCache::~SoCache()
{
  delete [] THIS->elementflags;

  int n = THIS->elements.getLength();
  for (int i = 0; i < n; i++) {
    delete THIS->elements[i];
  }
  delete THIS;
}

/*!
  Increases the reference count by one.
*/
void
SoCache::ref(void)
{
  THIS->refcount++;
}

/*!
  Decreases the reference count by one. When the reference count reaches
  zero, the cache is deleted. The SoCache::destroy() method is called
  before the destructor is called.
*/
void
SoCache::unref(SoState *state)
{
  assert(THIS->refcount > 0);
  if (--THIS->refcount == 0) {
    this->destroy(state);
    delete this;
  }
}

/*!
  Adds \a elem to the list of elements this cache depends on.
*/
void
SoCache::addElement(const SoElement * const elem)
{
  if (elem->getDepth() < THIS->statedepth) {
    int idx = elem->getStackIndex();
    int flag = 0x1 << (idx & 0x7);
    idx >>= 3; // get byte number
    if (!(THIS->elementflags[idx] & flag)) {
#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoCache::addElement",
                             "cache: %p, elem: %s", this,
                             elem->getTypeId().getName().getString());
#endif // debug
      SoElement * copy = elem->copyMatchInfo();
      if (copy) THIS->elements.append(copy);
      THIS->elementflags[idx] |= flag;
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
  int n = cache->pimpl->elements.getLength();
  const SoElement * const * ptr = cache->pimpl->elements.getArrayPtr();
  for (int i = 0; i < n; i++) {
    // use elements in state to get correct element depth
    this->addElement(state->getConstElement(ptr[i]->getStackIndex()));
  }
}

/*!
  Return \e TRUE if this cache is valid, \e FALSE otherwise.
*/
SbBool
SoCache::isValid(const SoState * state) const
{
  if (THIS->invalidated) return FALSE;
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
  if (THIS->invalidated) return NULL;

  // use local variables for speed
  int n = THIS->elements.getLength();
  const SoElement * const * ptr = THIS->elements.getArrayPtr();
  const SoElement * elem;
  for (int i = 0; i < n; i++) {
    elem = ptr[i];
    if (!elem->matches(state->getConstElement(elem->getStackIndex()))) {
#if COIN_DEBUG && 0 // debug
      SoDebugError::postInfo("SoCache::getInvalidElement",
                             "cache: %p, invalid element: %s", this,
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
  THIS->invalidated = TRUE;
}

/*!
  Can be overridden by subclasses to clean up before they are
  deleted. Default method does nothing.
*/
void
SoCache::destroy(SoState *)
{
}





