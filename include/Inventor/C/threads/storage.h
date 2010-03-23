#ifndef CC_STORAGE_H
#define CC_STORAGE_H

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
#include <Inventor/C/threads/common.h>  /* cc_storage */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* for emacs indentation */
}
#endif /* for emacs indentation */

/* ********************************************************************** */
typedef void cc_storage_f(void * closure);
typedef void cc_storage_apply_func(void * dataptr, void * closure);
COIN_DLL_API cc_storage * cc_storage_construct(unsigned int size);
COIN_DLL_API cc_storage * cc_storage_construct_etc(unsigned int size, 
                                                   cc_storage_f * constructor,
                                                   cc_storage_f * destructor);
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
