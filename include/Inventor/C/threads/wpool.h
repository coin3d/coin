#ifndef CC_WPOOL_H
#define CC_WPOOL_H

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
#include <Inventor/C/threads/common.h>  /* cc_worker_pool */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  COIN_DLL_API cc_wpool * cc_wpool_construct(int numworkers);
  COIN_DLL_API void cc_wpool_destruct(cc_wpool * pool);

  COIN_DLL_API int cc_wpool_get_num_workers(cc_wpool * pool);
  COIN_DLL_API void cc_wpool_set_num_workers(cc_wpool * pool, int newnum);
  COIN_DLL_API void cc_wpool_wait_all(cc_wpool * pool);

  COIN_DLL_API SbBool cc_wpool_try_begin(cc_wpool * pool, int numworkersneeded);
  COIN_DLL_API void cc_wpool_begin(cc_wpool * pool, int numworkersneeded);
  COIN_DLL_API void cc_wpool_start_worker(cc_wpool * pool,
                                          void (*workfunc)(void *), void * closure);
  COIN_DLL_API void cc_wpool_end(cc_wpool * pool);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_WPOOL_H */
