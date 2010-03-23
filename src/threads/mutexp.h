#ifndef CC_MUTEXP_H
#define CC_MUTEXP_H

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#define NO_IMPLEMENTATION
#ifdef USE_PTHREAD
#include <pthread.h>
#undef NO_IMPLEMENTATION
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#include <windows.h>
#undef NO_IMPLEMENTATION
#endif /* USE_W32THREAD */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct cc_mutex {
#ifdef USE_PTHREAD
  struct cc_pthread_mutex_data {
    pthread_mutex_t mutexid;
  } pthread;
#endif /* USE_PTHREAD */
#ifdef USE_W32THREAD
  union cc_w32thread_mutex_data {
    HANDLE mutexhandle;
    CRITICAL_SECTION critical_section;
  } w32thread;
#endif /* USE_W32THREAD */
};

#ifdef NO_IMPLEMENTATION
#error missing threads implementation support
#endif /* ! NO_IMPLEMENTATION */

/* ********************************************************************** */

void cc_mutex_init(void);
void cc_mutex_global_lock(void);
void cc_mutex_global_unlock(void);
void cc_mutex_struct_init(cc_mutex * mutex_struct);
void cc_mutex_struct_clean(cc_mutex * mutex_struct);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_MUTEXP_H */
