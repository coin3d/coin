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

#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/threads/condvarp.h>

#include <Inventor/C/errors/debugerror.h>

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
#ifdef USE_PTHREAD
#include "condvar_pthread.ic"
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#include "condvar_win32.ic"
#endif /* USE_W32THREAD */

/*
  \internal
*/

void
cc_condvar_struct_init(cc_condvar * condvar_struct)
{
  int ok;
  ok = internal_condvar_struct_init(condvar_struct);
  assert(ok == CC_OK);
}

/*
  \internal
*/
void
cc_condvar_struct_clean(cc_condvar * condvar_struct)
{
  int ok;
  assert(condvar_struct != NULL);
  ok = internal_condvar_struct_clean(condvar_struct);
  assert(ok == CC_OK);
}

/* ********************************************************************** */

/*
*/

cc_condvar *
cc_condvar_construct(void)
{
  cc_condvar * condvar;
  condvar = (cc_condvar *) malloc(sizeof(cc_condvar));
  assert(condvar != NULL);
  cc_condvar_struct_init(condvar);
  return condvar;
}

/*
*/

void
cc_condvar_destruct(cc_condvar * condvar)
{
  assert((condvar != NULL));
  cc_condvar_struct_clean(condvar);
  free(condvar);
}

/*
*/

int
cc_condvar_wait(cc_condvar * condvar, cc_mutex * mutex)
{
  int ok;
  assert(condvar != NULL);
  ok = internal_condvar_wait(condvar, mutex);
  assert(ok == CC_OK);
  return ok;
}

/*
*/

int
cc_condvar_timed_wait(cc_condvar * condvar,
                      cc_mutex * mutex,
                      cc_time period)
{
  int ret;
  assert(condvar != NULL);
  ret = internal_condvar_timed_wait(condvar, mutex, period);
  assert(ret == CC_OK || ret == CC_TIMEOUT);
  return ret;
}

/*
*/

void
cc_condvar_wake_one(cc_condvar * condvar)
{
  int ok;
  assert(condvar != NULL);
  ok = internal_condvar_wake_one(condvar);
  assert(ok == CC_OK);
}

/*
*/
void
cc_condvar_wake_all(cc_condvar * condvar)
{
  int ok;
  assert(condvar != NULL);

  ok = internal_condvar_wake_all(condvar);
  assert(ok == CC_OK);
}

/* ********************************************************************** */

/*!
  \class SbCondVar Inventor/threads/SbCondVar.h
  \ingroup multi-threading

  FIXME: write doc
*/

/*!
  \fn SbCondVar::SbCondVar(void)

  Constructor.
*/

/*!
  \fn SbCondVar::~SbCondVar(void)

  Destructor.
*/

/*!
  \fn SbBool SbCondVar::wait(void)

  This method blocks the calling thread until another thread calls
  wakeOne() or wakeAll() on the SbCondVar object.
*/

/*!
  \fn SbBool SbCondVar::timedWait(SbTime period)

  This method blocks the calling thread until another thread calls
  wakeOne() or wakeAll() on the object, but if this does not happen
  within \c period, the thread is woken again anyways.

  TRUE is returned when the thread wakes because either wakeOne()
  or wakeAll() was called, FALSE is returned if the wait times out.
*/

/*!
  \fn void SbCondVar::wakeOne(void)

  This method wakes up one of the threads waiting on this SbCondVar.
*/

/*!
  \fn void SbCondVar::wakeAll(void)

  This method wakes up all the threads waiting on this SbCondVar.
*/

/* ********************************************************************** */
