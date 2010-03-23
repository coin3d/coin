#ifndef CC_CONDVAR_H
#define CC_CONDVAR_H

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
#include <Inventor/C/base/time.h>  /* cc_time */
#include <Inventor/C/threads/common.h>  /* cc_condvar */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  COIN_DLL_API cc_condvar * cc_condvar_construct(void);
  COIN_DLL_API void cc_condvar_destruct(cc_condvar * condvar);
  
  COIN_DLL_API int cc_condvar_wait(cc_condvar * condvar, cc_mutex * mutex);
  COIN_DLL_API int cc_condvar_timed_wait(cc_condvar * condvar, cc_mutex * mutex,
                                         cc_time period);
  
  COIN_DLL_API void cc_condvar_wake_one(cc_condvar * condvar);
  COIN_DLL_API void cc_condvar_wake_all(cc_condvar * condvar);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_CONDVAR_H */
