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

#include <Inventor/C/threads/barrier.h>
#include <Inventor/C/threads/barrierp.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* ********************************************************************** */

/*
  \internal
*/

void
cc_barrier_struct_init(cc_barrier * barrier_struct, unsigned int count)
{
  assert(barrier_struct != NULL && barrier_struct->type == CC_INVALID_TYPE);

  barrier_struct->threshold = count;
  barrier_struct->counter = count;
  barrier_struct->cycle = 0;
  barrier_struct->valid = CC_BARRIER_VALID;

  barrier_struct->mutex.type = CC_INVALID_TYPE;
  (void) cc_mutex_struct_init(&barrier_struct->mutex);
  barrier_struct->condvar.type = CC_INVALID_TYPE;
  (void) cc_condvar_struct_init(&barrier_struct->condvar);

  barrier_struct->type = CC_BARRIER_TYPE;
  return;
} /* cc_barrier_struct_init() */

/*
  \internal
*/

void
cc_barrier_struct_clean(cc_barrier * barrier_struct)
{
  assert(barrier_struct != NULL && barrier_struct->type == CC_BARRIER_TYPE);
  cc_mutex_struct_clean(&barrier_struct->mutex);
  cc_condvar_struct_clean(&barrier_struct->condvar);
  barrier_struct->type = CC_INVALID_TYPE;
} /* cc_barrier_struct_clean() */

/* ********************************************************************** */

/*
*/

cc_barrier *
cc_barrier_construct(unsigned int count)
{
  cc_barrier * barrier;
  barrier = (cc_barrier *) malloc(sizeof(cc_barrier));
  barrier->type = CC_INVALID_TYPE;
  cc_barrier_struct_init(barrier, count);

  if (barrier->type != CC_BARRIER_TYPE) {
    free(barrier);
    barrier = NULL;
  }
  return barrier;
} /* cc_barrier_construct() */

/*
*/

void
cc_barrier_destruct(cc_barrier * barrier)
{
  assert((barrier != NULL) && (barrier->type == CC_BARRIER_TYPE));
  cc_barrier_struct_clean(barrier);
  free(barrier);
} /* cc_barrier_destruct() */

/*
*/

int
cc_barrier_enter(cc_barrier * barrier)
{
  int status, cycle, cancel, tmp;
  assert((barrier != NULL) && (barrier->type == CC_BARRIER_TYPE));

  status = cc_mutex_lock(&barrier->mutex);
/*
  if ( status != CC_OK )
    return FALSE;
*/

  cycle = barrier->cycle;
  if (--barrier->counter == 0) {
    barrier->cycle++;
    barrier->counter = barrier->threshold;
    cc_condvar_wake_all(&barrier->condvar);
    cc_mutex_unlock(&barrier->mutex);
/*
    status = pthread_cond_broadcast(&(barrier->pthread.condvar));
    pthread_mutex_unlock(&(barrier->pthread.mutex));
    if ( status != 0 )
      return CC_ERROR;
*/
  } else {
#ifdef USE_PTHREAD
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel);
#endif /* USE_PTHREAD */
/*
    while ( cycle == barrier->cycle ) {
      cc_condvar_wait(
      status = pthread_cond_wait(&(barrier->pthread.condvar),
                                 &(barrier->pthread.mutex));
      if ( status != 0 ) break;
    }
*/
#ifdef USE_PTHREAD
    pthread_setcancelstate(cancel, &tmp);
#endif /* USE_PTHREAD */
    cc__mutex_unlock(&barrier->mutex);
    if ( status != 0 ) return CC_ERROR;
  }

  return CC_OK;
} /* cc_barrier_enter() */

/* ********************************************************************** */

/*!
  \class SbBarrier Inventor/threads/SbBarrier.h
  \brief The SbBarrier class implements the "barrier" multi-thread
  synchronization technique.
  \ingroup multi-threading

  A barrier is a synchronization mechanism that is used for blocking
  threads as they enter the barrier until a given number of threads are
  blocked, at which point all the threads are released again.
*/

/*!
  \fn SbBarrier::SbBarrier(int count)

  Constructor.  The SbBarrier will be set for \c count number of
  threads.
*/

/*!
  \fn SbBarrier::~SbBarrier(void)

  Destructor.
*/

/*!
  \fn int SbBarrier::enter(void)

  This method blocks the calling thread on this barrier.  It returns
  when the given number of threads have been blocked.
*/

/* ********************************************************************** */
