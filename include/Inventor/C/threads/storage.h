#ifndef CC_STORAGE_H
#define CC_STORAGE_H

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
#include <Inventor/C/threads/common.h>  /* cc_storage */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 // for emacs indentation
}
#endif // for emacs indentation

/* ********************************************************************** */
typedef void cc_storage_apply_func(void * dataptr, void * closure);
COIN_DLL_API cc_storage * cc_storage_construct(unsigned int size);
COIN_DLL_API cc_storage * cc_storage_construct_etc(unsigned int size, 
                                                   void (*constructor)(void *), 
                                                   void (*destructor)(void *));
COIN_DLL_API void cc_storage_destruct(cc_storage * storage);

COIN_DLL_API void * cc_storage_get(cc_storage * storage);
COIN_DLL_API void cc_storage_apply_to_all(cc_storage * storage, 
                                          cc_storage_apply_func * func, 
                                          void * closure);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_STORAGE_H */
