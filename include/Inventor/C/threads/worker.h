#ifndef CC_WORKER_H
#define CC_WORKER_H

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

#include <Inventor/C/basic.h>  /* COIN_DLL_API */
#include <Inventor/C/threads/common.h>  /* cc_worker */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  COIN_DLL_API cc_worker * cc_worker_construct(void);
  COIN_DLL_API void cc_worker_destruct(cc_worker * worker);

  COIN_DLL_API SbBool cc_worker_start(cc_worker * worker, 
                                      void (*workfunc)(void *), void * closure);
  COIN_DLL_API SbBool cc_worker_is_busy(cc_worker * worker);
  COIN_DLL_API void cc_worker_wait(cc_worker * worker);
  COIN_DLL_API void cc_worker_set_idle_callback(cc_worker * worker, 
                                                void (*cb)(cc_worker *, void *), void * closure);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_THREAD_H */





