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

#include <Coin/threads/rwmutex.h>
#include <Coin/threads/rwmutexp.h>

#include <Coin/base/debug.h>
#include <Coin/threads/mutex.h>
#include <Coin/threads/condvar.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

/*!
  \internal
*/
void
cc_rwmutex_struct_init(cc_rwmutex * rwmutex)
{
  assert(rwmutex->type == CC_INVALID_TYPE);
  
  rwmutex->mutex.type = CC_INVALID_TYPE;
  cc_mutex_struct_init(&rwmutex->mutex);
  rwmutex->read.type = CC_INVALID_TYPE;
  cc_condvar_struct_init(&rwmutex->read);
  rwmutex->write.type = CC_INVALID_TYPE;
  cc_condvar_struct_init(&rwmutex->write);

  rwmutex->readers = 0;
  rwmutex->readwaiters = 0;
  rwmutex->writers = 0;
  rwmutex->writewaiters = 0;
  rwmutex->policy = CC_READ_PRECEDENCE;
  rwmutex->type = CC_RWMUTEX_TYPE;
}

/*!
  \internal
*/
void 
cc_rwmutex_struct_clean(cc_rwmutex * rwmutex)
{
  assert(rwmutex->type == CC_RWMUTEX_TYPE);
  cc_mutex_struct_clean(&rwmutex->mutex);
  cc_condvar_struct_clean(&rwmutex->read);
  cc_condvar_struct_clean(&rwmutex->write);
  rwmutex->type = CC_INVALID_TYPE;
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
  rwmutex->type = CC_INVALID_TYPE;
  cc_rwmutex_struct_init(rwmutex);
  return rwmutex;
} /* cc_rwmutex_construct() */

/*
  Construct a read-write mutex, with read precedence or write precedence.
*/

cc_rwmutex *
cc_rwmutex_construct_etc(enum cc_precedence policy)
{
  cc_rwmutex * rwmutex;
  assert((policy == CC_READ_PRECEDENCE) || (policy == CC_WRITE_PRECEDENCE));
  rwmutex = cc_rwmutex_construct();
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));
  rwmutex->policy = policy;
  return rwmutex;
} /* cc_rwmutex_construct() */

/*
  Destruct a read-write mutex.
*/

void
cc_rwmutex_destruct(cc_rwmutex * rwmutex)
{
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));

  cc_mutex_struct_clean(&(rwmutex->mutex));
  cc_condvar_struct_clean(&(rwmutex->read));
  cc_condvar_struct_clean(&(rwmutex->write));
  rwmutex->type = CC_INVALID_TYPE;
  free(rwmutex);
} /* cc_rwmutex_destruct() */

/* ********************************************************************** */

#if 0
static void
cc_rwmutex_write_lock_cleanup(void * closure)
{
  int status;
  cc_rwmutex * rwmutex;
  rwmutex = (cc_rwmutex *) closure;
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));
  rwmutex->writewaiters--;
  status = pthread_mutex_unlock(&(rwmutex->pthread.mutex));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
  }
} /* cc_rwmutex_write_lock_cleanup() */

#endif

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
    return 1;
  }
  rwmutex->writewaiters++;
  (void) cc_mutex_unlock(&rwmutex->mutex);
  (void) cc_condvar_wait(&rwmutex->write);
  (void) cc_mutex_lock(&rwmutex->mutex);
  assert(rwmutex->readers == 0 && rwmutex->writers == 0);
  rwmutex->writers++;
  rwmutex->writewaiters--;
  assert(rwmutex->writewaiters >= 0);
  cc_mutex_unlock(&rwmutex->mutex);
  return 1;
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
    return 1;
  }
  cc_mutex_unlock(&rwmutex->mutex);
  return 0;
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
  } else {
    if (wwait) cc_condvar_wake_one(&rwmutex->write);
    else cc_condvar_wake_all(&rwmutex->read);
  }
  return 0;
} /* cc_rwmutex_write_unlock() */

/*
*/

#if 0
static
void
cc_rwmutex_read_lock_cleanup(
  void * closure)
{
  int status;
  cc_rwmutex * rwmutex;
  rwmutex = (cc_rwmutex *) closure;
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));
  rwmutex->readwaiters--;
  status = pthread_mutex_unlock(&(rwmutex->pthread.mutex));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
  }
} /* cc_rwmutex_read_lock_cleanup() */

#endif
/*
*/

int
cc_rwmutex_read_lock(cc_rwmutex * rwmutex)
{
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));
  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->writers == 0) {
    rwmutex->readers++;
    (void) cc_mutex_unlock(&rwmutex->mutex);
    return 1;
  }
  rwmutex->readwaiters++;
  (void) cc_mutex_unlock(&rwmutex->mutex);
  (void) cc_condvar_wait(&rwmutex->read);
  (void) cc_mutex_lock(&rwmutex->mutex);
  assert(rwmutex->writers == 0);
  rwmutex->readers++;
  rwmutex->readwaiters--;
  assert(rwmutex->readwaiters >= 0);
  cc_mutex_unlock(&rwmutex->mutex);
  return 1;
} /* cc_rwmutex_read_lock() */

/*
*/

int
cc_rwmutex_read_try_lock(cc_rwmutex * rwmutex)
{
  assert((rwmutex != NULL) && (rwmutex->type == CC_RWMUTEX_TYPE));

  (void) cc_mutex_lock(&rwmutex->mutex);
  if (rwmutex->writers == 0 &&
      rwmutex->writewaiters == 0) {
    rwmutex->readers++;
    (void) cc_mutex_unlock(&rwmutex->mutex);
    return 1;
  }
  (void) cc_mutex_unlock(&rwmutex->mutex);
  return 0;
} /* cc_rwmutex_read_try_lock() */

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
  return 0;
} /* cc_rwmutex_read_unlock() */

/* ********************************************************************** */

/*!
  \class SbRWMutex Inventor/threads/SbRWMutex.h
  \ingroup multi-threading
*/

/*!
  \fn SbRWMutex::SbRWMutex(void)
*/

/*!
  \fn SbRWMutex::~SbRWMutex(void)
*/

/*!
  \fn int SbRWMutex::writeLock(void)
*/

/*!
  \fn SbBool SbRWMutex::tryWriteLock(void)
*/

/*!
  \fn int SbRWMutex::writeUnlock(void)
*/

/*!
  \fn int SbRWMutex::readLock(void)
*/

/*!
  \fn SbBool SbRWMutex::tryReadLock(void)
*/

/*!
  \fn int SbRWMutex::readUnlock(void)
*/

/* ********************************************************************** */
