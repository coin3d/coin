/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/threads/sync.h>
#include <Inventor/C/threads/syncp.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/base/hash.h>
#include <stddef.h>
#include "../tidbits.h" // coin_atexit

static cc_mutex * sync_global_mutex = NULL;
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
  cc_mutex_destruct(sync_global_mutex);
}

/*
  Initialize synchronizer. Should only be called once, from SoDB::init()
*/
void 
cc_sync_init(void)
{
  sync_global_mutex = cc_mutex_construct();
  sync_hash_table = cc_hash_construct(256, 0.75f);

  coin_atexit((coin_atexit_f*) sync_cleanup);
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

  cc_mutex_lock(sync_global_mutex);

  if (!cc_hash_get(sync_hash_table, (unsigned long) id, &mutex)) {
    mutex = (void*) cc_mutex_construct();
    (void) cc_hash_put(sync_hash_table, (unsigned long) id, mutex);
  }
  
  cc_mutex_unlock(sync_global_mutex);
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


