/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/C/threads/rwmutex.h>
#include <Inventor/C/threads/rwmutexp.h>

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/condvar.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

/*!
  \internal
*/
void
cc_rwmutex_struct_init(cc_rwmutex * rwmutex)
{
  cc_mutex_struct_init(&rwmutex->mutex);
  cc_condvar_struct_init(&rwmutex->read);
  cc_condvar_struct_init(&rwmutex->write);

  rwmutex->readers = 0;
  rwmutex->readwaiters = 0;
  rwmutex->writers = 0;
  rwmutex->writewaiters = 0;
  rwmutex->policy = CC_READ_PRECEDENCE;
}

/*!
  \internal
*/
void
cc_rwmutex_struct_clean(cc_rwmutex * rwmutex)
{
  cc_mutex_struct_clean(&rwmutex->mutex);
  cc_condvar_struct_clean(&rwmutex->read);
  cc_condvar_struct_clean(&rwmutex->write);
}

/*
  Construct a read-write mutex, with read precedence.
*/

cc_rwmutex *
cc_rwmutex_construct(void)
{
  cc_rwmutex * rwmutex;
  rwmutex = (cc_rwmutex *) malloc(sizeof(cc_rwmutex));
  assert(rwmutex != NULL);
  cc_rwmutex_struct_init(rwmutex);
  return rwmutex;
}

/*
  Construct a read-write mutex, with read precedence or write precedence.
*/

cc_rwmutex *
cc_rwmutex_construct_etc(enum cc_precedence policy)
{
  cc_rwmutex * rwmutex;
  assert((policy == CC_READ_PRECEDENCE) || (policy == CC_WRITE_PRECEDENCE));
  rwmutex = cc_rwmutex_construct();
  assert(rwmutex != NULL);
  rwmutex->policy = policy;
  return rwmutex;
}

/*
  Destruct a read-write mutex.
*/

void
cc_rwmutex_destruct(cc_rwmutex * rwmutex)
{
  assert(rwmutex != NULL);
  cc_rwmutex_struct_clean(rwmutex);
  free(rwmutex);
}

/* ********************************************************************** */

/*
*/
int
cc_rwmutex_write_lock(cc_rwmutex * rwmutex)
{
  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->readers == 0 &&
      rwmutex->writers == 0 &&
      rwmutex->readwaiters == 0 &&
      rwmutex->writewaiters == 0) {
    rwmutex->writers++;
    cc_mutex_unlock(&rwmutex->mutex);
    return CC_OK;
  }
  rwmutex->writewaiters++;
  
  (void) cc_condvar_wait(&rwmutex->write, &rwmutex->mutex);
  assert(rwmutex->readers == 0 && rwmutex->writers == 0);
  rwmutex->writers++;
  rwmutex->writewaiters--;
  assert(rwmutex->writewaiters >= 0);
  cc_mutex_unlock(&rwmutex->mutex);
  return CC_OK;
} /* cc_rwmutex_write_lock() */

/*
*/

int
cc_rwmutex_write_try_lock(cc_rwmutex * rwmutex)
{
  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->readers == 0 &&
      rwmutex->writers == 0 &&
      rwmutex->readwaiters == 0 &&
      rwmutex->writewaiters == 0) {
    rwmutex->writers++;
    cc_mutex_unlock(&rwmutex->mutex);
    return CC_OK;
  }
  cc_mutex_unlock(&rwmutex->mutex);
  return CC_BUSY;
} /* cc_rwmutex_write_try_lock() */

/*
*/

int
cc_rwmutex_write_unlock(cc_rwmutex * rwmutex)
{
  int rwait;
  int wwait;
  (void) cc_mutex_lock(&rwmutex->mutex);
  rwmutex->writers--;
  assert(rwmutex->writers >= 0);
  rwait = rwmutex->readwaiters;
  wwait = rwmutex->writewaiters;
  (void) cc_mutex_unlock(&rwmutex->mutex);

  if (rwmutex->policy == CC_READ_PRECEDENCE) {
    if (rwait) cc_condvar_wake_all(&rwmutex->read);
    else cc_condvar_wake_one(&rwmutex->write);
  } 
  else {
    if (wwait) cc_condvar_wake_one(&rwmutex->write);
    else cc_condvar_wake_all(&rwmutex->read);
  }
  return CC_OK;
}

