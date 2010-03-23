/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/C/threads/sync.h>
#include "coindefs.h"

#include <stddef.h>

#include <Inventor/C/threads/mutex.h>

#include "tidbitsp.h"
#include "base/dict.h"
#include "threads/syncp.h"
#include "threads/mutexp.h"

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static cc_dict * sync_hash_table = NULL;

static void
sync_hash_cb(uintptr_t COIN_UNUSED_ARG(key), void * val, void * COIN_UNUSED_ARG(closure))
{
  cc_mutex_destruct((cc_mutex*) val);
}

static void
sync_cleanup(void)
{
  cc_dict_apply(sync_hash_table, sync_hash_cb, NULL);
  cc_dict_destruct(sync_hash_table);
  sync_hash_table = NULL;
}

/*
  Initialize synchronizer. Should only be called once, from SoDB::init()
*/
void
cc_sync_init(void)
{
  if (sync_hash_table == NULL) {
    /* the priority is set so to make this callback trigger late,
       after normal cleanup function which might still use a cc_sync
       instance */
    coin_atexit((coin_atexit_f*) sync_cleanup, CC_ATEXIT_THREADING_SUBSYSTEM);
    sync_hash_table = cc_dict_construct(256, 0.75f);    
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
  if (!cc_dict_get(sync_hash_table, (uintptr_t)id, &mutex)) {
    mutex = (void*) cc_mutex_construct();
    (void) cc_dict_put(sync_hash_table, (uintptr_t)id, mutex);
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
  if (cc_dict_get(sync_hash_table, (uintptr_t)id, &mutex)) {
    cc_mutex_destruct((cc_mutex*) mutex);
    cc_dict_remove(sync_hash_table, (uintptr_t)id);
  }
  cc_mutex_global_unlock();
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
