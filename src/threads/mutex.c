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

// define to use the Win32 mutex object, undefine to use critical_sections (which has a trylock bug)
#define USE_W32_MUTEX 1

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>

#include <Inventor/C/base/debug.h>

#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef _WIN32

#include <Inventor/C/base/basep.h>

#endif /* WIN32 */


/**************************************************************************/

/*
  \internal
*/

void
cc_mutex_struct_init(cc_mutex * mutex_struct)
{
  CC_PTHREAD(int status;)
  assert(mutex_struct->type == CC_INVALID_TYPE);

#ifdef USE_PTHREAD
  status = pthread_mutex_init(&(mutex_struct->pthread.mutexid), NULL);
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_init() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX
  mutex_struct->w32thread.mutexhandle = CreateMutex(NULL, FALSE, NULL);
  if (mutex_struct->w32thread.mutexhandle == NULL) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CreateMutex() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#else
  mutex_struct->w32thread.lock_count = -1;
  InitializeCriticalSection(&(mutex_struct->w32thread.critical_section));
#endif /* USE_W32_MUTEX */
#endif /* USE_W32THREAD */

  mutex_struct->type = CC_MUTEX_TYPE;
  return;

error:
  /* FIXME: do cleanup in case it's needed */
  mutex_struct->type = CC_INVALID_TYPE;
} /* cc_mutex_struct_init() */

/*
  \internal
*/

void
cc_mutex_struct_clean(cc_mutex * mutex_struct)
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  assert(mutex_struct->type == CC_MUTEX_TYPE);

#ifdef USE_PTHREAD
  status = pthread_mutex_destroy(&(mutex_struct->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_destroy() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX
  status = CloseHandle(mutex_struct->w32thread.mutexhandle);
  if (status == FALSE) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CloseHandle() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#else
  DeleteCriticalSection(&(mutex_struct->w32thread.critical_section));
#endif /* USE_W32THREAD */
#endif /* USE_W32THREAD */

  mutex_struct->type = CC_INVALID_TYPE;
  return;

error:
  mutex_struct->type = CC_INVALID_TYPE;
  /* FIXME: should set to something else ("corrupt"?)  20011113 larsa */
} /* cc_mutex_struct_clean() */

/**************************************************************************/

/*
*/

cc_mutex *
cc_mutex_construct(void)
{
  cc_mutex * mutex;
  mutex = (cc_mutex *) malloc(sizeof(cc_mutex));
  assert(mutex != NULL);
  mutex->type = CC_INVALID_TYPE;
  cc_mutex_struct_init(mutex);
  if ( mutex->type != CC_MUTEX_TYPE ) goto error;
  return mutex;

error:
  free(mutex);
  return NULL;
} /* cc_mutex_construct() */

/*
*/

void
cc_mutex_destruct(
  cc_mutex * mutex )
{
  CC_PTHREAD(int status;)
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));
  cc_mutex_struct_clean(mutex);
  if ( mutex->type != CC_INVALID_TYPE ) goto error;
  free(mutex);
  return;

error:
  free(mutex);
} /* cc_mutex_destruct() */

/**************************************************************************/

/*
*/

int
cc_mutex_lock(
  cc_mutex * mutex )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(DWORD status;)
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));

#ifdef USE_PTHREAD
  status = pthread_mutex_lock(&(mutex->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_lock() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX
  status = WaitForSingleObject(mutex->w32thread.mutexhandle, INFINITE);
  if (status == WAIT_FAILED) {  
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitSingleObject() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  };
#else
  if (InterlockedIncrement(&(mutex->w32thread.lock_count)) ==0) {
    /* we were the first to lock */
  }
  EnterCriticalSection(&(mutex->w32thread.critical_section));
#endif /* USE_W32_MUTEX */
#endif /* USE_W32THREAD */

  return TRUE;

error:
  return FALSE;
} /* cc_mutex_lock() */

/*
*/

int
cc_mutex_try_lock(
  cc_mutex * mutex )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));

#ifdef USE_PTHREAD
  status = pthread_mutex_trylock(&(mutex->pthread.mutexid));
  if ( status != 0 && status != EBUSY) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_trylock() error: %d\n", status);
    goto error;
  }
  return status == 0;
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD

#ifdef USE_W32_MUTEX
  status = WaitForSingleObject(mutex->w32thread.mutexhandle, 0);
  if (status == WAIT_TIMEOUT)
    return CC_BUSY; // FIXME: correct return value
  else if ( (status == WAIT_OBJECT_0) || (status == WAIT_ABANDONED) )
    return TRUE;
  else {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitSingleObject() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  };
#else

#if 0
  /* 20011106 thammer, this function is unsupported in Win95/98/Me and
   * NT<=3.51 to enable support, #define _WIN32_WINNT>=0x0400, before
   * including winbase.h we might want to add a test for this in
   * configure, or at runtime ... investigate further. */
  status = TryEnterCriticalSection(&(mutex->w32thread.critical_section));
  return status;
  /* 20011108 thammer, decided to rewrite to avoid using this function */
#endif


  if (InterlockedIncrement(&(mutex->w32thread.lock_count)) ==0) {
    /* we were the first to lock fixme: 20011108 thammer: potential
     * problem, allthough unlikely to happen very often: if this
     * thread looses control here, and some other thread performs a
     * lock before this thread enters the critical section, then this
     * try_lock function can risk actually locking a locked mutex -
     * and it will then be waiting for the mutex. */
    EnterCriticalSection(&(mutex->w32thread.critical_section));
    return TRUE;
  } else {
    /* we didn't lock, so decrement the count again */
    InterlockedDecrement(&(mutex->w32thread.lock_count));
    return FALSE;
  }
#endif /* USE_W32_MUTEX */

#endif /* USE_W32THREAD */

error:
  return FALSE;
} /* cc_mutex_try_lock() */

/*
*/

int
cc_mutex_unlock(
  cc_mutex * mutex )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));

#ifdef USE_PTHREAD
  status = pthread_mutex_unlock(&(mutex->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX
  status = ReleaseMutex(mutex->w32thread.mutexhandle);
  if (status == FALSE) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "ReleaseMutex() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#else
  LeaveCriticalSection(&(mutex->w32thread.critical_section));
  if (InterlockedDecrement(&(mutex->w32thread.lock_count)) >=0) {
    /* some other thread is waiting */
  }
#endif /* USE_W32_MUTEX */
#endif /* USE_W32THREAD */


  return TRUE;

error:
  return FALSE;
} /* cc_mutex_unlock() */

/* ********************************************************************** */

/*!
  \class SbMutex Inventor/threads/SbMutex.h
  \brief A basic class for managing a mutex
  \ingroup multi-threading

  FIXME: write doc
*/

/*!
  \fn SbMutex::SbMutex(void)

  FIXME: write doc
*/

/*!
  \fn SbMutex::~SbMutex(void)

  FIXME: write doc
*/

/*!
  \fn SbBool SbMutex::lock(void)

  FIXME: write doc
*/

/*!
  \fn SbBool SbMutex::unlock(void)

  FIXME: write doc
*/

/*!
  \fn SbBool SbMutex::tryLock(void)

  FIXME: write doc
*/

/* ********************************************************************** */
