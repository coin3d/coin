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

#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/threads/condvarp.h>

#include <Inventor/C/base/debug.h>

#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif /* HAVE_SYS_TIME_H */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */

/* ********************************************************************** */

/*
  \internal
*/

void
cc_condvar_struct_init(cc_condvar * condvar_struct)
{
  CC_PTHREAD(int status;)
  assert(condvar_struct->type == CC_INVALID_TYPE);

#ifdef USE_PTHREAD
  status = pthread_mutex_init(&(condvar_struct->pthread.mutexid), NULL);
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_init() error: %d\n", status);
    goto error;
  }
  status = pthread_cond_init(&(condvar_struct->pthread.condid), NULL);
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_cond_init() error: %d\n", status);
    goto error;
  }
  /* FIXME: initialize */

#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  condvar_struct->w32thread.eventhandle_one = CreateEvent(NULL, FALSE, FALSE, NULL); /* auto-reset, initially is non-signaled */
  if (condvar_struct->w32thread.eventhandle_one == NULL) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CreateEvent() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
  condvar_struct->w32thread.eventhandle_all = CreateEvent(NULL, TRUE, FALSE, NULL); /* auto-reset, initially is non-signaled */
  if (condvar_struct->w32thread.eventhandle_all == NULL) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "CreateEvent() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#endif /* USE_W32THREAD */

  condvar_struct->type = CC_CONDVAR_TYPE;
  return;

error:
  condvar_struct->type = CC_INVALID_TYPE;
} /* cc_condvar_struct_init() */

/*
  \internal
*/

void
cc_condvar_struct_clean(cc_condvar * condvar_struct)
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  assert((condvar_struct != NULL) && (condvar_struct->type == CC_CONDVAR_TYPE));

