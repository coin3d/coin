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
  \class SoCache include/Inventor/caches/SoCache.h
  \brief The SoCache class is the supoerclass for all caches.
  It organizes reference counting to make it possible to share
  cache instances. It also organizes a list of elements that
  will affect the cache. If any of the elements have changed
  since the cache was created, the cache is invalidated.
*/

#include <Inventor/caches/SoCache.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

/*!
  Constructor with \a state being the current state.
*/
SoCache::SoCache(SoState * const state)
{
  this->elementFlags = NULL;
  this->refCount = 0;
  this->invalidated = FALSE;
  if (state)
    this->stateDepth = state->getDepth();
  else 
    this->stateDepth = 0;
}

/*!
  Destructor
*/
SoCache::~SoCache()
{
  delete [] this->elementFlags;
}
  
/*!
  Increases the reference count by one.
*/
void 
SoCache::ref()
{
  this->refCount++;
}

/*!
  Decreases the reference count by one. When the reference count reaches
  zero, the cache is deleted. The SoCache::destroy() method is called
  before the destructor is called.
*/
void 
SoCache::unref(SoState *state)
{
  if (--this->refCount == 0) {
    destroy(state);
    delete this;
  }
  assert(this->refCount > 0);
}

/*!
  Adds \a elem to the list of elements this cache depends on.
*/
void 
SoCache::addElement(const SoElement * const elem)
{
  this->elements.append((void*)elem);
  // FIXME: what to do with elementsFlags
}

/*!
  Adds dependencies from \a cache to this cache.
*/
void 
SoCache::addCacheDependency(const SoState *state,
			    SoCache *cache)
{
  assert(0);
}

/*!
  Return \e TRUE if this cache is valid, \e FALSE otherwise.
*/ 
SbBool 
SoCache::isValid(const SoState *state) const
{
  if (this->invalidated) return FALSE;
  // FIXME:
  return TRUE;
}

/*!
  Returns the element that caused the invalidation. Returns \e NULL
  if the cache is valid, or if the cache was not invalidated 
  bacause of an element.
*/
const SoElement *
SoCache::getInvalidElement(const SoState * const state) const
{
  if (this->isValid(state) || this->invalidated) return NULL;

  // FIXME: find element that caused the invlidation
  return NULL;
}

/*!
  Forces an cache to be invalid.
*/
void 
SoCache::invalidate()
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
