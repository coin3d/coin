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

#include <Inventor/C/threads/sched.h>
#include <Inventor/C/threads/schedp.h>
#include <Inventor/C/threads/wpool.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/threads/thread.h>

#include <Inventor/C/base/debug.h>
#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */
/* private methods */

static void sched_worker_entry_point(void * userdata);

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
  void (*func)(void *);
  void * closure;

  cc_sched * sched = (cc_sched*) userdata;
  
  func = NULL;
  closure = NULL;
  cc_mutex_lock(sched->mutex);
  while (cc_list_get_length(sched->funclist)) {
    func = (void (*)(void*)) cc_list_pop(sched->funclist);
    closure = cc_list_pop(sched->closurelist);

    cc_mutex_unlock(sched->mutex);
    func(closure);
    cc_mutex_lock(sched->mutex);
  }
  cc_mutex_unlock(sched->mutex);

  /* in case the main thread is in sched_wait_all() */
  cc_condvar_wake_one(sched->cond);
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
  
  sched->funclist = cc_list_construct();
  sched->closurelist = cc_list_construct();
  sched->prilist = cc_list_construct();

  return sched;
}

/*!
  Destruct the scheduler.
*/
void 
cc_sched_destruct(cc_sched * sched)
{
  cc_sched_wait_all(sched);
  cc_list_destruct(sched->prilist);
  cc_list_destruct(sched->closurelist);
  cc_list_destruct(sched->funclist);

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
  Schedule a not job. \a priority is currently ignored, but
  in the future this will be supported.
*/
void 
cc_sched_schedule(cc_sched * sched, 
                  void (*workfunc)(void *), void * closure,
                  unsigned int priority)
{
  cc_mutex_lock(sched->mutex);
  cc_list_append(sched->funclist, (void*) workfunc);
  cc_list_append(sched->closurelist, closure);

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
  while (cc_list_get_length(sched->funclist)) {
    cc_condvar_wait(sched->cond, sched->mutex);
  }
  cc_mutex_unlock(sched->mutex);
  cc_wpool_wait_all(sched->pool);
}

/* ********************************************************************** */

