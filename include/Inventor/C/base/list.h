#ifndef CC_LIST_H
#define CC_LIST_H

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

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef struct cc_list cc_list;

COIN_DLL_API cc_list * cc_list_construct(void);
COIN_DLL_API cc_list * cc_list_construct_sized(int size);
COIN_DLL_API cc_list * cc_list_clone(cc_list * list);
COIN_DLL_API void cc_list_destruct(cc_list * list);

COIN_DLL_API void cc_list_append(cc_list * list, void * item);
COIN_DLL_API int cc_list_find(cc_list * list, void * item);
COIN_DLL_API void cc_list_insert(cc_list * list, void * item, int pos);
COIN_DLL_API void cc_list_remove(cc_list * list, int pos);
COIN_DLL_API void cc_list_remove_item(cc_list * list, void * item);
COIN_DLL_API void cc_list_remove_fast(cc_list * list, int pos);
COIN_DLL_API void cc_list_fit(cc_list * list);
COIN_DLL_API void cc_list_truncate(cc_list * list, int length);
COIN_DLL_API void cc_list_truncate_fit(cc_list * list, int length);

COIN_DLL_API int cc_list_get_length(cc_list * list);
COIN_DLL_API void ** cc_list_get_array(cc_list * list);
COIN_DLL_API void * cc_list_get(cc_list * list, int itempos);

COIN_DLL_API void cc_list_push(cc_list * list, void * item);
COIN_DLL_API void * cc_list_pop(cc_list * list);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_LIST_H */
