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

#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/threadp.h>
#include <Inventor/C/base/basep.h>
#include <Inventor/C/base/debug.h>

#include <stdlib.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

/* ********************************************************************** */

/*
 FIXME:
 - copy struct malloc/free/init/clean setup scheme from cc_string
 - use static table of cc_thread structures?
 - use cc_storage to reference self-structure for cc_thread_get_self()?
*/

/* ********************************************************************** */

#ifdef USE_PTHREAD
#include "thread_pthread.ic"
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#include "thread_win32.ic"
#endif /* USE_W32THREAD */

/*
*/

cc_thread *
cc_thread_construct(void * (*func)(void *), void * closure)
{
  cc_thread * thread;
  int ok;

  thread = (cc_thread*) malloc(sizeof(cc_thread));
  assert(thread != NULL);
  thread->func = func;
  thread->closure = closure;

  ok = internal_init(thread);
  if (ok) return thread;
  assert(0 && "unable to create thread");
  free(thread);
  return NULL;
}

/* ********************************************************************** */

/*
*/

void
cc_thread_destruct(cc_thread * thread)
{
  int ok;
  assert(thread != NULL);
  ok = internal_clean(thread);
  assert(ok == CC_OK);
  free(thread);
}

/* ********************************************************************** */

/*
*/

int
cc_thread_join(cc_thread * thread,
               void ** retvalptr)
{
  int ok;
  assert(thread != NULL);

  ok = internal_join(thread, retvalptr);
  assert(ok == CC_OK);
  return ok;
}

/* ********************************************************************** */

void
cc_sleep(float seconds)
{
#ifndef _WIN32
  /* FIXME: 20011107, thammer: create a configure macro to detect
   * which sleep function is available */
  sleep(floor(seconds));
#else
  Sleep((int)(seconds*1000.0));
#endif
};


unsigned long 
cc_thread_id(void)
{
#ifdef USE_PTHREAD
  return (unsigned long) pthread_self();
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  return 0; /* not supported yet */
#endif /* USE_W32THREAD */
}


/* ********************************************************************** */

/* maybe use static table of thread structures, reference counted, to be
   able to implement something like this, if needed */
/* cc_thread * cc_thread_get_self(void); */

/* ********************************************************************** */

/*
 * We don't really want to expose internal id types, which would mean we
 * must include threads-implementation-specific headers in the header files.
 * It's therefore better to implement the missing/needed functionality for
 * the cc_thread type, so id peeking won't be necessary.
 */

/* <id> cc_thread_get_id(cc_thread * thread); */
/* <id> cc_thread_get_current_id(void); */

/* ********************************************************************** */

/*!
  \class SbThread Inventor/threads/SbThread.h
  \ingroup multi-threading
*/

/*!
  \fn static SbThread * SbThread::create(void (*func)(void *), void * closure)

  This function creates a new thread, or returns NULL on failure.
*/

/*!
  \fn static void SbThread::destroy(SbThread * thread)

  This function destroys a thread.
*/

/*!
  \fn static int SbThread::join(SbThread * thread, void ** retval)

  This function waits on the death of the given thread, returning the thread's
  return value at the location pointed to by \c retval.
*/

/*!
  \fn int SbThread::join(void ** retval)

  This function waits on the death of the given thread, returning the thread's
  return value at the location pointed to by \c retval.
*/

/*!
  \fn SbThread::SbThread(cc_thread * thread)

  Protected constructor handling the internal thread ADT.

  \sa SbThread::create
*/

/*!
  \fn SbThread::~SbThread(void)

  Destructor.

  \sa SbThread::destroy
*/

/* ********************************************************************** */
