#ifndef CC_WORKER_H
#define CC_WORKER_H

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
#include <Inventor/C/threads/common.h>  /* cc_worker */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef void cc_worker_f(void *);
typedef void cc_worker_idle_f(cc_worker *, void *);

  COIN_DLL_API cc_worker * cc_worker_construct(void);
  COIN_DLL_API void cc_worker_destruct(cc_worker * worker);

  COIN_DLL_API SbBool cc_worker_start(cc_worker * worker, 
                                      cc_worker_f * workfunc, void * closure);
  COIN_DLL_API SbBool cc_worker_is_busy(cc_worker * worker);
  COIN_DLL_API void cc_worker_wait(cc_worker * worker);
  COIN_DLL_API void cc_worker_set_idle_callback(cc_worker * worker, 
                                                cc_worker_idle_f * idlefunc, void * closure);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_WORKER_H */
