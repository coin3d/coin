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
#include <Inventor/C/base/basep.h> /* needed for cc_internal_w32_getlasterrorstring */

#ifdef USE_PTHREAD
static int
internal_struct_init(cc_mutex * mutex_struct)
{
  int status = pthread_mutex_init(&(mutex_struct->pthread.mutexid), NULL);
  if (status != 0) {
    if (COIN_DEBUG)
      cc_fprintf(stderr, "pthread_mutex_init() error: %d\n", status);
    return CC_ERROR;
  }
  return CC_OK;
}

static int
internal_struct_clean(cc_mutex * mutex_struct)
{
  int status = pthread_mutex_destroy(&(mutex_struct->pthread.mutexid));
  if (status != 0) {
    if (COIN_DEBUG)
      cc_fprintf(stderr, "pthread_mutex_destroy() error: %d\n", status);
    return CC_ERROR;
  }
  return CC_OK;
}

static int 
internal_lock(cc_mutex * mutex)
{
  int status = pthread_mutex_lock(&(mutex->pthread.mutexid));
  if (status != 0) {
    if (COIN_DEBUG)
      cc_fprintf(stderr, "pthread_mutex_lock() error: %d\n", status);
    return CC_ERROR;
  }
  return CC_OK;
}

static int 
internal_try_lock(cc_mutex * mutex)
{
  int status = pthread_mutex_trylock(&(mutex->pthread.mutexid));
  if (status == EBUSY) return CC_BUSY;
  else if (status != 0) {
    if (COIN_DEBUG)
      cc_fprintf(stderr, "pthread_mutex_trylock() error: %d\n", status);
    return CC_ERROR;
  }
  return CC_OK;
}

static int 
internal_unlock(cc_mutex * mutex)
{
  int status = pthread_mutex_unlock(&(mutex->pthread.mutexid));
  if (status != 0) {
    if (COIN_DEBUG)
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
    return CC_ERROR;
  }
  return CC_OK;
}

#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX

static int
internal_struct_init(cc_mutex * mutex_struct)
{
  mutex_struct->w32thread.mutexhandle = CreateMutex(NULL, FALSE, NULL);
  if (mutex_struct->w32thread.mutexhandle == NULL) {
    if (COIN_DEBUG) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CreateMutex() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    return CC_ERROR;
  }
  return CC_OK;
}

static int
internal_struct_clean(cc_mutex * mutex_struct)
{
  BOOL status = CloseHandle(mutex_struct->w32thread.mutexhandle);
  if (status == FALSE) {
    if (COIN_DEBUG) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CloseHandle() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    return CC_ERROR;
  }
  return CC_OK;
}

static int 
internal_lock(cc_mutex * mutex)
{
  DWORD status;
  status = WaitForSingleObject(mutex->w32thread.mutexhandle, INFINITE);
  if (status == WAIT_FAILED) {
    if (COIN_DEBUG) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitSingleObject() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    return CC_ERROR;
  }
  return CC_OK;
}

static int 
internal_try_lock(cc_mutex * mutex)
{
  DWORD status;
  status = WaitForSingleObject(mutex->w32thread.mutexhandle, 0);
  if (status == WAIT_TIMEOUT)
    return CC_BUSY;
  else if ((status == WAIT_OBJECT_0) || (status == WAIT_ABANDONED))
    return CC_OK;
  
  /* if we get here there was an error */
  if (COIN_DEBUG) {
    DWORD err;
    char *errstr;
    err = GetLastError();
    errstr = cc_internal_w32_getlasterrorstring(err);
    cc_fprintf(stderr, "WaitSingleObject() error: %d, \"%s\"\n",
               err, errstr);
    cc_internal_w32_freelasterrorstring(errstr);
  }
  return CC_ERROR;
}

static int 
internal_unlock(cc_mutex * mutex)
{
  BOOL status = ReleaseMutex(mutex->w32thread.mutexhandle);
  if (status == FALSE) {
    if (COIN_DEBUG) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "ReleaseMutex() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    return CC_ERROR;
  }
  return CC_OK;
}

#else /* USE_W32_MUTEX */

static int
internal_struct_init(cc_mutex * mutex)
{
  mutex_struct->w32thread.lock_count = -1;
  InitializeCriticalSection(&(mutex_struct->w32thread.critical_section));
}

static int
internal_struct_clean(cc_mutex * mutex_struct)
{
  DeleteCriticalSection(&(mutex_struct->w32thread.critical_section));
  /* this function does not have a return value, always return OK */
  return CC_OK;
}

