#ifndef CC_MEMALLOC_H
#define CC_MEMALLOC_H

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
