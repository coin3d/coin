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

//$ BEGIN TEMPLATE ElementSource(SoCacheElement)

/*!
  \var SoCacheElement::classTypeId

  This is the static class type identifier for the
  SoCacheElement class.
*/

SoType SoCacheElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoCacheElement::getClassTypeId(void)
{
  return SoCacheElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoCacheElement class.
*/
void *
SoCacheElement::createInstance(void)
{
  return (void *) new SoCacheElement;
}

/*!
  \var SoCacheElement::classStackIndex

  This is the static state stack index for the
  SoCacheElement class.
*/
int SoCacheElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoCacheElement class.
*/
int
SoCacheElement::getClassStackIndex(void)
{
  return SoCacheElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoCacheElement class.
*/

void
SoCacheElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoCacheElement)
  assert(SoCacheElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCacheElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoCacheElement",
                       &SoCacheElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoCacheElement::classStackIndex =
      createStackIndex(SoCacheElement::classTypeId);
  }
  else {
    SoCacheElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
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
    assert(0 && "FIXME: not implemented");
}

//! FIXME: write doc.

void
SoCacheElement::set(SoState * const /* state */, SoCache * const /* cache */)
{
  assert(0 && "FIXME: not implemented");
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
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  This method invalidate open caches.  It should be called by uncacheable nodes.
*/

void
SoCacheElement::invalidate(SoState * const /* state */)
{
    assert(0 && "FIXME: not implemented");
}

/*!
  SoCacheElement objects should not be compared because you obviously don't
  cache them in the cache.
*/

SbBool
SoCacheElement::matches(const SoElement * /* element */) const
{
    assert(0 && "FIXME: not implemented"); // print error message
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
    assert(0 && "FIXME: not implemented"); // print error message
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
    assert(0 && "FIXME: not implemented");
}

/*!
  This method creates dependencies on \a cache for all the open caches
  in \a state.
*/

void
SoCacheElement::addCacheDependency(SoState * const /* state */,
				   SoCache * const /* cache */)
{
    assert(0 && "FIXME: not implemented");
}

/*!
  This method returns the old invalidated bit value, and sets it to
  \a newValue.
*/

SbBool
SoCacheElement::setInvalid(const SbBool /* newValue */)
{
    assert(0 && "FIXME: not implemented");
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

