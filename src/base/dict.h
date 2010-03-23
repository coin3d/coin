#ifndef CC_DICT_H
#define CC_DICT_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // COIN_INTERNAL

#include <cstdlib>
#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef struct cc_dict cc_dict;
  typedef uintptr_t cc_dict_hash_func(const uintptr_t key);
  typedef void cc_dict_apply_func(uintptr_t key, void * val, void * closure);

  cc_dict * cc_dict_construct(unsigned int size, float loadfactor);
  void cc_dict_destruct(cc_dict * ht);
  void cc_dict_clear(cc_dict * ht);

  SbBool cc_dict_put(cc_dict * ht, uintptr_t key, void * val);
  SbBool cc_dict_get(cc_dict * ht, uintptr_t key, void ** val);
  SbBool cc_dict_remove(cc_dict * ht, uintptr_t key);
  void cc_dict_apply(cc_dict * ht, cc_dict_apply_func * func, void * closure);

  unsigned int cc_dict_get_num_elements(cc_dict * ht);

  void cc_dict_set_hash_func(cc_dict * ht, cc_dict_hash_func * func);
  void cc_dict_print_stat(cc_dict * ht);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_DICT_H */
