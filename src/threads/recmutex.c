/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/C/threads/recmutex.h>
#include <Inventor/C/threads/recmutexp.h>

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/condvar.h>
#include <Inventor/C/tidbitsp.h>

#include <stdlib.h>
#include <assert.h>

/* ********************************************************************** */

/*!
  \internal
*/
void
cc_recmutex_struct_init(cc_recmutex * recmutex)
{
  cc_mutex_struct_init(&recmutex->mutex);
  cc_condvar_struct_init(&recmutex->condvar);
  
  recmutex->threadid = 0;
  recmutex->level = 0;
  recmutex->waiters = 0;
}

/*!
  \internal
*/
void
cc_recmutex_struct_clean(cc_recmutex * recmutex)
{
  cc_mutex_struct_clean(&recmutex->mutex);
  cc_condvar_struct_clean(&recmutex->condvar);
}

/*
  Construct a recursive mutex.
*/

cc_recmutex *
cc_recmutex_construct(void)
{
  cc_recmutex * recmutex;
  recmutex = (cc_recmutex *) malloc(sizeof(cc_recmutex));
  assert(recmutex != NULL);
  cc_recmutex_struct_init(recmutex);
  return recmutex;
}


/*
  Destruct a recursive mutex.
*/

void
cc_recmutex_destruct(cc_recmutex * recmutex)
{
  assert(recmutex != NULL);
  cc_recmutex_struct_clean(recmutex);
  free(recmutex);
}


/*
  Lock a recursive mutex. Returns the nesting level.
*/

int
cc_recmutex_lock(cc_recmutex * recmutex)
{
  int level;
  unsigned long id = cc_thread_id();

  assert(recmutex != NULL);
  cc_mutex_lock(&recmutex->mutex);
  if (recmutex->level == 0) {
    recmutex->level++;
    recmutex->threadid = id;
  }
  else if (id == recmutex->threadid) {
    recmutex->level++;
  }
  else {
    recmutex->waiters++;
    cc_condvar_wait(&recmutex->condvar, &recmutex->mutex);
    assert(recmutex->level == 0);
    recmutex->waiters--;
    recmutex->threadid = id;
    recmutex->level++;
  }
  level = recmutex->level;
  cc_mutex_unlock(&recmutex->mutex);
  return level;
}

/*
  Unlock a recursive mutex. Returns the nesting level after unlock.
*/

int
cc_recmutex_unlock(cc_recmutex * recmutex)
{
  int level;
  assert(recmutex != NULL);
  assert(recmutex->threadid == cc_thread_id());
  assert(recmutex->level > 0);
  cc_mutex_lock(&recmutex->mutex);
  recmutex->level--;
  if (recmutex->level == 0 && recmutex->waiters) {
    cc_condvar_wake_one(&recmutex->condvar);
  }
  level = recmutex->level;
  cc_mutex_unlock(&recmutex->mutex);
  return level;
}

/*
  internal functions
*/

static cc_recmutex * recmutex_field_lock;

static void
recmutex_cleanup(void)
{
  cc_recmutex_destruct(recmutex_field_lock);
}

void 
cc_recmutex_init(void)
{
  recmutex_field_lock = cc_recmutex_construct();
  coin_atexit((coin_atexit_f*) recmutex_cleanup, 0);
}

int 
cc_recmutex_internal_field_lock(void)
{
  return cc_recmutex_lock(recmutex_field_lock);
}

int 
cc_recmutex_internal_field_unlock(void)
{
  return cc_recmutex_unlock(recmutex_field_lock);
}

