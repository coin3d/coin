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

#include <Inventor/C/threads/worker.h>
#include <Inventor/C/threads/workerp.h>
#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include <Inventor/C/errors/debugerror.h>
#include <stdlib.h>
#include <assert.h>

/*
 * this is the thread's main loop.
 */
static void 
worker_thread_loop(cc_worker * worker)
{
  cc_mutex_lock(worker->mutex);
  /* signal master that we've got into the scheduling loop */
  cc_condvar_wake_one(worker->begincond);

  while (!worker->shutdown) {
    /* wait for job */
    cc_condvar_wait(worker->cond, worker->mutex);
    /* check if there are more jobs */
    if (!worker->shutdown) {
      /* do the scheduled job */
      worker->workfunc(worker->workclosure);
      /* call the idle cb */
      if (worker->idlecb) {
        worker->idlecb(worker, worker->idleclosure);
      }
    }
  }

  worker->workfunc = NULL;
  /* remember to unlock mutex after we break out of the loop */
  cc_mutex_unlock(worker->mutex);
}

/*
 * entry point for thread. It just jumps to the main loop
 */
static void * 
worker_thread_entry(void * data)
{
  cc_worker * worker = (cc_worker*) data;
  /* FIXME, FIXME, FIXME!!!!!  John Ivar Haugland reported problems
   * with the worker code after updating from IRIX 6.5.15 to
   * 6.5.18. What happened was that the new thread somehow managed to
   * send the begincond signal (in worker_thread_loop()) before the
   * main thread got around to waiting for it. This should be
   * impossible, since the main thread locks a mutex before starting
   * the new thread (in worker_start_thread(), and the new thread also
   * locks this mutex before sending the begincond signal. Very
   * strange. A beer to the person who finds the flaw in my logic
   * here.  pederb, 2002-12-18 */

  /* This sleep is only a workaround for the bug described above */
  cc_sleep(2.0f); 
  worker_thread_loop(worker);
  return NULL;
}

/*
 * called to start thread. Assumes worker->mutex is locked by caller */
static void 
worker_start_thread(cc_worker * worker)
{
  if (!worker->threadisrunning) {
    worker->thread = cc_thread_construct(worker_thread_entry, worker);
    
    /* wait for thread to get to the main loop */
    cc_condvar_wait(worker->begincond, worker->mutex);
    worker->threadisrunning = 1;
  }
}

/*
 * join caller thread with this thread.
 */
static void 
worker_stop_thread(cc_worker * worker)
{
  if (worker->threadisrunning) {
    cc_mutex_lock(worker->mutex);
    worker->threadisrunning = 0;
    
    worker->shutdown = TRUE;  /* signal thread to exit loop */
    cc_mutex_unlock(worker->mutex);
    /* in case thread is waiting for cond... */
    cc_condvar_wake_one(worker->cond);
    /* wait for thread to finish */
    cc_thread_join(worker->thread, NULL);
    cc_thread_destruct(worker->thread);
    worker->thread = NULL;
    worker->shutdown = FALSE; /* reset signal */
  }
}
  

cc_worker * 
cc_worker_construct(void)
{
  cc_worker * worker = (cc_worker*) malloc(sizeof(cc_worker));
  assert(worker);

  worker->mutex = cc_mutex_construct();
  worker->cond = cc_condvar_construct();
  worker->begincond = cc_condvar_construct();
  worker->thread = NULL; /* delay creating thread */
  worker->threadisrunning = FALSE;
  worker->shutdown = FALSE;
  worker->workfunc = NULL;
  worker->workclosure = NULL;
  worker->idlecb = NULL;
  worker->idleclosure = NULL;

  return worker;
}

void 
cc_worker_destruct(cc_worker * worker)
{
  if (worker->threadisrunning) {
    worker_stop_thread(worker);
  }
  assert(worker->thread == NULL);
  cc_mutex_destruct(worker->mutex);
  cc_condvar_destruct(worker->cond);
  cc_condvar_destruct(worker->begincond);
  free(worker);
}

SbBool 
cc_worker_start(cc_worker * worker, void (*workfunc)(void *), void * closure)
{
  assert(workfunc);

  cc_mutex_lock(worker->mutex);
  
  /* no need to lock, thread is idle or not running */
  worker->workfunc = workfunc;
  worker->workclosure = closure;

  if (!worker->threadisrunning) {
    worker_start_thread(worker);
  }
  
  /* We now know that thread is waiting for a signal */
  cc_condvar_wake_one(worker->cond);
  cc_mutex_unlock(worker->mutex);
  return TRUE;
}

SbBool 
cc_worker_is_busy(cc_worker * worker)
{
  SbBool busy = TRUE;
  
  if (cc_mutex_try_lock(worker->mutex)) {
    busy = FALSE;
    cc_mutex_unlock(worker->mutex);
  }
  return busy;
}

void 
cc_worker_wait(cc_worker * worker)
{
  /* synchronize by stopping thread. A new thread must be created
   * if more work needs to be done, but this is ok, I guess.
   */
  worker_stop_thread(worker);
}

void 
cc_worker_set_idle_callback(cc_worker * worker, 
                            void (*cb)(cc_worker *, void *), void * closure)
{
  cc_mutex_lock(worker->mutex);
  worker->idlecb = cb;
  worker->idleclosure = closure;
  cc_mutex_unlock(worker->mutex);
}

