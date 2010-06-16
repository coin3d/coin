#ifndef CC_FIFO_H
#define CC_FIFO_H

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
#include <Inventor/C/threads/common.h>  /* cc_fifo */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

COIN_DLL_API cc_fifo * cc_fifo_new(void);
COIN_DLL_API void cc_fifo_delete(cc_fifo * fifo);

COIN_DLL_API void cc_fifo_assign(cc_fifo * fifo, void * ptr, uint32_t type);
COIN_DLL_API void cc_fifo_retrieve(cc_fifo * fifo, void ** ptr, uint32_t * type);
COIN_DLL_API SbBool cc_fifo_try_retrieve(cc_fifo * fifo, void ** ptr, uint32_t * type);

COIN_DLL_API unsigned int cc_fifo_size(cc_fifo * fifo);

COIN_DLL_API void cc_fifo_lock(cc_fifo * fifo);
COIN_DLL_API void cc_fifo_unlock(cc_fifo * fifo);

COIN_DLL_API SbBool cc_fifo_peek(cc_fifo * fifo, void ** item, uint32_t * type);
COIN_DLL_API SbBool cc_fifo_contains(cc_fifo * fifo, void * item);
COIN_DLL_API SbBool cc_fifo_reclaim(cc_fifo * fifo, void * item);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_FIFO_H */
