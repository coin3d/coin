#ifndef CC_LIST_H
#define CC_LIST_H

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

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef  struct cc_list  cc_list;

COIN_DLL_API cc_list * cc_list_construct(void);
COIN_DLL_API cc_list * cc_list_construct_sized(unsigned int size);
/* cc_list * cc_list_clone(cc_list * list); */
COIN_DLL_API void cc_list_destruct(cc_list * list);

COIN_DLL_API void cc_list_append(cc_list * list, void * item);
COIN_DLL_API int cc_list_find(cc_list * list, void * item);
COIN_DLL_API void cc_list_insert(cc_list * list, void * item, unsigned int pos);
COIN_DLL_API void cc_list_remove(cc_list * list, unsigned int pos);
COIN_DLL_API void cc_list_remove_item(cc_list * list, void * item);
COIN_DLL_API void cc_list_remove_fast(cc_list * list, unsigned int pos);
COIN_DLL_API void cc_list_fit(cc_list * list);
COIN_DLL_API void cc_list_truncate(cc_list * list, unsigned int length);
COIN_DLL_API void cc_list_truncate_fit(cc_list * list, unsigned int length);

COIN_DLL_API int cc_list_get_length(cc_list * list);
COIN_DLL_API void ** cc_list_get_array(cc_list * list);

COIN_DLL_API void cc_list_push(cc_list * list, void * item);
COIN_DLL_API void * cc_list_pop(cc_list * list);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_LIST_H */
