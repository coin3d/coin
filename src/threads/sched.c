/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/C/threads/sched.h>
#include <Inventor/C/threads/schedp.h>
#include <Inventor/C/threads/wpool.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/threads/thread.h>

#include <Inventor/C/errors/debugerror.h>
#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */
/* private methods */

static void sched_worker_entry_point(void * userdata);

typedef struct {
  void (*workfunc)(void *);
  void * closure;
  unsigned int priority;
} sched_item;

static int
sched_item_compare(void * o1, void * o2)
{
  sched_item * i1, * i2;
  
  i1 = (sched_item*) o1;
  i2 = (sched_item*) o2;
  
  return ((int) i1->priority) - ((int) i2->priority);
}

/* assumes mutex is locked */
static void
sched_try_trigger(cc_sched * sched)
{
  if (cc_wpool_try_begin(sched->pool, 1)) {
    cc_wpool_start_worker(sched->pool, sched_worker_entry_point, sched);
    cc_wpool_end(sched->pool);
  }
}

void
sched_worker_entry_point(void * userdata)
{
  sched_item * item;
  cc_sched * sched = (cc_sched*) userdata;

  cc_mutex_lock(sched->mutex);
  while (!cc_heap_empty(sched->itemheap)) {
    item = (sched_item*) cc_heap_extract_top(sched->itemheap);

    cc_mutex_unlock(sched->mutex);
    item->workfunc(item->closure);
    cc_mutex_lock(sched->mutex);
    cc_memalloc_deallocate(sched->itemalloc, (void*) item);
  }
  /* in case the main thread is in sched_wait_all() */
  cc_condvar_wake_one(sched->cond);
  cc_mutex_unlock(sched->mutex);
}

/* ********************************************************************** */
/* public api */


/*!
  Construct a scheduler that uses \a numthreads threads.
*/
cc_sched *
cc_sched_construct(int numthreads)
{
  cc_sched * sched = (cc_sched*) malloc(sizeof(cc_sched));
  assert(sched);
  sched->pool = cc_wpool_construct(numthreads);
  sched->mutex = cc_mutex_construct();
  sched->cond = cc_condvar_construct();
 
  sched->itemheap = cc_heap_construct(64, sched_item_compare, FALSE);
  sched->itemalloc = cc_memalloc_construct(sizeof(sched_item));

  return sched;
}

/*!
  Destruct the scheduler.
*/
void
cc_sched_destruct(cc_sched * sched)
{
  cc_sched_wait_all(sched);

  cc_heap_destruct(sched->itemheap);
  cc_memalloc_destruct(sched->itemalloc);

  cc_condvar_destruct(sched->cond);
  cc_mutex_destruct(sched->mutex);
  cc_wpool_destruct(sched->pool);
  free(sched);
}

/*!
  Set/change the number of threads used by the scheduler.
*/
void
cc_sched_set_num_threads(cc_sched * sched, int num)
{
  cc_sched_wait_all(sched);
  cc_wpool_set_num_workers(sched->pool, num);
}

/*!
  Returns the number of threads used by the scheduler.
*/
int
cc_sched_get_num_threads(cc_sched * sched)
{
  return cc_wpool_get_num_workers(sched->pool);
}

/*! 
  Schedule a new job. A thread calls \a workfunc with the \a closure
  argument when a thread becomes available. \a priority is currently
  ignored, but in the future this will be supported.
*/
void
cc_sched_schedule(cc_sched * sched,
                  void (*workfunc)(void *), void * closure,
                  unsigned int priority)
{
  sched_item * item;

  cc_mutex_lock(sched->mutex);
  item = (sched_item*) cc_memalloc_allocate(sched->itemalloc);
  
  item->workfunc = workfunc;
  item->closure = closure;
  item->priority = priority;

  cc_heap_add(sched->itemheap, (void*) item);
  sched_try_trigger(sched);

  cc_mutex_unlock(sched->mutex);
}

/*!
  Wait for all jobs to finish.
*/
void
cc_sched_wait_all(cc_sched * sched)
{
  cc_mutex_lock(sched->mutex);
  while (!cc_heap_empty(sched->itemheap)) {
    cc_condvar_wait(sched->cond, sched->mutex);
  }
  cc_mutex_unlock(sched->mutex);
  cc_wpool_wait_all(sched->pool);
}

/* ********************************************************************** */
