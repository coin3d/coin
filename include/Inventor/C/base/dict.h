#ifndef CC_DICT_H
#define CC_DICT_H

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
/* #include <Inventor/C/base/list.h>  - cc_list */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef  struct cc_dict  cc_dict;

/* ********************************************************************** */

COIN_DLL_API cc_dict * cc_dict_construct(void);
COIN_DLL_API cc_dict * cc_dict_construct_sized(unsigned int entries);
COIN_DLL_API void cc_dict_destruct(cc_dict * dict);

COIN_DLL_API void cc_dict_clear(cc_dict * dict);

/* void cc_dict_copy(cc_dict * to, cc_dict * from); */

COIN_DLL_API SbBool cc_dict_enter(cc_dict * dict, unsigned long key, void * value);
COIN_DLL_API SbBool cc_dict_find(cc_dict * dict, unsigned long key, void ** value);
COIN_DLL_API SbBool cc_dict_remove(cc_dict * dict, unsigned long key);
/* void cc_dict_make_lists(cc_dict * dict, cc_list * keys, cc_list * values); */

COIN_DLL_API void cc_dict_set_hash(cc_dict * dict, unsigned long (*func)(unsigned long key));

COIN_DLL_API void cc_dict_apply(cc_dict * dict, void (*func)(unsigned long key, void * val));
COIN_DLL_API void cc_dict_apply_with_closure(cc_dict * dict, void (*func)(unsigned long key, void * val, void * closure), void * closure);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_DICT_H */
