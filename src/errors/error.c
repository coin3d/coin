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

#include <Inventor/C/errors/error.h>
#include <stdio.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_THREADS
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/mutexp.h>
#endif /* HAVE_THREADS */

#ifdef HAVE_UNISTD_H
#include <unistd.h> /* STDERR_FILENO */
#endif /* HAVE_UNISTD_H */

#include "../tidbitsp.h"

static cc_mutex * cc_error_mutex = NULL;



/* FIXME: should be hidden from public API, and only visible to
   subclasses. 20020526 mortene. */
void
cc_error_default_handler_cb(const cc_error * err, void * data)
{
  /* It is not possible to "pass" C library data from the application
     to a MSWin .DLL, so this is necessary to get hold of the stderr
     FILE*. Just using fprintf(stderr, ...) or fprintf(stdout, ...)
     directly will result in a crash when Coin has been compiled as a
     .DLL. */
  static FILE * coin_stderr = NULL;
  if (!coin_stderr) { coin_stderr = coin_get_stderr(); }

  if (coin_stderr) {
    const cc_string * str = cc_error_get_debug_string(err);
    (void)fprintf(coin_stderr, "%s\n", cc_string_get_text(str));
    (void)fflush(coin_stderr);
  }
}

static cc_error_cb * cc_error_callback = cc_error_default_handler_cb;
static void * cc_error_callback_data = NULL;


void
cc_error_init(cc_error * me)
{
  cc_string_construct(&(me->debugstring));
}

void
cc_error_clean(cc_error * me)
{
  cc_string_clean(&(me->debugstring));
}

void
cc_error_copy(const cc_error * src, cc_error * dst)
{
  cc_string_set_string(&dst->debugstring, &src->debugstring);
}

void
cc_error_set_debug_string(cc_error * me, const char * str)
{
  cc_string_set_text(&(me->debugstring), str);
}

void
cc_error_append_to_debug_string(cc_error * me, const char * str)
{
  cc_string_append_text(&(me->debugstring), str);
}

void
cc_error_handle(cc_error * me)
{
  void * arg = NULL;

  cc_error_cb * function = cc_error_get_handler(&arg);
  assert(function != NULL);

#ifdef HAVE_THREADS
  if (!cc_error_mutex) {
    /* extra locking to avoid that two threads create the mutex */
    cc_mutex_global_lock();
    if (cc_error_mutex == NULL) {
      cc_error_mutex = cc_mutex_construct();
    }
    cc_mutex_global_unlock();
  }
  cc_mutex_lock(cc_error_mutex);
#endif /* HAVE_THREADS */

  (*function)(me, arg);

#ifdef HAVE_THREADS
  cc_mutex_unlock(cc_error_mutex);
#endif /* HAVE_THREADS */
}

void
cc_error_set_handler_callback(cc_error_cb * func, void * data)
{
  cc_error_callback = func;
  cc_error_callback_data = data;
}

cc_error_cb *
cc_error_get_handler_callback(void)
{
  return cc_error_callback;
}

void *
cc_error_get_handler_data(void)
{
  return cc_error_callback_data;
}

cc_error_cb *
cc_error_get_handler(void ** data)
{
  *data = cc_error_callback_data;
  return cc_error_callback;
}

const cc_string *
cc_error_get_debug_string(const cc_error * me)
{
  return &(me->debugstring);
}

void
cc_error_post(const char * format, ...)
{
  va_list args;
  cc_string s;
  cc_error err;

  cc_string_construct(&s);

  va_start(args, format);
  cc_string_vsprintf(&s, format, args);
  va_end(args);

  cc_error_init(&err);
  cc_error_set_debug_string(&err, cc_string_get_text(&s));
  cc_error_handle(&err);
  cc_error_clean(&err);

  cc_string_clean(&s);
}
