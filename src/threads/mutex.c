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

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>

#include <Inventor/C/errors/debugerror.h>

#include "Inventor/C/tidbits.h" /* coin_atexit() */
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <errno.h>
#include <Inventor/C/base/basep.h> /* needed for cc_internal_w32_getlasterrorstring */

#ifdef USE_PTHREAD
#include "mutex_pthread.ic"
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
/* we test if Win32 TryEnterCriticalSection exists, and use Win32
   critical section if it does, and Win32 mutex if it doesn't */
typedef BOOL cc_mutex_TryEnterCriticalSection_func(LPCRITICAL_SECTION);
static cc_mutex_TryEnterCriticalSection_func * cc_mutex_TryEnterCriticalSection = NULL; 
#include "mutex_win32mutex.ic" 
#include "mutex_win32cs.ic" 
#endif /* USE_W32THREAD */

/**************************************************************************/

/*
  \internal
*/

void
cc_mutex_struct_init(cc_mutex * mutex_struct)
{
  int ok;
#ifdef USE_W32THREAD
  if (cc_mutex_TryEnterCriticalSection)
    ok = win32_cs_struct_init(mutex_struct);
  else 
    ok = win32_mutex_struct_init(mutex_struct);
#else /* USE_W32THREAD */
  ok = internal_mutex_struct_init(mutex_struct);
#endif /* ! USE_W32THREAD */
  assert(ok);
}

/*
  \internal
*/

void
cc_mutex_struct_clean(cc_mutex * mutex_struct)
{
  int ok;
  assert(mutex_struct);
#ifdef USE_W32THREAD
  if (cc_mutex_TryEnterCriticalSection)
    ok = win32_cs_struct_clean(mutex_struct);
  else 
    ok = win32_mutex_struct_clean(mutex_struct);
#else /* USE_W32THREAD */  
  ok = internal_mutex_struct_clean(mutex_struct);
#endif /* ! USE_W32THREAD */
  assert(ok == CC_OK);
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
  cc_mutex_struct_init(mutex);

  return mutex;
}

/*
*/

void
cc_mutex_destruct(cc_mutex * mutex)
{
  assert(mutex != NULL);
  cc_mutex_struct_clean(mutex);
  free(mutex);
}

/**************************************************************************/

/*
*/

int
cc_mutex_lock(cc_mutex * mutex)
{
  int ok;
  assert(mutex != NULL);

#ifdef USE_W32THREAD
  if (cc_mutex_TryEnterCriticalSection)
    ok = win32_cs_lock(mutex);
  else 
    ok = win32_mutex_lock(mutex);
#else /* USE_W32THREAD */  
  ok = internal_mutex_lock(mutex);
#endif /* USE_W32THREAD */
  assert(ok == CC_OK);
  return ok;
}

/*
*/

int
cc_mutex_try_lock(cc_mutex * mutex)
{
  int ok;
  assert(mutex != NULL);
#ifdef USE_W32THREAD
  if (cc_mutex_TryEnterCriticalSection)
    ok = win32_cs_try_lock(mutex);
  else 
    ok = win32_mutex_try_lock(mutex);
#else /* USE_W32THREAD */  
  ok = internal_mutex_try_lock(mutex);
#endif /* ! USE_W32THREAD */
  assert(ok == CC_OK || ok == CC_BUSY);
  return ok;
}

/*
*/

int
cc_mutex_unlock(cc_mutex * mutex)
{
  int ok;
  assert(mutex != NULL);
#ifdef USE_W32THREAD
  if (cc_mutex_TryEnterCriticalSection)
    ok = win32_cs_unlock(mutex);
  else 
    ok = win32_mutex_unlock(mutex);
#else /* USE_W32THREAD */  
  ok = internal_mutex_unlock(mutex);
#endif /* USE_W32THREAD */
  assert(ok == CC_OK);
  return ok;
}

static cc_mutex * cc_global_mutex = NULL;

static void
cc_mutex_cleanup(void)
{
  cc_mutex_destruct(cc_global_mutex);
  cc_global_mutex = NULL;
}

void
cc_mutex_init(void)
{
#ifdef USE_W32THREAD /* TryEnterCriticalSection test. */

  HINSTANCE h = GetModuleHandle("kernel32.dll");
  /* If we can't get a handle to kernel32.dll, something is seriously
     wrong, and we should investigate. <mortene> */
  assert(h && "GetModuleHandle('kernel32.dll') failed!");

  /* This function is unsupported in Win95/98/Me and NT <=3.51, but we
     still want to use it if it's available, since it can provide
     major speed-ups for certain aspects of Win32 mutex handling. */
  cc_mutex_TryEnterCriticalSection = (cc_mutex_TryEnterCriticalSection_func *)
    GetProcAddress(h, "TryEnterCriticalSection");
  
#endif /* USE_W32THREAD */

  if (cc_global_mutex == NULL) {
    cc_global_mutex = cc_mutex_construct();
    coin_atexit((coin_atexit_f*) cc_mutex_cleanup);
  }
}

void 
cc_mutex_global_lock(void)
{
  /* Do this test in case a mutex is needed before cc_mutex_init() is
     called (called from SoDB::init()). This is safe, since the
     application should not be multithreaded before SoDB::init() is
     called */
  if (cc_global_mutex == NULL) cc_mutex_init();
  
  (void) cc_mutex_lock(cc_global_mutex);
}

void 
cc_mutex_global_unlock(void)
{
  (void) cc_mutex_unlock(cc_global_mutex);
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
