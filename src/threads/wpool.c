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

#include <Inventor/C/threads/wpool.h>
#include <Inventor/C/threads/wpoolp.h>
#include <Inventor/C/threads/worker.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include <Inventor/C/errors/debugerror.h>
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
wpool_wait(cc_wpool * pool, int num)
{
  pool->iswaiting = TRUE;
  while (cc_list_get_length(pool->idlepool) < num) {
    /* wait() will atomically unlock the mutex, and wait
     * for signal. When signal arrived, the mutex will again be
     * atomically locked. */
    cc_condvar_wait(pool->waitcond, pool->mutex);
  }
  pool->iswaiting = FALSE;
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

/*!
  Construct worker pool.
*/
cc_wpool *
cc_wpool_construct(int numworkers)
{
  cc_wpool * pool = (cc_wpool*) malloc(sizeof(cc_wpool));

  pool->mutex = cc_mutex_construct();
  pool->waitcond = cc_condvar_construct();
  pool->idlepool = cc_list_construct();
  pool->busypool = cc_list_construct();
  pool->iswaiting = FALSE;
  pool->numworkers = 0;

  cc_wpool_set_num_workers(pool, numworkers);
  return pool;
}

/*!
  Destruct worker pool.
*/
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

/*!
  Returns the number of workers in the pool.
*/
int
cc_wpool_get_num_workers(cc_wpool * pool)
{
  return pool->numworkers;
}

/*!
  Sets the number of workers in the pool.
*/
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

/*!
  Wait for all pool workers to finish working and go into idle state.
  This method should only be called by the thread controlling the pool.
  A pool worker should not call this method, since it will obviously
  never return from here (it will never go idle).
*/
void
cc_wpool_wait_all(cc_wpool * pool)
{
  wpool_lock(pool);
  wpool_wait(pool, pool->numworkers); /* wait for all workers to become idle */
  wpool_unlock(pool);
}

/*!

  Locks the pool so that workers can be started using the
  cc_wpool_start_worker() method. \a numworkersneeded should contain
  the minumum number of workers that is needed. If \numworkersneeded
  workers are available, the pool will be locked and TRUE is returned.
  Otherwise FALSE is returned.

  Usage pseudocode:

  \code

  int numworkers = 5;

  if (cc_wpool_begin(pool, numworkers)) {
    for (int i = 0; i < numworkers; i++) {
      cc_wpool_start_worker(my_work[i], my_closure[i]);
    }
    cc_wpool_end(pool);
  }

  \endcode

  Important! If too few workers are available, the pool will not be
  locked and cc_wpool_end() should not be called.

  \sa cc_wpool_start_worker(), cc_wpool_end()
*/
SbBool
cc_wpool_try_begin(cc_wpool * pool, int numworkersneeded)
{
  int n;
  wpool_lock(pool);

  n = cc_list_get_length(pool->idlepool);
  if (n < numworkersneeded) {
    wpool_unlock(pool);
    return FALSE;
  }
  return TRUE;
}

/*!
  Wait for \a numworkersneeded workers to become idle. When returning
  from this call, the pool will be locked, and up to \a numworkersneeded
  can be started using the cc_wpool_start_worker() method. Remember
  to call cc_wpool_end() to unlock the pool again.

  \sa cc_wpool_try_begin()
*/
void
cc_wpool_begin(cc_wpool * pool, int numworkersneeded)
{
  int n;
  wpool_lock(pool);
  n = cc_list_get_length(pool->idlepool);
  if (n < numworkersneeded) {
    wpool_wait(pool, numworkersneeded);
  }
}

/*!

  Starts a worker. The pool must be locked (using cc_wpool_begin())
  before calling this method.

  \sa cc_wpool_begin() , cc_wpool_end()
*/
void
cc_wpool_start_worker(cc_wpool * pool, void (*workfunc)(void *), void * closure)
{
  cc_worker * worker = wpool_get_idle_worker(pool);
  assert(worker);
  if (worker) {
    cc_worker_start(worker, workfunc, closure);
  }
}

/*!

  Unlocks the pool after a cc_wpool_begin(), cc_wpool_start_worker()
  sequence.

  Please note that if cc_wpool_begin() returns 0, you should not call
  cc_wpool_end().

  \sa cc_wpool_begin(), cc_wpool_start_worker()

*/
void
cc_wpool_end(cc_wpool * pool)
{
  wpool_unlock(pool);
}

/* ********************************************************************** */
