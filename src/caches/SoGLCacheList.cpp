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
  \class SoGLCacheList include/Inventor/caches/SoGLCacheList.h
  \brief The SoGLCacheList class is used to store and manage OpenGL caches.
  \ingroup caches
*/

// SGI Inventor uses an LRU/MRU strategy or something here. We're not
// quite sure we should support multiple caches per SoSeparator
// though. After all, there is some overhead in cheching for valid
// caches etc. If a situation occurs where multiple caches would help
// the performance, the user should probably redesign the scene graph
// and enable caching further down the scene graph instead. We will
// store at least one cache per cache context to support rendering in
// multiple contexts though.

#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

// if we render once without any child nodes invalidating the cache,
// this flag is set and we will try to create a cache.
#define FLAG_SHOULD_TRY 0x1


#ifndef DOXYGEN_SKIP_THIS

class SoGLCacheListP {
public:
  SbList <SoGLRenderCache *> itemlist;
  int numcaches;
  unsigned int flags;
  SoGLRenderCache * opencache;
  SbBool savedinvalid;
  int autocachebits;
#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
  SoGLLazyElement::GLState prelazystate;
  SoGLLazyElement::GLState postlazystate;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

#ifdef COIN_THREADSAFE
#define GLCACHE_LOCK(_thisp_) (_thisp_)->pimpl->mutex.lock()
#define GLCACHE_UNLOCK(_thisp_) (_thisp_)->pimpl->mutex.unlock()
#else // COIN_THREADSAFE
#define GLCACHE_LOCK(_thisp_)
#define GLCACHE_UNLOCK(_thisp_)
#endif // ! COIN_THREADSAFE

/*!
  Constructor.
*/
SoGLCacheList::SoGLCacheList(int numcaches)
{
  // FIXME: currently the \a numcaches argument is not used. ???????? pederb.
  //
  // UPDATE 20020220 mortene: yikes! This means the
  // SoSeparator::setNumRenderCaches() call is not working properly,
  // for instance.

  THIS = new SoGLCacheListP;
  THIS->flags = 0;
  THIS->numcaches = numcaches;
  THIS->opencache = NULL;
  THIS->autocachebits = SoGLCacheContextElement::DO_AUTO_CACHE;
}

/*!
  Destructor. Frees remaining caches.
*/
SoGLCacheList::~SoGLCacheList()
{
  const int n = THIS->itemlist.getLength();
  for (int i = 0; i < n; i++) {
    THIS->itemlist[i]->unref();
  }
  delete THIS;
}

/*!
  Test for valid cache and execute. Returns TRUE if a valid cache
  could be found, FALSE otherwise. Note that when a valid cache is
  found, it is executed before returning from this method.
*/
SbBool
SoGLCacheList::call(SoGLRenderAction * action)
{
  int i;
  SoState * state = action->getState();
  int context = SoGLCacheContextElement::get(state);

  GLCACHE_LOCK(this);
  int n = THIS->itemlist.getLength();

  for (i = 0; i < n; i++) {
    SoGLRenderCache * cache = THIS->itemlist[i];
    if (cache->getCacheContext() == context) {
      if (cache->isValid(state) && 
          SoGLLazyElement::preCacheCall(state, &THIS->prelazystate)) {
        cache->ref();
        GLCACHE_UNLOCK(this); // allow other threads to access cache list
        cache->call(state);
        SoGLLazyElement::postCacheCall(state, &THIS->postlazystate);
        cache->unref(state);
        return TRUE;
      }
      // if we get here cache is invalid. Throw it away.
      cache->unref(state);
      THIS->itemlist.removeFast(i);
    }
  }

  GLCACHE_UNLOCK(this);
  // none found
  return FALSE;
}

/*!
  Start recording a new cache. Remember to call close() when you've
  finished recording the cache.

  \sa close()
*/
void
SoGLCacheList::open(SoGLRenderAction * action, SbBool autocache)
{
  GLCACHE_LOCK(this);

  assert(THIS->opencache == NULL);
  SoState * state = action->getState();

  // will be restored in close()
  THIS->savedinvalid = SoCacheElement::setInvalid(FALSE);

  if (SoCacheElement::anyOpen(state)) return;
  if (autocache && !(THIS->flags & FLAG_SHOULD_TRY)) return;

  SbBool shouldcreate = TRUE;
  if (autocache && THIS->autocachebits == SoGLCacheContextElement::DO_AUTO_CACHE) {
    shouldcreate = FALSE;
  }

  if (shouldcreate) {
    THIS->opencache = new SoGLRenderCache(state);
    THIS->opencache->ref();
    SoCacheElement::set(state, THIS->opencache);
    THIS->opencache->open(state);
    SoGLLazyElement::beginCaching(state, &THIS->prelazystate,
                                  &THIS->postlazystate);
  }
}

/*!
  Finish recording the currently open cache.
  \sa open()
 */
void
SoGLCacheList::close(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (THIS->opencache) {
    THIS->opencache->close();
    THIS->itemlist.append(THIS->opencache);
    THIS->opencache = NULL;
    SoGLLazyElement::endCaching(state);
  }
  if (SoCacheElement::setInvalid(THIS->savedinvalid)) {
    // notify parent caches
    SoCacheElement::setInvalid(TRUE);
    THIS->flags &= ~FLAG_SHOULD_TRY;
  }
  else {
    THIS->flags |= FLAG_SHOULD_TRY;
  }
  int bits = SoGLCacheContextElement::resetAutoCacheBits(state);
  SoGLCacheContextElement::setAutoCacheBits(state, bits|THIS->autocachebits);
  THIS->autocachebits = bits;
  
  GLCACHE_UNLOCK(this);
}

/*!
  Invalidate all caches in this instance. Should be called
  from the notify() method of nodes doing caching.
*/
void
SoGLCacheList::invalidateAll(void)
{
  int n = THIS->itemlist.getLength();
  for (int i = 0; i < n; i++) {
    THIS->itemlist[i]->invalidate();
  }
  THIS->flags &= ~FLAG_SHOULD_TRY;
}

#undef GLCACHE_READ_LOCK
#undef GLCACHE_READ_UNLOCK
#undef GLCACHE_WRITE_LOCK
#undef GLCACHE_WRITE_UNLOCK
