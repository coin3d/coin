#ifndef CC_SCHED_H
#define CC_SCHED_H

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

#include <Inventor/C/basic.h>  /* COIN_DLL_API */
#include <Inventor/C/threads/common.h>  /* cc_sched */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  COIN_DLL_API cc_sched * cc_sched_construct(int numthreads);
  COIN_DLL_API void cc_sched_destruct(cc_sched * sched);
  COIN_DLL_API void cc_sched_set_num_threads(cc_sched * sched, int num);
  COIN_DLL_API int cc_sched_get_num_threads(cc_sched * sched);

  COIN_DLL_API void cc_sched_schedule(cc_sched * sched, 
                                      void (*workfunc)(void *), void * closure,
                                      unsigned int priority);
  COIN_DLL_API void cc_sched_wait_all(cc_sched * sched);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_SCHED_H */
