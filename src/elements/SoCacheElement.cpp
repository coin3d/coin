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
  \class SoCacheElement Inventor/elements/SoCacheElement.h
  \brief The SoCacheElement class stores the last opened cache.

  This is not in use yet.
*/

#include <Inventor/SbName.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoCacheElement.h>
#include <assert.h>

// static variables
SbBool SoCacheElement::invalidated;

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
  A constructor.  Can't be used directly.

  \sa void * SoCacheElement::createInstance()
*/

SoCacheElement::SoCacheElement(void)
  : cache(NULL)
{
    setTypeId(SoCacheElement::classTypeId);
    setStackIndex(SoCacheElement::classStackIndex);
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
}

//! FIXME: write doc.

void
SoCacheElement::push(SoState * state)
{
    inherited::push(state);
}

/*!
  Unrefs the cache.
*/

void
SoCacheElement::pop(SoState * state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  COIN_STUB();
}

//! FIXME: write doc.

void
SoCacheElement::set(SoState * const /* state */, SoCache * const /* cache */)
{
  COIN_STUB();
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
SoCacheElement::anyOpen(SoState * const /* state */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  This method invalidate open caches.  It should be called by uncacheable nodes.
*/

void
SoCacheElement::invalidate(SoState * const /* state */)
{
  COIN_STUB();
}

/*!
  SoCacheElement objects should not be compared because you obviously don't
  cache them in the cache.
*/

SbBool
SoCacheElement::matches(const SoElement * /* element */) const
{
  COIN_STUB();
  // FIXME: debug check + error message here. 19991213 mortene.
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
  COIN_STUB();
  // FIXME: debug check + error message here. 19991213 mortene.
  return NULL;
}

/*!
  This method returns the next cache element.
*/

SoCacheElement *
SoCacheElement::getNextCacheElement(void) const
{
    return (SoCacheElement *) this->next;
}

/*!
  This method adds \a element to the elements used lists of all the open
  caches in \a state.
*/

void
SoCacheElement::addElement(SoState * const /* state */,
                           const SoElement * const /* element */)
{
  COIN_STUB();
}

/*!
  This method creates dependencies on \a cache for all the open caches
  in \a state.
*/

void
SoCacheElement::addCacheDependency(SoState * const /* state */,
                                   SoCache * const /* cache */)
{
  COIN_STUB();
}

/*!
  This method returns the old invalidated bit value, and sets it to
  \a newValue.
*/

SbBool
SoCacheElement::setInvalid(const SbBool /* newValue */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  This method returns the current cache.  No cache dependencies are honored.
*/

SoCache *
SoCacheElement::getCurrentCache(SoState * const state)
{
  return ((SoCacheElement *)(state->getElementNoPush(classStackIndex)))->cache;
}