/*
*/

int
cc_rwmutex_read_lock(cc_rwmutex * rwmutex)
{
  assert(rwmutex != NULL);
  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->writers == 0) {
    rwmutex->readers++;
    (void) cc_mutex_unlock(&rwmutex->mutex);
    return CC_OK;
  }
  rwmutex->readwaiters++;
  (void) cc_condvar_wait(&rwmutex->read, &rwmutex->mutex);
  assert(rwmutex->writers == 0);
  rwmutex->readers++;
  rwmutex->readwaiters--;
  assert(rwmutex->readwaiters >= 0);
  cc_mutex_unlock(&rwmutex->mutex);
  return CC_OK;
}

/*
*/

int
cc_rwmutex_read_try_lock(cc_rwmutex * rwmutex)
{
  assert(rwmutex != NULL);

  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->writers == 0 &&
      rwmutex->writewaiters == 0) {
    rwmutex->readers++;
    (void) cc_mutex_unlock(&rwmutex->mutex);
    return CC_OK;
  }
  (void) cc_mutex_unlock(&rwmutex->mutex);
  return CC_BUSY;
}

/*
*/

int
cc_rwmutex_read_unlock(cc_rwmutex * rwmutex)
{
  int rwait;
  int wwait;
  int readers;
  (void) cc_mutex_lock(&rwmutex->mutex);
  rwmutex->readers--;
  readers = rwmutex->readers;
  assert(readers >= 0);
  rwait = rwmutex->readwaiters;
  wwait = rwmutex->writewaiters;
  (void) cc_mutex_unlock(&rwmutex->mutex);

  if (rwmutex->policy == CC_READ_PRECEDENCE || readers) {
    if (rwait) cc_condvar_wake_all(&rwmutex->read);
    else if (!readers) cc_condvar_wake_one(&rwmutex->write);
  }
  else {
    if (wwait) cc_condvar_wake_one(&rwmutex->write);
    else cc_condvar_wake_all(&rwmutex->read);
  }
  return CC_OK;
} /* cc_rwmutex_read_unlock() */

/* ********************************************************************** */

/*!
  \class SbRWMutex Inventor/threads/SbRWMutex.h
  \ingroup multi-threading

  The SbRWMutex is a mutex that i sused to grant both read and write access
  to the data it protects.  Multiple threads can have read locks on the data
  at once, but only one write-lock can be given out and not while a thread
  has a read lock on the data.

  The policy on granting read locks to threads when a thread is waiting for
  the write lock is something that should be configurable at construction
  time but which isn't at the moment.
*/

/*!
  \fn SbRWMutex::SbRWMutex(void)

  Constructor.
*/

/*!
  \fn SbRWMutex::~SbRWMutex(void)

  Destructor.
*/

/*!
  \fn int SbRWMutex::writeLock(void)

  This method blocks the calling thread until it gains write lock status
  on the SbRWMutex object.
*/

/*!
  \fn SbBool SbRWMutex::tryWriteLock(void)

  This method tries to gain write lock status on the SbRWMutex.  TRUE is
  returned if it was successful, FALSE otherwise.  This is a non-blocking
  operation.
*/

/*!
  \fn int SbRWMutex::writeUnlock(void)

  This method is used for unlocking the calling thread's write lock.
*/

/*!
  \fn int SbRWMutex::readLock(void)

  This method blocks the calling thread until it gains read lock status
  on the SbRWMutex object.
*/

/*!
  \fn SbBool SbRWMutex::tryReadLock(void)

  This method tries to gain read lock status on the SbRWMutex.  TRUE is
  returned if it was successful, FALSE otherwise.  This is a non-blocking
  operation.
*/

/*!
  \fn int SbRWMutex::readUnlock(void)

  This method is used for unlocking the calling thread's read lock.
*/

/* ********************************************************************** */
