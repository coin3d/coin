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

#if HAVE_CONFIG_H
#include <config.h>
#endif
#include <Inventor/C/base/debug.h>
#include <Inventor/C/threads/mutex.h>
#include <stdarg.h>
#include <assert.h>

static cc_mutex * mutex = NULL;

/* ********************************************************************** */

int
cc_fprintf(
  FILE * stream,
  const char * format,
  ...)
{
  int retval;
  va_list args;
#ifdef HAVE_THREADS
  if ( ! mutex ) mutex = cc_mutex_construct();
#endif
  va_start(args, format);
#ifdef HAVE_THREADS
  cc_mutex_lock(mutex);
#endif
  retval = vfprintf(stream, format, args);
#ifdef HAVE_THREADS
  cc_mutex_unlock(mutex);
#endif
  va_end(args);
  return retval;
} /* cc_fprintf() */

/* ********************************************************************** */

#ifdef HAVE_THREADS

void
cc_fprintf_lock(void)
{
  if ( ! mutex ) mutex = cc_mutex_construct();
  cc_mutex_lock(mutex);
} /* cc_fprintf_lock() */

void
cc_fprintf_unlock(void)
{
  assert(mutex != NULL);
  cc_mutex_unlock(mutex);
} /* cc_fprintf_unlock() */

#else /* HAVE_THREADS */

void
cc_fprintf_lock(void)
{
}

void
cc_fprintf_unlock(void)
{
}

#endif /* !HAVE_THREADS */

int
cc_fprintf_locked(
  FILE * stream,
  const char * format,
  ...)
{
  int retval;
  va_list args;
  va_start(args, format);
  retval = vfprintf(stream, format, args);
  va_end(args);
  return retval;
} /* cc_fprintf_locked() */

/* ********************************************************************** */
