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

#include <Inventor/C/threads/barrier.h>
#include <Inventor/C/threads/barrierp.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* ********************************************************************** */

/*
*/

cc_barrier *
cc_barrier_construct(unsigned int count)
{
  cc_barrier * barrier;
  barrier = (cc_barrier *) malloc(sizeof(cc_barrier));
  barrier->numthreads = count;
  barrier->counter = 0;
  barrier->mutex = cc_mutex_construct();
  barrier->condvar = cc_condvar_construct();
  return barrier;
}

/*
*/

void
cc_barrier_destruct(cc_barrier * barrier)
{
  assert(barrier != NULL);
         
  cc_condvar_wake_all(barrier->condvar);
  cc_condvar_destruct(barrier->condvar);
  cc_mutex_destruct(barrier->mutex);
  free(barrier);
}

/*
*/

int
cc_barrier_enter(cc_barrier * barrier)
{
  assert(barrier != NULL);
  cc_mutex_lock(barrier->mutex);
  barrier->counter++;
  if (barrier->counter == barrier->numthreads) {
    barrier->counter = 0;
    cc_mutex_unlock(barrier->mutex);
    cc_condvar_wake_all(barrier->condvar);
    return 1;
  }
  else {
    cc_condvar_wait(barrier->condvar, barrier->mutex);
    cc_mutex_unlock(barrier->mutex);
  }
  return 0;
}

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
  when the given number of threads have been blocked. 1 is
  returned if the caller was the last thread to enter the barrier,
  0 otherwise.
*/

/* ********************************************************************** */
