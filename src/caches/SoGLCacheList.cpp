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


static int COIN_AUTO_CACHING = -1;

#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <../tidbits.h> // coin_getenv()

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#ifndef DOXYGEN_SKIP_THIS

class SoGLCacheListP {
public:
  SbList <SoGLRenderCache *> itemlist;
  int numcaches;
  SoGLRenderCache * opencache;
  SbBool savedinvalid;
  int autocachebits;
  int numused;
  int numinvalidated;
  SbBool needclose;
  SoElement * invalidelement;
  int numframesok;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoGLCacheList::SoGLCacheList(int numcaches)
{
  THIS = new SoGLCacheListP;
  THIS->numcaches = numcaches;
  THIS->opencache = NULL;
  THIS->autocachebits = 0;
  THIS->numused = 0;
  THIS->numinvalidated = 0;
  THIS->needclose = FALSE;
  THIS->invalidelement = NULL;
  THIS->numframesok = 0;

  // auto caching must be enabled using an environment variable
  if (COIN_AUTO_CACHING < 0) {
    const char * env = coin_getenv("COIN_AUTO_CACHING");
    if (env) COIN_AUTO_CACHING = atoi(env);
    else COIN_AUTO_CACHING = 0;
  }
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
  // do a quick return if there are no caches in the list
  int n = THIS->itemlist.getLength();
  if (n == 0) return FALSE;

  int i;
  SoState * state = action->getState();
  int context = SoGLCacheContextElement::get(state);


  for (i = 0; i < n; i++) {
    SoGLRenderCache * cache = THIS->itemlist[i];
    if (cache->getCacheContext() == context) {
      if (cache->isValid(state) && 
          SoGLLazyElement::preCacheCall(state, cache->getPreLazyState())) {
        cache->ref();
        // move cache to the end of the list. The MRU cache will be at
        // the end of the list, and the LRU will be the first
        // item. This makes it easy to choose a cache to destroy when
        // the maximum number of caches is exceeded.
        THIS->itemlist.remove(i);
        THIS->itemlist.append(cache);
        // update lazy GL state before calling cache
        SoGLLazyElement::getInstance(state)->send(state, SoLazyElement::ALL_MASK);
        cache->call(state);
        SoGLLazyElement::postCacheCall(state, cache->getPostLazyState());
        cache->unref(state);
        THIS->numused++;
        return TRUE;
      }
    }
  }
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
  // needclose is used to quickly return in close()
  if (THIS->numcaches == 0 || (autocache && COIN_AUTO_CACHING == 0)) {
    THIS->needclose = FALSE;
    return;
  }
  THIS->needclose = TRUE;

  assert(THIS->opencache == NULL);
  SoState * state = action->getState();

  // will be restored in close()
  THIS->savedinvalid = SoCacheElement::setInvalid(FALSE);

  if (SoCacheElement::anyOpen(state)) return;

  SbBool shouldcreate = FALSE;
  if (!autocache) {
    if (THIS->numframesok >= 1) shouldcreate = TRUE;
  }
  else {
    if (THIS->numframesok >= 2 && 
        (THIS->autocachebits == SoGLCacheContextElement::DO_AUTO_CACHE)) {
      shouldcreate = TRUE;
    }
  }

  if (shouldcreate) {
    if (THIS->itemlist.getLength() >= THIS->numcaches) {
      // the cache at position 0 will be the LRU cache. Remove it.
      SoGLRenderCache * cache = THIS->itemlist[0];
      cache->unref(state);
      THIS->itemlist.remove(0);
    }
    THIS->opencache = new SoGLRenderCache(state);
    THIS->opencache->ref();
    SoCacheElement::set(state, THIS->opencache);
    SoGLLazyElement::beginCaching(state, THIS->opencache->getPreLazyState(),
                                  THIS->opencache->getPostLazyState());
    THIS->opencache->open(state);
  }
  THIS->autocachebits = SoGLCacheContextElement::resetAutoCacheBits(state);
}

/*!
  Finish recording the currently open cache.
  \sa open()
 */
void
SoGLCacheList::close(SoGLRenderAction * action)
{
  if (!THIS->needclose) return;

  SoState * state = action->getState();

  // close open cache before accepting it or throwing it away
  if (THIS->opencache) {
    THIS->opencache->close();
    SoGLLazyElement::endCaching(state);            
  }
  if (SoCacheElement::setInvalid(THIS->savedinvalid)) {
    // notify parent caches
    SoCacheElement::setInvalid(TRUE);
    THIS->numframesok = 0;
    // just throw away the open cache, it's invalid
    if (THIS->opencache) {
      THIS->opencache->unref();
      THIS->opencache = NULL;
    }
  }
  else {
    THIS->numframesok++;
  }

  // open cache is ok, add it to the cache list
  if (THIS->opencache) {
    THIS->itemlist.append(THIS->opencache);
    THIS->opencache = NULL;
  }

  int bits = SoGLCacheContextElement::resetAutoCacheBits(state);
  SoGLCacheContextElement::setAutoCacheBits(state, bits|THIS->autocachebits);
  THIS->autocachebits = bits;
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
  THIS->numframesok = 0;
}
