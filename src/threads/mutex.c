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

/* define to use the Win32 mutex object, undefine to use
   critical_sections (which has a trylock bug) */
#define USE_W32_MUTEX 1

#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>

#include <Inventor/C/base/debug.h>

#include "../tidbits.h" /* coin_atexit() */
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <errno.h>
#include <Inventor/C/base/basep.h> /* needed for cc_internal_w32_getlasterrorstring */

#ifdef USE_PTHREAD
#include "mutex_pthread.ic"
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#ifdef USE_W32_MUTEX
#include "mutex_win32mutex.ic"
#else /* USE_W32_MUTEX */
#include "mutex_win32cs.ic"
#endif /* ! USE_W32_MUTEX */
#endif /* USE_W32THREAD */


/**************************************************************************/

/*
  \internal
*/

void
cc_mutex_struct_init(cc_mutex * mutex_struct)
{
  int ok;
  ok = internal_struct_init(mutex_struct);
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
  ok = internal_struct_clean(mutex_struct);
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
  assert(mutex != NULL);

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
  assert(mutex != NULL);

  ok = internal_unlock(mutex);
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
