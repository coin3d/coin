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
  \class SoCacheElement Inventor/elements/SoCacheElement.h
  \brief The SoCacheElement class stores and manages the opene caches.
  \ingroup elements
*/

#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/caches/SoCache.h>
#include <assert.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_THREADS
#include <Inventor/threads/SbTypedStorage.h>
#endif // HAVE_THREADS


#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SbBool SoCacheElement::invalidated = FALSE;


#ifdef HAVE_THREADS
SbTypedStorage <SbBool*> * invalidated_storage = NULL;

static void
cacheelement_cleanup(void)
{
  delete invalidated_storage;
}

#endif // HAVE_THREADS

SO_ELEMENT_SOURCE(SoCacheElement);

/*!
  This static method initializes static data for the SoCacheElement class.
*/

void
SoCacheElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCacheElement, inherited);

#ifdef HAVE_THREADS
  invalidated_storage = new SbTypedStorage <SbBool*> (sizeof(SbBool));
  *(invalidated_storage->get()) = FALSE;
  atexit(cacheelement_cleanup);
#endif // HAVE_THREADS
}

/*!
  The destructor.
*/

SoCacheElement::~SoCacheElement(void)
{
}

/*!
  Initializes element in state stack.
*/

void
SoCacheElement::init(SoState * state)
{
  inherited::init(state);
  this->cache = NULL;
}

// Documented in superclass. Overridden to initialize element.
void
SoCacheElement::push(SoState * state)
{
  inherited::push(state);
  this->cache = NULL;
}

// Documented in superclass. Overridden to unref the cache, since the
// cache is ref'ed in set().
void
SoCacheElement::pop(SoState * state, const SoElement * prevTopElement)
{
  SoCacheElement * prev = (SoCacheElement*) prevTopElement;
  if (prev->cache) {
    prev->cache->unref();
    prev->cache = NULL;
  }
  inherited::pop(state, prevTopElement);
  if (!this->anyOpen(state)) state->setCacheOpen(FALSE);
}


/*!
  Sets the current cache. The cache is ref'ed before returning.
*/
void
SoCacheElement::set(SoState * const state, SoCache * const cache)
{
  SoCacheElement * elem = (SoCacheElement*)
    SoElement::getElement(state, classStackIndex);
  
  if (elem) {
    if (elem->cache) elem->cache->unref();
    elem->cache = cache;
    if (elem->cache) {
      elem->cache->ref();
      state->setCacheOpen(TRUE);
    }
  }
}

/*!
  This method returns the cache, or NULL if there is no cache.
*/

SoCache *
SoCacheElement::getCache(void) const
{
  return this->cache;
}

/*!
  This method returns TRUE if a cache is currently open.
*/

SbBool
SoCacheElement::anyOpen(SoState * const state)
{
  SoCacheElement * elem = (SoCacheElement*)
    state->getElementNoPush(classStackIndex);
  while (elem) {
    if (elem->cache) return TRUE;
    elem = (SoCacheElement*) elem->getNextInStack();
  }
  return FALSE;
}

/*!
  This method invalidates open caches. It should be called by
  uncacheable nodes.
*/
void
SoCacheElement::invalidate(SoState * const state)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoCacheElement::invalidate",
                         "Invalidate all caches");
#endif // debug

#ifdef HAVE_THREADS
  *(invalidated_storage->get()) = TRUE;
#else // HAVE_THREADS
  SoCacheElement::invalidated = TRUE;
#endif // !HAVE_THREADS

  SoCacheElement * elem = (SoCacheElement*)
    state->getElementNoPush(classStackIndex);
  while (elem) {
    if (elem->cache) {
      elem->cache->invalidate();
      elem->cache->unref();
      elem->cache = NULL;
    }
    elem = (SoCacheElement*) elem->getNextInStack();
  }
}

/*!
  SoCacheElement objects should not be compared because you obviously don't
  cache them in the cache.
*/

SbBool
SoCacheElement::matches(const SoElement * /* element */) const
{
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoCacheElement::matches",
                         "this method should not be called for this element");
#endif // debug
  return FALSE;
}

/*!
  SoCacheElement objects should not be "copied" because you obviously don't
  cache them in the cache.

  \sa SbBool SoCacheElement::matches(const SoElement * element) const
*/

SoElement *
SoCacheElement::copyMatchInfo(void) const
{
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoCacheElement::copyMatchInfo",
                         "this method should not be called for this element");
#endif // debug
  return NULL;
}

/*!
  This method returns the next cache element. That is the next cache
  element pointing towards the bottom of the state. 
*/
SoCacheElement *
SoCacheElement::getNextCacheElement(void) const
{
  return (SoCacheElement *) this->getNextInStack();
}

/*!
  This method adds \a element to the elements used lists of all the open
  caches in \a state.
*/

void
SoCacheElement::addElement(SoState * const state,
                           const SoElement * const element)
{
  SoCacheElement * elem = (SoCacheElement*)
    state->getElementNoPush(classStackIndex);
  while (elem) {
    if (elem->cache) elem->cache->addElement(element);
    elem = (SoCacheElement*) elem->getNextInStack();
  }
}

/*!
  This method creates dependencies on \a cache for all the open caches
  in \a state.
*/

void
SoCacheElement::addCacheDependency(SoState * const state,
                                   SoCache * const cache)
{
  SoCacheElement * elem = (SoCacheElement*)
    state->getElementNoPush(classStackIndex);
  while (elem) {
    if (elem->cache) elem->cache->addCacheDependency(state, cache);
    elem = (SoCacheElement*) elem->getNextInStack();
  }
}

/*!
  This method returns the old invalidated bit value, and sets it to
  \a newvalue.
*/

SbBool
SoCacheElement::setInvalid(const SbBool newvalue)
{
#ifdef HAVE_THREADS
  SbBool * ptr = invalidated_storage->get();
  SbBool oldval = *ptr;
  *ptr = newvalue;
#else // HAVE_THREADS
  SbBool oldval = SoCacheElement::invalidated;
  SoCacheElement::invalidated = newvalue;
#endif // ! HAVE_THREADS
  return oldval;
}

/*!
  This method returns the current cache.  No cache dependencies are honored.
*/

SoCache *
SoCacheElement::getCurrentCache(SoState * const state)
{
  return ((SoCacheElement *)(state->getElementNoPush(classStackIndex)))->cache;
}
