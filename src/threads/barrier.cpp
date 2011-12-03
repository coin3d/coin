/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <Inventor/C/threads/barrier.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include "threads/barrierp.h"

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
    cc_condvar_wake_all(barrier->condvar);
    cc_mutex_unlock(barrier->mutex);
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
  \ingroup threads

  A barrier is a synchronization mechanism that is used for blocking
  threads as they enter the barrier until a given number of threads are
  blocked, at which point all the threads are released again.
*/

/*!
  \fn SbBarrier::SbBarrier(unsigned int count)

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
