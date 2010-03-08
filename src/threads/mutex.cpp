/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/* FIXME: should provide dummy implementations of the (few) internal
   public cc_mutex_*() calls, so one can include the header files
   mutex.h and SbMutex.h without #ifdef checks, and also declare
   e.g. SbMutex instances when thread-support is missing.

   This would clean up source code everywhere we're using mutex'es.

   20050516 mortene.
*/

#include <Inventor/C/threads/mutex.h>

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <errno.h>
#include <float.h>

#include <Inventor/C/base/time.h>
#include <Inventor/C/errors/debugerror.h>

#include "threads/mutexp.h"
#include "tidbitsp.h"

#ifdef USE_PTHREAD
#include "mutex_pthread.icc"
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
/* we test if Win32 TryEnterCriticalSection exists, and use Win32
   critical section if it does, and Win32 mutex if it doesn't */
typedef BOOL (WINAPI * cc_mutex_TryEnterCriticalSection_func)(LPCRITICAL_SECTION);
static cc_mutex_TryEnterCriticalSection_func cc_mutex_TryEnterCriticalSection = NULL; 
#include "mutex_win32mutex.icc" 
#include "mutex_win32cs.icc" 
#endif /* USE_W32THREAD */

/**************************************************************************/

static double maxmutexlocktime = DBL_MAX;
static double reportmutexlocktiming = DBL_MAX;

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

/* debugging. for instance useful for checking that there's not
   excessive mutex construction. */

/* don't hide 'static' these to hide them in file-scope, as they are
   used from rwmutex.cpp and recmutex.cpp aswell. */
unsigned int cc_debug_mtxcount = 0;
const char * COIN_DEBUG_MUTEX_COUNT = "COIN_DEBUG_MUTEX_COUNT";

/**************************************************************************/

cc_mutex *
cc_mutex_construct(void)
{
  cc_mutex * mutex;
  mutex = (cc_mutex *) malloc(sizeof(cc_mutex));
  assert(mutex != NULL);
  cc_mutex_struct_init(mutex);

  { /* debugging */
    const char * env = coin_getenv(COIN_DEBUG_MUTEX_COUNT);
    if (env && (atoi(env) > 0)) {
      cc_debug_mtxcount += 1;
      (void)fprintf(stderr, "DEBUG: live mutexes +1 => %u (mutex++)\n",
                    cc_debug_mtxcount);
    }
  }

  return mutex;
}

void
cc_mutex_destruct(cc_mutex * mutex)
{
  { /* debugging */
    const char * env = coin_getenv(COIN_DEBUG_MUTEX_COUNT);
    if (env && (atoi(env) > 0)) {
      assert((cc_debug_mtxcount > 0) && "skewed mutex construct/destruct pairing");
      cc_debug_mtxcount -= 1;
      (void)fprintf(stderr, "DEBUG: live mutexes -1 => %u (mutex--)\n",
                    cc_debug_mtxcount);
    }
  }

  assert(mutex != NULL);
  cc_mutex_struct_clean(mutex);
  free(mutex);
}

/**************************************************************************/

