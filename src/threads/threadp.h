#ifndef CC_THREADP_H
#define CC_THREADP_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/threads/common.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#define NO_IMPLEMENTATION

#ifdef USE_PTHREAD
#include <pthread.h>
#undef NO_IMPLEMENTATION
#ifdef HAVE_SCHED_H
#include <sched.h>
#endif /* HAVE_SCHED_H */
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#include <windows.h>
#undef NO_IMPLEMENTATION
#endif /* USE_W32THREAD */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct cc_thread {
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

void cc_thread_init(void);
void cc_thread_struct_init(cc_thread * thread_struct);
void cc_thread_struct_clean(cc_thread * thread_struct);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_THREADP_H */
