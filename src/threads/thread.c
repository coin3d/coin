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

#include <Coin/threads/thread.h>
#include <Coin/threads/threadp.h>

#include <Coin/base/debug.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

#ifdef USE_W32THREAD

DWORD WINAPI cc_w32thread_thread_proc( LPVOID lpParameter)
{
  cc_thread *thread = (cc_thread *)lpParameter;
  return (DWORD) thread->func(thread->closure);
};

#endif /* USE_W32THREAD */

/* ********************************************************************** */

/*
 - use static table of cc_thread structures?
 - use cc_storage to reference self-structure for cc_thread_get_self()?
*/

/* ********************************************************************** */

/*
  \internal
  This function is provided so compound types can have thread structure
  members instead of just pointers.  The point is to avoid the extra
  memory allocation calls.  This function is basically the constructor
  without the memory allocation part.
*/

void
cc_thread_struct_init(cc_thread * thread_struct)
{
  /* FIXME: move code from _construct() */
} /* cc_thread_struct_init() */

/*
  \internal
  This function is provided so compound types can have thread structure
  members instead of just pointers.  The point is to avoid the extra
  memory allocation calls.  This function is basically the destructor
  without the memory freeing part.
*/

void
cc_thread_struct_clean(cc_thread * thread_struct)
{
  /* FIXME: move code from _destruct() */
} /* cc_thread_struct_clean() */

/* ********************************************************************** */

/*
*/

cc_thread *
cc_thread_construct(
  void * (*func)(void *),
  void * closure )
{
  cc_thread * thread;
  CC_PTHREAD(int status;)
  CC_W32THREAD(DWORD threadid_unused;)
  thread = (cc_thread*) malloc(sizeof(cc_thread));
  assert(thread != NULL);
  thread->type = CC_THREAD_TYPE;
  thread->func = func;
  thread->closure = closure;

#ifdef USE_PTHREAD
  status = pthread_attr_init(&(thread->pthread.threadattrs));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_attr_init() error: %d\n", status);
    goto error;
  }

  status = pthread_create(&(thread->pthread.threadid),
                          &(thread->pthread.threadattrs),
                          thread->func, thread->closure);
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_create() error: %d\n", status);
    goto error;
  }

#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD

  thread->w32thread.threadhandle = CreateThread(NULL, 0,
    cc_w32thread_thread_proc, (LPVOID) thread, 0, &threadid_unused);

  /* threadid_unused - see PlatformSDK doc. for CreateThread */

  /* FIXME: thammer 20011108, check PlatformSDK doc for
   * _beginthreadex, _endthreadex, and note about using these with
   * LIBCMT.LIB "A thread that uses functions from the C run-time
   * libraries should use the beginthread and endthread C run-time
   * functions for thread management rather than CreateThread and
   * ExitThread. Failure to do so results in small memory leaks when
   * ExitThread is called. " */

  if (thread->w32thread.threadhandle == NULL) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CreateThread() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }

#endif /* USE_W32THREAD */

  return thread;

error:
  free(thread);
  return NULL;
} /* cc_thread_construct() */

/* ********************************************************************** */

/*
*/

void
cc_thread_destruct(
  cc_thread * thread )
{
  CC_PTHREAD(int status;)
  assert((thread != NULL) && (thread->type == CC_THREAD_TYPE));

#ifdef USE_PTHREAD
  status = pthread_attr_destroy(&(thread->pthread.threadattrs));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_attr_destroy() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

  thread->type = CC_INVALID_TYPE;
  free(thread);
  return;

error:
  free(thread);
} /* cc_thread_destruct() */

/* ********************************************************************** */

/*
*/

int
cc_thread_join(
  cc_thread * thread,
  void ** retvalptr )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(DWORD status;)
  CC_W32THREAD(BOOL bstatus;)
  CC_W32THREAD(DWORD exitcode;)

  assert((thread != NULL) && (thread->type == CC_THREAD_TYPE));

#ifdef USE_PTHREAD
  status = pthread_join(thread->pthread.threadid, retvalptr);
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_join() error: %d\n", status);
    return TRUE;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  status = WaitForSingleObject(thread->w32thread.threadhandle, INFINITE);
  if (status == WAIT_FAILED) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitForSingleObject() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    return TRUE; /* error */
  }
  else if ( status != WAIT_OBJECT_0 ) {
    if ( COIN_DEBUG ) {
      cc_fprintf(stderr, "WaitForSingleObject() - unknown return value: %d\n",
        status);
    }
  }
  bstatus = GetExitCodeThread (thread->w32thread.threadhandle, &exitcode);
  if (bstatus == FALSE) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "GetExitCodeThread() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
  } else {
    *retvalptr = (void *)exitcode;
  }
  /* termination could be forced with TerminateThread() - but this
   * will result in memory leaks - or bigger problems - see Platform
   * SDK doc. */
  CloseHandle(thread->w32thread.threadhandle);
  thread->w32thread.threadhandle = NULL;
#endif

  return FALSE; /* ok */
} /* cc_thread_join() */

/* ********************************************************************** */

#if 0
int
cc_thread_priority_set(
  cc_thread * thread,
  int value )
{
  assert((thread != NULL) && (thread->type == CC_THREAD_TYPE));

#ifdef USE_PTHREAD
#endif /* USE_PTHREAD */

  return 0;
} /* cc_thread_priority_set() */
#endif /* 0 */

#if 0
int
cc_thread_priority_change(
  cc_thread * thread,
  int change )
{
  assert((thread != NULL) && (thread->type == CC_THREAD_TYPE));

#ifdef USE_PTHREAD
#endif /* USE_PTHREAD */

  return 0;
} /* cc_thread_priority_change() */
#endif /* 0 */

#if 0
int
cc_thread_priority_get(
  cc_thread * thread )
{
  assert((thread != NULL) && (thread->type == CC_THREAD_TYPE));

#ifdef USE_PTHREAD
#endif /* USE_PTHREAD */

  return 0;
} /* cc_thread_priority_get() */
#endif /* 0 */


void
cc_sleep(float seconds)
{
#ifndef _WIN32
  /* fixme: 20011107, thammer: create a configure macro to detect
   * which sleep function is available */
  sleep(floor(seconds));
#else
  Sleep((int)(seconds*1000.0));
#endif
};


#ifdef _WIN32

char *cc_internal_w32_getlasterrorstring(DWORD lasterror)
{
  LPVOID lpMsgBuf;
  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      lasterror,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
      (LPTSTR) &lpMsgBuf,
      0,
      NULL
  );
  return (LPTSTR)lpMsgBuf;
};

void cc_internal_w32_freelasterrorstring(char *str)
{
  LocalFree((LPVOID)str);
};

#endif /* _WIN32 */


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
*/

/*!
  \fn static void SbThread::destroy(SbThread * thread)
*/

/*!
  \fn static int SbThread::join(SbThread * thread, void ** retval)
*/

/*!
  \fn int SbThread::join(void ** retval)
*/

/*!
  \fn SbThread::SbThread(cc_thread * thread)
*/

/*!
  \fn SbThread::~SbThread(void)
*/

/* ********************************************************************** */
