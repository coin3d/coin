#ifndef CC_HASH_H
#define CC_HASH_H

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

#include <stdlib.h>

  typedef struct cc_hash cc_hash;
  typedef unsigned long cc_hash_func(const unsigned long key);
  typedef void cc_hash_apply_func(unsigned long key, void * val, void * closure);

  COIN_DLL_API cc_hash * cc_hash_construct(unsigned int size, float loadfactor);
  COIN_DLL_API void cc_hash_destruct(cc_hash * ht);
  COIN_DLL_API void cc_hash_clear(cc_hash * ht);

  COIN_DLL_API SbBool cc_hash_put(cc_hash * ht, unsigned long key, void * val);
  COIN_DLL_API SbBool cc_hash_get(cc_hash * ht, unsigned long key, void ** val);
  COIN_DLL_API SbBool cc_hash_remove(cc_hash * ht, unsigned long key);
  COIN_DLL_API void cc_hash_apply(cc_hash * ht, cc_hash_apply_func * func, void * closure);

  COIN_DLL_API unsigned int cc_hash_get_num_elements(cc_hash * ht);

  COIN_DLL_API void cc_hash_set_hash_func(cc_hash * ht, cc_hash_func * func);
  COIN_DLL_API void cc_hash_print_stat(cc_hash * ht);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_HASH_H */
