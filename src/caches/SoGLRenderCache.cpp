/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoGLRenderCache include/Inventor/caches/SoGLRenderCache.h
  \brief The SoGLRenderCache class is used to cache GL calls.

*/

#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <assert.h>

/*!
  Constructor with \a state being the current state.
*/
SoGLRenderCache::SoGLRenderCache(SoState * state)
  : SoCache(state),
    displaylist(NULL),
    openstate(NULL)
{
}

/*!
  Destructor.
*/
SoGLRenderCache::~SoGLRenderCache()
{
  // stuff should have been deleted in destroy()
  assert(this->displaylist == NULL);
  assert(this->nestedcachelist.getLength() == 0);
}

/*!
  Opens the cache. All GL calls will be cached until close() is called.

  \sa close()
  \sa call()
*/
void
SoGLRenderCache::open(SoState * state)
{
  assert(this->displaylist == NULL);
  assert(this->openstate == NULL); // cache should not be open
  this->openstate = state;
  this->displaylist =
    new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
  this->displaylist->ref();
  this->displaylist->open(state);
}

/*!
  Closes the cache. Should be called after all GL calls.

  \sa open()
*/
void
SoGLRenderCache::close(void)
{
  assert(this->openstate != NULL);
  assert(this->displaylist != NULL);
  this->displaylist->close(this->openstate);
  this->openstate = NULL;
}

/*!
  Executes the cached display list.

  \sa open()
*/
void
SoGLRenderCache::call(SoState * state)
{
  assert(this->displaylist != NULL);
  this->displaylist->call(state);
}

/*!
  Returns the cache context of this cache. This is used to quickly
  determine if cache can be used for a state.

  This method is an extension versus the Open Inventor API.
*/
int
SoGLRenderCache::getCacheContext(void) const
{
  if (this->displaylist) return this->displaylist->getContext();
  return -1;
}

/*!
  Overloaded to test and update lazy GL elements.
*/
SbBool
SoGLRenderCache::isValid(const SoState * state) const
{
  // FIXME: pederb, 20001005 we should do some testing on the lazy
  // elements here.  If the state of an lazy element has changed, we
  // must either return FALSE here, or update the lazy element(s) to
  // the correct state before calling the list.
  return inherited::isValid(state);
}

/*!
  Adds a display list which is called from this cache. This is to
  make sure the child display list isn't destroyed too early.
*/
void
SoGLRenderCache::addNestedCache(SoGLDisplayList * child)
{
  child->ref();
  this->nestedcachelist.append(child);
}

/*!
  Overloaded to unref display lists.
*/
void
SoGLRenderCache::destroy(SoState * state)
{
  int n = this->nestedcachelist.getLength();
  for (int i = 0; i < n; i++) {
    this->nestedcachelist[i]->unref(state);
  }
  this->nestedcachelist.truncate(0);
  if (this->displaylist) {
    this->displaylist->unref(state);
    this->displaylist = NULL;
  }
}

