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
  \class SoCacheElement Inventor/elements/SoCacheElement.h
  \brief The SoCacheElement class stores and manages the opene caches.
*/

#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/caches/SoCache.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SbBool SoCacheElement::invalidated = FALSE;

SO_ELEMENT_SOURCE(SoCacheElement);

/*!
  This static method initializes static data for the SoCacheElement class.
*/

void
SoCacheElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoCacheElement, inherited);
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

/*!
  Overloaded to unref the cache, since the cache is ref'ed in set(). 
*/
void
SoCacheElement::pop(SoState * state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  if (this->cache) this->cache->unref();
}


/*!
  Sets the current cache. The cache is ref'ed before returning.
*/
void
SoCacheElement::set(SoState * const state, SoCache * const cache)
{
  SoCacheElement * elem = (SoCacheElement*)
    SoElement::getElement(state, classStackIndex);
  elem->cache = cache;
  if (elem->cache) elem->cache->ref();
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
  return SoCacheElement::getCurrentCache(state) != NULL;
}

/*!
  This method invalidates open caches.  It should be called by uncacheable nodes.
*/
void
SoCacheElement::invalidate(SoState * const state)
{
  SoCacheElement::invalidated = TRUE;
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
  element pointing towards the bottom of the state. In Coin we call
  this the previous element.
*/
SoCacheElement *
SoCacheElement::getNextCacheElement(void) const
{
  return (SoCacheElement *) this->prev;
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
    elem = (SoCacheElement*) elem->prev;
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
    elem = (SoCacheElement*) elem->prev;
  }
}

/*!
  This method returns the old invalidated bit value, and sets it to
  \a newvalue.
*/

SbBool
SoCacheElement::setInvalid(const SbBool newvalue)
{
  SbBool oldval = SoCacheElement::invalidated;
  SoCacheElement::invalidated = newvalue;
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