#ifdef USE_PTHREAD
  status = pthread_cond_destroy(&(condvar_struct->pthread.condid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_cond_destroy() error: %d\n", status);
    goto error;
  }

  status = pthread_mutex_destroy(&(condvar_struct->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_destroy() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  status = CloseHandle(condvar_struct->w32thread.eventhandle_one);
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
  status = CloseHandle(condvar_struct->w32thread.eventhandle_all);
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
#endif /* USE_W32THREAD */

  condvar_struct->type = CC_INVALID_TYPE;
  return;
error:

  condvar_struct->type = CC_INVALID_TYPE;
} /* cc_condvar_struct_clean() */

/* ********************************************************************** */

/*
*/

cc_condvar *
cc_condvar_construct(void)
{
  cc_condvar * condvar;
  condvar = (cc_condvar *) malloc(sizeof(cc_condvar));
  assert(condvar != NULL);
  condvar->type = CC_INVALID_TYPE;
  cc_condvar_struct_init(condvar);
  if ( condvar->type != CC_CONDVAR_TYPE ) goto error;
  return condvar;

error:
  free(condvar);
  return NULL;
} /* cc_condvar_construct() */

/*
*/

void
cc_condvar_destruct(
  cc_condvar * condvar )
{
  assert((condvar != NULL) && (condvar->type == CC_CONDVAR_TYPE));
  cc_condvar_struct_clean(condvar);
  if ( condvar->type != CC_INVALID_TYPE ) goto error;
  free(condvar);
  return;

error:
  free(condvar);
} /* cc_condvar_destruct() */

/*
*/

int
cc_condvar_wait(
  cc_condvar * condvar )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(DWORD status;)
  CC_W32THREAD(HANDLE eventhandles[2];)
  assert((condvar != NULL) && (condvar->type == CC_CONDVAR_TYPE));

#ifdef USE_PTHREAD
  status = pthread_mutex_lock(&(condvar->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_lock() error: %d\n", status);
    goto error;
  }
  status = pthread_cond_wait(&(condvar->pthread.condid),
                             &(condvar->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_cond_wait() error: %d\n", status);
    goto error;
  }
  status = pthread_mutex_unlock(&(condvar->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  eventhandles[0] = condvar->w32thread.eventhandle_one;
  eventhandles[1] = condvar->w32thread.eventhandle_all;
  status = WaitForMultipleObjects(2, eventhandles, FALSE, INFINITE);
  if (status == WAIT_FAILED) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitForMultipleObjects() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
  else if ( (status != WAIT_OBJECT_0) && (status != WAIT_OBJECT_0+1) ) {
    if ( COIN_DEBUG ) {
      cc_fprintf(stderr, "WaitForSingleObject() - unknown return value: %d\n",
        status);
    }
  }
#endif /* USE_W32THREAD */

  return TRUE;

error:
  return FALSE;
} /* cc_condvar_wait() */

/*
*/

int
cc_condvar_timed_wait(
  cc_condvar * condvar,
  cc_time period )
{
  CC_PTHREAD(struct timespec timeout;)
#ifdef HAVE_GETTIMEOFDAY
  CC_PTHREAD(struct timeval now;)
  CC_PTHREAD(struct timezone zone;)
#else /* HAVE_GETTIMEOFDAY */
  unsigned long int now;
#endif /* ! HAVE_GETTIMEOFDAY */
  CC_PTHREAD(int status;)
  CC_W32THREAD(DWORD status;)
  CC_W32THREAD(HANDLE eventhandles[2];)
  assert((condvar != NULL) && (condvar->type == CC_CONDVAR_TYPE));

#ifdef USE_PTHREAD
  status = pthread_mutex_lock(&(condvar->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_lock() error: %d\n", status);
    goto error;
  }

#ifdef HAVE_GETTIMEOFDAY
  gettimeofday(&now, &zone);
  timeout.tv_sec = now.tv_sec + (int) floor(period);
  timeout.tv_nsec = now.tv_usec + (int) ((period - floor(period)) * 1000000.0f);
#else /* HAVE_GETTIMEODDAY */
  /*FIXME: write a configure macro to test for existence of timeGetTime */
  now = timeGetTime(); /* in milliseconds */
  timeout.tv_sec = (int) floor(now/1000.0) + (int) floor(period);
  timeout.tv_nsec = (now-timeout.tv_sec*1000.0)*1000.0;
    (int) ((period - floor(period)) * 1000000.0f);
#endif /* ! HAVE_GETTIMEOFDAY */
  status = pthread_cond_timedwait(&(condvar->pthread.condid),
                                  &(condvar->pthread.mutexid), &timeout);
  if ( status != 0 ) {
    if ( status != ETIMEDOUT ) {
      if ( COIN_DEBUG ) {
        cc_fprintf_lock();
        cc_fprintf_locked(stderr, "pthread_cond_timedwait() error: %d", status);
        switch ( status ) {
        case EINTR: cc_fprintf_locked(stderr, "EINTR\n"); break;
        case EBUSY: cc_fprintf_locked(stderr, "EBUSY\n"); break;
        default: cc_fprintf_locked(stderr, "default\n"); break;
        }
        cc_fprintf_unlock();
      }
    }
    status = pthread_mutex_unlock(&(condvar->pthread.mutexid));
    if ( status != 0 ) {
      if ( COIN_DEBUG )
        cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
      /* goto error; */
    }
    goto error;
  }
  status = pthread_mutex_unlock(&(condvar->pthread.mutexid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_mutex_unlock() error: %d\n", status);
    goto error;
  }

  return TRUE;
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  eventhandles[0] = condvar->w32thread.eventhandle_one;
  eventhandles[1] = condvar->w32thread.eventhandle_all;
  status = WaitForMultipleObjects(2, eventhandles, FALSE, (DWORD)floor(period*1000.0f));
  if (status == WAIT_FAILED) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "WaitForMultipleObjects() error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
  else if ( (status == WAIT_OBJECT_0) || (status == WAIT_OBJECT_0+1) ) {
    return CC_OK;
  }
  else if (status == WAIT_TIMEOUT) {
    return CC_TIMEOUT;
  }
  else {
    if ( COIN_DEBUG ) {
      cc_fprintf(stderr, "WaitForSingleObject() - unknown return value: %d\n",
        status);
    }
    return CC_OK;
  }
#endif /* USE_W32THREAD */

error:
  return CC_ERROR;
} /* cc_condvar_timed_wait() */

/*
*/

void
cc_condvar_wake_one(
  cc_condvar * condvar )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  /* FIXME: should return that thread was actually woken? */
  assert((condvar != NULL) && (condvar->type == CC_CONDVAR_TYPE));

#ifdef USE_PTHREAD
  status = pthread_cond_signal(&(condvar->pthread.condid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_cond_signal() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  status = PulseEvent(condvar->w32thread.eventhandle_one);
  /* the event is auto-reset */
  if (status == FALSE) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "SetEvent error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#endif /* USE_W32THREAD */

  return;
error:
  ;
} /* cc_condvar_wake_one() */

/*
*/

void
cc_condvar_wake_all(
  cc_condvar * condvar )
{
  CC_PTHREAD(int status;)
  CC_W32THREAD(BOOL status;)
  /* FIXME: should return that thread was actually woken? */
  assert((condvar != NULL) && (condvar->type == CC_CONDVAR_TYPE));

#ifdef USE_PTHREAD
  status = pthread_cond_broadcast(&(condvar->pthread.condid));
  if ( status != 0 ) {
    if ( COIN_DEBUG )
      cc_fprintf(stderr, "pthread_cond_broadcast() error: %d\n", status);
    goto error;
  }
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  status = PulseEvent(condvar->w32thread.eventhandle_all);
  /* event is manually-reset, but PulseEvent will reset when all
     waiting threads have been released */
  if (status == FALSE) {
    if ( COIN_DEBUG ) {
      DWORD err;
      char *errstr;
      err = GetLastError();
      errstr = cc_internal_w32_getlasterrorstring(err);
      cc_fprintf(stderr, "SetEvent error: %d, \"%s\"\n",
        err, errstr);
      cc_internal_w32_freelasterrorstring(errstr);
    }
    goto error;
  }
#endif /* USE_W32THREAD */

  return;
error:
  ;
} /* cc_condvar_wake_all() */

/* ********************************************************************** */

/*!
  \class SbCondVar Inventor/threads/SbCondVar.h
  \ingroup multi-threading

  FIXME: write doc
*/

/*!
  \fn SbCondVar::SbCondVar(void)

  FIXME: write doc
*/

/*!
  \fn SbCondVar::~SbCondVar(void)

  FIXME: write doc
*/

/*!
  \fn SbBool SbCondVar::wait(void)

  FIXME: write doc
*/

/*!
  \fn SbBool SbCondVar::timedWait(SbTime period)

  FIXME: write doc
*/

/*!
  \fn void SbCondVar::wakeOne(void)

  FIXME: write doc
*/

/*!
  \fn void SbCondVar::wakeAll(void)

  FIXME: write doc
*/

/* ********************************************************************** */
