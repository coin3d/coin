/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoGLRenderCache include/Inventor/caches/SoGLRenderCache.h
  \brief The SoGLRenderCache class is used to cache OpenGL calls.
  \ingroup caches
*/

#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/lists/SbList.h>
#include <assert.h>

#ifndef DOXYGEN_SKIP_THIS
class SoGLRenderCacheP {
public:
  SoGLDisplayList * displaylist;
  SoState * openstate;
  SbList <SoGLDisplayList*> nestedcachelist;
  SoGLLazyElement::GLState prestate;
  SoGLLazyElement::GLState poststate;
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor with \a state being the current state.
*/
SoGLRenderCache::SoGLRenderCache(SoState * state)
  : SoCache(state)
{
  THIS = new SoGLRenderCacheP;
  THIS->displaylist = NULL;
  THIS->openstate = NULL;
}

/*!
  Destructor.
*/
SoGLRenderCache::~SoGLRenderCache()
{
  // stuff should have been deleted in destroy()
  assert(THIS->displaylist == NULL);
  assert(THIS->nestedcachelist.getLength() == 0);
  
  delete THIS;
}

/*!
  Opens the cache. All GL calls will be cached until close() is called.

  \sa close()
  \sa call()
*/
void
SoGLRenderCache::open(SoState * state)
{
  assert(THIS->displaylist == NULL);
  assert(THIS->openstate == NULL); // cache should not be open
  THIS->openstate = state;
  THIS->displaylist =
    new SoGLDisplayList(state, SoGLDisplayList::DISPLAY_LIST);
  THIS->displaylist->ref();
  THIS->displaylist->open(state);
}

/*!
  Closes the cache. Should be called after all GL calls.

  \sa open()
*/
void
SoGLRenderCache::close(void)
{
  assert(THIS->openstate != NULL);
  assert(THIS->displaylist != NULL);
  THIS->displaylist->close(THIS->openstate);
  THIS->openstate = NULL;
}

/*!
  Executes the cached display list.

  \sa open()
*/
void
SoGLRenderCache::call(SoState * state)
{
  assert(THIS->displaylist != NULL);
  THIS->displaylist->call(state);
}

/*!
  Returns the cache context of this cache. This is used to quickly
  determine if cache can be used for a state.

  This method is an extension versus the Open Inventor API.
*/
int
SoGLRenderCache::getCacheContext(void) const
{
  if (THIS->displaylist) return THIS->displaylist->getContext();
  return -1;
}

// Documented in superclass. Overridden to test and update lazy GL
// elements.
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
  THIS->nestedcachelist.append(child);
}

// Documented in superclass. Overridden to unref display lists.
void
SoGLRenderCache::destroy(SoState * state)
{
  int n = THIS->nestedcachelist.getLength();
  for (int i = 0; i < n; i++) {
    THIS->nestedcachelist[i]->unref(state);
  }
  THIS->nestedcachelist.truncate(0);
  if (THIS->displaylist) {
    THIS->displaylist->unref(state);
    THIS->displaylist = NULL;
  }
}

SoGLLazyElement::GLState * 
SoGLRenderCache::getPreLazyState(void)
{
  return &THIS->prestate;
}

SoGLLazyElement::GLState * 
SoGLRenderCache::getPostLazyState(void)
{
  return &THIS->poststate;
}

