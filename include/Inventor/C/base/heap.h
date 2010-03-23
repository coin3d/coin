#ifndef CC_HEAP_H
#define CC_HEAP_H

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

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef int cc_heap_compare_cb(void * o1, void * o2);

  typedef struct cc_heap cc_heap;

  COIN_DLL_API cc_heap * cc_heap_construct(unsigned int size,
                                           cc_heap_compare_cb * comparecb,
                                           SbBool support_remove);
  
  COIN_DLL_API void cc_heap_destruct(cc_heap * h);
  COIN_DLL_API void cc_heap_clear(cc_heap * h);
  
  COIN_DLL_API void cc_heap_add(cc_heap * h, void * o);
  COIN_DLL_API void * cc_heap_get_top(cc_heap * h);
  COIN_DLL_API void * cc_heap_extract_top(cc_heap * h);
  COIN_DLL_API int cc_heap_remove(cc_heap * h, void * o);
  COIN_DLL_API unsigned int cc_heap_elements(cc_heap * h);
  COIN_DLL_API SbBool cc_heap_empty(cc_heap * h);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_HEAP_H */
