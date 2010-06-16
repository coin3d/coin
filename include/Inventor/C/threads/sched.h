#ifndef CC_SCHED_H
#define CC_SCHED_H

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

#include <Inventor/C/basic.h>  /* COIN_DLL_API */
#include <Inventor/C/threads/common.h>  /* cc_sched */

/* ********************************************************************** */

/* Implementation note: it is important that this header file can be
   included even when Coin was built with no threads support.

   (This simplifies client code, as we get away with far less #ifdef
   HAVE_THREADS wrapping.) */

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void cc_sched_f(void * closure);

/* ********************************************************************** */

  COIN_DLL_API cc_sched * cc_sched_construct(int numthreads);
  COIN_DLL_API void cc_sched_destruct(cc_sched * sched);
  COIN_DLL_API void cc_sched_set_num_threads(cc_sched * sched, int num);
  COIN_DLL_API int cc_sched_get_num_threads(cc_sched * sched);

  COIN_DLL_API uint32_t cc_sched_schedule(cc_sched * sched, 
                                          cc_sched_f * workfunc, 
                                          void * closure,
                                          float priority);
  COIN_DLL_API SbBool cc_sched_unschedule(cc_sched * sched, 
                                          uint32_t schedid); 
  COIN_DLL_API void cc_sched_wait_all(cc_sched * sched);
  COIN_DLL_API int cc_sched_get_num_remaining(cc_sched * sched);
  COIN_DLL_API void cc_sched_set_num_allowed(cc_sched * sched, int num);
  COIN_DLL_API void cc_sched_change_priority(cc_sched * sched, 
                                             uint32_t schedid, 
                                             float priority);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_SCHED_H */
