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

#include <Inventor/C/threads/wpool.h>
#include <Inventor/C/threads/wpoolp.h>
#include <Inventor/C/threads/worker.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include <Inventor/C/base/debug.h>
#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */
/* private methods */

static void 
wpool_lock(cc_wpool * pool)
{
  cc_mutex_lock(pool->mutex);
}

static void 
wpool_unlock(cc_wpool * pool)
{
  cc_mutex_unlock(pool->mutex);
}

static void 
wpool_idle_cb(cc_worker * worker, void * data)
{
  SbBool iswaiting;
  int idx;
  cc_wpool * pool = (cc_wpool*) data;
  
  wpool_lock(pool);
  
  idx = cc_list_find(pool->busypool, worker);
  assert(idx >= 0);
  if (idx >= 0) {
    cc_list_remove_fast(pool->busypool, idx);
    cc_list_append(pool->idlepool, worker);
  }
  
  iswaiting = pool->iswaiting;
  wpool_unlock(pool);
  
  if (iswaiting) {
    cc_condvar_wake_one(pool->waitcond);
  }  
}


static void
wpool_add_workers(cc_wpool * pool, int num)
{
  int i;
  cc_worker * worker;
  for (i = 0; i < num; i++) {
    worker = cc_worker_construct();
    cc_worker_set_idle_callback(worker, wpool_idle_cb, pool);
    cc_list_append(pool->idlepool, worker);
  }
}

static cc_worker * 
wpool_get_idle_worker(cc_wpool * pool)
{
  cc_worker * worker;
  /* assumes pool is locked (begin() has been called) */
  assert(cc_list_get_length(pool->idlepool));
  
  worker = (cc_worker*) cc_list_pop(pool->idlepool);
  cc_list_append(pool->busypool, worker);
  return worker;
}

/* ********************************************************************** */
/* public api */

cc_wpool * 
cc_wpool_construct(int numworkers)
{
  cc_wpool * pool = malloc(sizeof(cc_wpool));

  pool->mutex = cc_mutex_construct();
  pool->waitcond = cc_condvar_construct();
  pool->idlepool = cc_list_construct();
  pool->busypool = cc_list_construct();
  pool->iswaiting = FALSE;
  pool->numworkers = 0;

  cc_wpool_set_num_workers(pool, numworkers);
  return pool;
}

void 
cc_wpool_destruct(cc_wpool * pool)
{
  int i, n;
  cc_worker ** workers;
  cc_wpool_wait_all(pool);
  assert(cc_list_get_length(pool->busypool) == 0);
  n = cc_list_get_length(pool->idlepool);
  
  workers = (cc_worker**)
    cc_list_get_array(pool->idlepool);
  
  for (i = 0; i < n; i++) {
    cc_worker_destruct(workers[i]);
  }

  cc_list_destruct(pool->idlepool);
  cc_list_destruct(pool->busypool);
  cc_mutex_destruct(pool->mutex);
  cc_condvar_destruct(pool->waitcond);
  free(pool);
}

int 
cc_wpool_get_num_workers(cc_wpool * pool)
{
  return pool->numworkers;
}

void 
cc_wpool_set_num_workers(cc_wpool * pool, int newnum)
{
  if (newnum == pool->numworkers) return;
  
  cc_wpool_wait_all(pool);

  /* no need to call lock()/unlock(), since all threads 
   * are guaranteed to be idle */

  if (newnum > pool->numworkers) {
    wpool_add_workers(pool, newnum - pool->numworkers);
  }
  else {
    int i, n = pool->numworkers - newnum;
    for (i = 0; i < n; i++) {
      cc_worker_destruct((cc_worker*) cc_list_pop(pool->idlepool));
    }
  }
  pool->numworkers = newnum;
}

void 
cc_wpool_wait_all(cc_wpool * pool)
{
  wpool_lock(pool);
  pool->iswaiting = TRUE;
  while (cc_list_get_length(pool->busypool)) {
    /* wait() will atomically unlock the mutex, and wait
     * for signal. When signal arrived, the mutex will again be
     * atomically locked. */
    cc_condvar_wait(pool->waitcond, pool->mutex);
  }
  pool->iswaiting = FALSE;
  wpool_unlock(pool);
}

int 
cc_wpool_begin(cc_wpool * pool, int numworkersneeded)
{
  int n;
  wpool_lock(pool);
  
  n = cc_list_get_length(pool->idlepool);
  if (n < numworkersneeded) {
    wpool_unlock(pool);
    return 0;
  }
  return n;
}

void 
cc_wpool_start_worker(cc_wpool * pool, void (*workfunc)(void *), void * closure)
{
  cc_worker * worker = wpool_get_idle_worker(pool);
  assert(worker);
  if (worker) {
    cc_worker_start(worker, workfunc, closure);
  }
}


void 
cc_wpool_end(cc_wpool * pool)
{
  wpool_unlock(pool);
}

/* ********************************************************************** */