static int 
internal_lock(cc_mutex * mutex)
{
  if (InterlockedIncrement(&(mutex->w32thread.lock_count)) == 0) {
    /* we were the first to lock */

    /* FIXME: What to we need this if-statement for when we're not
       going to do anything? pederb, 2001-12-10 */
    
  }
  EnterCriticalSection(&(mutex->w32thread.critical_section));
  return CC_OK; 
}

static int 
internal_try_lock(cc_mutex * mutex)
{
#if 0
  BOOL status;
  /* 20011106 thammer, this function is unsupported in Win95/98/Me and
   * NT<=3.51 to enable support, #define _WIN32_WINNT>=0x0400, before
   * including winbase.h we might want to add a test for this in
   * configure, or at runtime ... investigate further. */
  status = TryEnterCriticalSection(&(mutex->w32thread.critical_section));
  return status == TRUE ? CC_OK : CC_BUSY;
  /* 20011108 thammer, decided to rewrite to avoid using this function */
#endif

  if (InterlockedIncrement(&(mutex->w32thread.lock_count)) == 0) {
    /* we were the first to lock fixme: 20011108 thammer: potential
     * problem, allthough unlikely to happen very often: if this
     * thread looses control here, and some other thread performs a
     * lock before this thread enters the critical section, then this
     * try_lock function can risk actually locking a locked mutex -
     * and it will then be waiting for the mutex. */
    EnterCriticalSection(&(mutex->w32thread.critical_section));
    return CC_OK;
  }
  /* we didn't lock. Decrement the count again */
  InterlockedDecrement(&(mutex->w32thread.lock_count));
  return CC_BUSY;
}

static int 
internal_unlock(cc_mutex * mutex)
{
  LeaveCriticalSection(&(mutex->w32thread.critical_section));
  if (InterlockedDecrement(&(mutex->w32thread.lock_count)) >= 0) {
    /* some other thread is waiting */

    /* FIXME: What to we need this if-statement for when we're not
       going to do anything? pederb, 2001-12-10 */
  }
  return CC_OK;
}

#endif /* !USE_W32_MUTEX */
#endif /* USE_W32THREAD */


/**************************************************************************/

/*
  \internal
*/

void
cc_mutex_struct_init(cc_mutex * mutex_struct)
{
  int ok;
  assert(mutex_struct->type == CC_INVALID_TYPE);

  ok = internal_struct_init(mutex_struct);
  assert(ok);
  if (ok == CC_OK) 
    mutex_struct->type = CC_MUTEX_TYPE;
  else
    mutex_struct->type = CC_INVALID_TYPE;
}

/*
  \internal
*/

void
cc_mutex_struct_clean(cc_mutex * mutex_struct)
{
  int ok;
  assert(mutex_struct && mutex_struct->type == CC_MUTEX_TYPE);
  ok = internal_struct_clean(mutex_struct);
  assert(ok == CC_OK);
  mutex_struct->type = CC_INVALID_TYPE;
}

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
  assert(mutex->type == CC_MUTEX_TYPE);

  return (mutex->type == CC_MUTEX_TYPE) ?
    mutex : NULL;
}

/*
*/

void
cc_mutex_destruct(cc_mutex * mutex)
{
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));
  cc_mutex_struct_clean(mutex);
  assert(mutex->type == CC_INVALID_TYPE);
  free(mutex);
}

/**************************************************************************/

/*
*/

int
cc_mutex_lock(cc_mutex * mutex)
{
  int ok;
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));
  
  ok = internal_lock(mutex);
  assert(ok == CC_OK);
  return ok;
}

/*
*/

int
cc_mutex_try_lock(cc_mutex * mutex)
{
  int ok;
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));

  ok = internal_try_lock(mutex);
  assert(ok == CC_OK || ok == CC_BUSY);
  return ok;
}

/*
*/

int
cc_mutex_unlock(cc_mutex * mutex)
{
  int ok;
  assert((mutex != NULL) && (mutex->type == CC_MUTEX_TYPE));

  ok = internal_unlock(mutex);
  assert(ok == CC_OK);
  return ok;
}

/* ********************************************************************** */

/*!
  \class SbMutex Inventor/threads/SbMutex.h
  \brief A basic class for managing a mutex
  \ingroup multi-threading

  FIXME: write doc
*/

/*!
  \fn SbMutex::SbMutex(void)

  Constructor.
*/

/*!
  \fn SbMutex::~SbMutex(void)

  Destructor.
*/

/*!
  \fn SbBool SbMutex::lock(void)

  This method locks the mutex.  TRUE is returned on success.
  This is a blocking operation.
*/

/*!
  \fn SbBool SbMutex::unlock(void)

  This method unlocks the mutex.
*/

/*!
  \fn SbBool SbMutex::tryLock(void)

  This method tries to lock the mutex, and returns whether it was locked
  or not.  This is a non-blocking operation.
*/

/* ********************************************************************** */