/*
*/
void
cc_mutex_lock(cc_mutex * mutex)
{
  int ok;
  SbBool timeit;
  cc_time start = 0.0;

  assert(mutex != NULL);

  timeit = (maxmutexlocktime != DBL_MAX) || (reportmutexlocktiming != DBL_MAX);
  if (timeit) { start = cc_time_gettimeofday(); }

#ifdef USE_W32THREAD
  ok = cc_mutex_TryEnterCriticalSection ? win32_cs_lock(mutex) : win32_mutex_lock(mutex);
#else /* USE_W32THREAD */  
  ok = internal_mutex_lock(mutex);
#endif /* USE_W32THREAD */

  assert(ok == CC_OK);

  /* This is here as an optional debugging aid, when having problems
     related to locks that are held too long. (Typically resulting in
     unresponsive user interaction / lags.)  */
  if (timeit) {
    const cc_time spent = cc_time_gettimeofday() - start;

    if (spent >= reportmutexlocktiming) {
      /* Can't use cc_debugerror_postinfo() here, because we get a
         recursive call to this function, and a non-terminating lock /
         hang. */
      (void)fprintf(stdout, "DEBUG cc_mutex_lock(): mutex %p spent %f secs in lock\n",
                    mutex, spent);
    }

    assert(spent <= maxmutexlocktime);
  }
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

void
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
  const char * env = coin_getenv("COIN_DEBUG_MUTEXLOCK_MAXTIME");

#ifdef USE_W32THREAD /* TryEnterCriticalSection test. */

  HINSTANCE h = GetModuleHandle("kernel32.dll");
  /* If we can't get a handle to kernel32.dll, something is seriously
     wrong, and we should investigate. <mortene> */
  assert(h && "GetModuleHandle('kernel32.dll') failed!");

  /* This function is unsupported in Win95/98/Me and NT <=3.51, but we
     still want to use it if it's available, since it can provide
     major speed-ups for certain aspects of Win32 mutex handling. */
  cc_mutex_TryEnterCriticalSection = (cc_mutex_TryEnterCriticalSection_func)
    GetProcAddress(h, "TryEnterCriticalSection");
  
#endif /* USE_W32THREAD */

  if (cc_global_mutex == NULL) {
    cc_global_mutex = cc_mutex_construct();
    /* atexit priority makes this callback trigger after other cleanup
       functions. */
    /* FIXME: not sure if this really needs the "- 1", but I added it
       to keep the same order wrt the other thread-related clean-up
       functions, since before I changed hard-coded numbers for
       enumerated values for coin_atexit() invocations. 20060301 mortene. */
    coin_atexit((coin_atexit_f*) cc_mutex_cleanup, CC_ATEXIT_THREADING_SUBSYSTEM_LOWPRIORITY);
  }

  if (env) { maxmutexlocktime = atof(env); }

  env = coin_getenv("COIN_DEBUG_MUTEXLOCK_TIMING");
  if (env) { reportmutexlocktiming = atof(env); }
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
  \brief A basic class for managing a mutex.
  \ingroup threads

  This class provides a portable framework around the mutex interface
  of the underlying native thread-handling toolkit.
*/

/*!
  \fn SbMutex::SbMutex(void)

  Constructor.
*/

/*!
  \fn SbMutex::~SbMutex()

  Destructor.
*/

/*!
  \fn int SbMutex::lock(void)

  This method locks the mutex. \c 0 is returned on success.

  If the mutex was previously unlocked, the thread will lock the mutex
  and continue running. If the mutex was already locked when this call
  is made, the thread will be suspended until the other thread holding
  the mutex releases it.

  This is a blocking operation.
*/

/*!
  \fn int SbMutex::unlock(void)

  This method unlocks the mutex.
*/

/*!
  \fn SbBool SbMutex::tryLock(void)

  This method tries to lock the mutex, and returns whether it was
  locked or not.  This is a non-blocking operation.
*/


/*!
  \class SbThreadMutex Inventor/threads/SbThreadMutex.h
  \brief A basic class for managing a recursive  mutex.
  \ingroup threads

  This class provides a portable framework around the mutex interface
  of the underlying native thread-handling toolkit.
*/

/*!
  \fn SbThreadMutex::SbThreadMutex(void)

  Constructor.
*/

/*!
  \fn SbThreadMutex::~SbThreadMutex()

  Destructor.
*/

/*!
  \fn int SbThreadMutex::lock(void)

  This method locks the mutex. \c 0 is returned on success.

  If the mutex was previously unlocked, the thread will lock the mutex
  and continue running. If the mutex was already locked by another thread
  when this call is made, this method will block until the other thread holding
  the mutex releases it.

  This is a blocking operation.
*/

/*!
  \fn int SbThreadMutex::unlock(void)

  This method unlocks the mutex.
*/

/*!
  \fn SbBool SbThreadMutex::tryLock(void)

  This method tries to lock the mutex, and returns whether it was
  locked by another thread or not. This is a non-blocking operation.
*/

/* ********************************************************************** */

/*!
  \class SbThreadAutoLock Inventor/threads/SbThreadAutoLock.h
  \brief Simple convenience class for locking access to a function.
  \ingroup threads

  This class provides a simple convenience mechanism for automatically
  locking access to a function that is not re-entrant.

  Usage example:

  \code
  void
  myfunction(void)
  {
    SbThreadAutoLock lock(aMutexPtr);

    // [other code]
  }
  \endcode

  In the class constructor, SbMutex::lock() is called on the mutex,
  and when the function exits (this is the convenience part) the
  destructor will automatically be invoked, calling SbMutex::unlock()
  on the same mutex.
*/

/*!
  \fn SbThreadAutoLock::SbThreadAutoLock(SbMutex * mutex)

  The constructor calls SbMutex::lock() on \a mutex.
*/

/*!
  \fn SbThreadAutoLock::SbThreadAutoLock(SbThreadMutex * mutex)

  The constructor calls SbThreadMutex::lock() on \a mutex.
*/

/*!
  \fn SbThreadAutoLock::~SbThreadAutoLock()

  The destructor calls unlock() on the mutex passed in as a
  parameter to the constructor.
*/

/*!
  \var SbThreadAutoLock::mutex
  \var SbThreadAutoLock::recmutex

  \COININTERNAL
*/

/* ********************************************************************** */
