#ifndef CC_THREADP_H
#define CC_THREADP_H

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

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/threads/common.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* macro to wrap one-line thread-implementation-specific statements in */
#define CC_PTHREAD(stmt)   /* nop */
#define CC_W32THREAD(stmt) /* nop */

#ifdef USE_PTHREAD
#include <pthread.h>
#ifdef HAVE_SCHED_H
#include <sched.h>
#endif /* HAVE_SCHED_H */
#undef CC_PTHREAD
#define CC_PTHREAD(stmt) stmt
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD

#include <windows.h>

#undef CC_W32THREAD
#define CC_W32THREAD(stmt) stmt

#endif /* USE_W32THREAD */


struct cc_thread {
  unsigned int type;
  void * (*func)(void *);
  void * closure;

#ifdef USE_PTHREAD
  struct cc_pthread_data {
    pthread_t threadid;
    pthread_attr_t threadattrs;
  } pthread;
#undef NO_IMPLEMENTATION
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
  struct cc_w32thread_data {
    HANDLE threadhandle;
  } w32thread;
#undef NO_IMPLEMENTATION
#endif /* USE_W32THREAD */
};

#ifdef NO_IMPLEMENTATION
#error missing threads implementation support
#endif /* NO_IMPLEMENTATION */

/* ********************************************************************** */

void cc_thread_struct_init(cc_thread * thread_struct);
void cc_thread_struct_clean(cc_thread * thread_struct);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_THREADP_H */
