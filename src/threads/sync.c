/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/C/threads/sync.h>
#include <Inventor/C/threads/syncp.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>
#include <Inventor/C/base/hash.h>
#include <stddef.h>
#include <Inventor/C/tidbitsp.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static cc_hash * sync_hash_table = NULL;

static void
sync_hash_cb(unsigned long key, void * val, void * closure)
{
  cc_mutex_destruct((cc_mutex*) val);
}

static void
sync_cleanup(void)
{
  cc_hash_apply(sync_hash_table, sync_hash_cb, NULL);
  cc_hash_destruct(sync_hash_table);
}

/*
  Initialize synchronizer. Should only be called once, from SoDB::init()
*/
void
cc_sync_init(void)
{
  if (sync_hash_table == NULL) {
    /* use -100 as atexit priority to make this callback trigger after
       normal cleanup function which might still use a cc_sync
       instance */
    coin_atexit((coin_atexit_f*) sync_cleanup, -100);
    sync_hash_table = cc_hash_construct(256, 0.75f);    
  }
}

/*!

  Begin synchronizing \a id. After returning from this call, other
  threads trying to synchronize \a id will be blocked until the first
  thread calls cc_sync_end() with the key returned from this function.

*/
void *
cc_sync_begin(void * id)
{
  void * mutex;

  cc_mutex_global_lock();
  if (sync_hash_table == NULL) {
    cc_sync_init();
  }
  if (!cc_hash_get(sync_hash_table, (unsigned long) id, &mutex)) {
    mutex = (void*) cc_mutex_construct();
    (void) cc_hash_put(sync_hash_table, (unsigned long) id, mutex);
  }

  cc_mutex_global_unlock();
  cc_mutex_lock((cc_mutex*) mutex);
  
  return mutex;
}

/*!
  End synchronize. \a key should be the key returned from cc_sync_begin().
*/
void
cc_sync_end(void * key)
{
  cc_mutex_unlock((cc_mutex*) key);
}

/*!
  Free sync item. This can be called from the destructor of objects
  that use a sync item so that the sync-mutex is freed when the object
  is destructed.

  \since Coin 2.3
*/
void 
cc_sync_free(void * id)
{
  void * mutex;

  cc_mutex_global_lock();
  if (sync_hash_table == NULL) {
    cc_sync_init();
  }
  if (cc_hash_get(sync_hash_table, (unsigned long) id, &mutex)) {
    cc_mutex_destruct((cc_mutex*) mutex);
    cc_hash_remove(sync_hash_table, (unsigned long) id);
  }
  cc_mutex_global_unlock();
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
