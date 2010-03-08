#ifndef CC_MEMALLOC_H
#define CC_MEMALLOC_H

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

#include <Inventor/C/basic.h> /* COIN_DLL_API */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef struct cc_memalloc cc_memalloc;
  typedef int cc_memalloc_strategy_cb(const int numunits_allocated);

/* ********************************************************************** */
  
  COIN_DLL_API cc_memalloc * cc_memalloc_construct(const unsigned int unitsize);
  COIN_DLL_API void cc_memalloc_destruct(cc_memalloc * allocator);
  COIN_DLL_API void * cc_memalloc_allocate(cc_memalloc * allocator);
  COIN_DLL_API void cc_memalloc_deallocate(cc_memalloc * allocator, void * ptr);
  COIN_DLL_API void cc_memalloc_clear(cc_memalloc * allocator);
  COIN_DLL_API void cc_memalloc_set_strategy(cc_memalloc * allocator, cc_memalloc_strategy_cb * cb);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_MEMALLOC_H */
