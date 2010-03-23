#ifndef COIN_DYNARRAY_H
#define COIN_DYNARRAY_H

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

/* This is the interface for a C ADT of a dynamic array. */

/*************************************************************************/

#ifndef COIN_INTERNAL
#error Only for internal use.
#endif /* COIN_INTERNAL */

/*************************************************************************/

#include <Inventor/C/basic.h>

/*************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct cc_dynarray cc_dynarray;

  cc_dynarray * cc_dynarray_new(void);
  cc_dynarray * cc_dynarray_duplicate(const cc_dynarray * src);
  void cc_dynarray_destruct(cc_dynarray * arr);

  unsigned int cc_dynarray_length(const cc_dynarray * arr);

  void cc_dynarray_append(cc_dynarray * arr, void * item);
  void cc_dynarray_insert(cc_dynarray * arr, void * item, unsigned int idx);

  int cc_dynarray_find(const cc_dynarray * arr, void * item);

  void * cc_dynarray_get(const cc_dynarray * arr, unsigned int idx);
  void cc_dynarray_set(cc_dynarray * arr, unsigned int idx, void * item);

  void cc_dynarray_remove(cc_dynarray * arr, void * item);
  void cc_dynarray_remove_idx(cc_dynarray * arr, unsigned int idx);
  void cc_dynarray_removefast(cc_dynarray * arr, unsigned int idx);

  void cc_dynarray_truncate(cc_dynarray * arr, unsigned int len);
  void cc_dynarray_fit(cc_dynarray * arr);

  void ** cc_dynarray_get_arrayptr(const cc_dynarray * arr);

  SbBool cc_dynarray_eq(const cc_dynarray * arr1, const cc_dynarray * arr2);
  
#ifdef __cplusplus
}
#endif

#endif /* !COIN_DYNARRAY_H */
