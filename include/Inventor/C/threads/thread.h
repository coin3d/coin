#ifndef CC_THREAD_H
#define CC_THREAD_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/C/basic.h>  /* COIN_DLL_API */
#include <Inventor/C/threads/common.h>  /* cc_thread */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

COIN_DLL_API cc_thread * cc_thread_construct(void * (*func)(void *), void * closure);
COIN_DLL_API void cc_thread_destruct(cc_thread * thread);

COIN_DLL_API int cc_thread_join(cc_thread * thread, void ** retvalptr);

COIN_DLL_API unsigned long cc_thread_id(void);
COIN_DLL_API void cc_sleep(float seconds);

/*
COIN_DLL_API int cc_thread_priority_set(cc_thread * thread, int value);
COIN_DLL_API int cc_thread_priority_change(cc_thread * thread, int change);
COIN_DLL_API int cc_thread_priority_get(cc_thread * thread);
*/

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_THREAD_H */
