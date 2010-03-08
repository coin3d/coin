#ifndef CC_RECMUTEXP_H
#define CC_RECMUTEXP_H

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
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/threads/common.h>

#include "threads/mutexp.h"
#include "threads/condvarp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct cc_recmutex {
  unsigned long threadid;
  int level;
  int waiters;
  cc_mutex mutex;
  cc_condvar condvar;
};

/* ********************************************************************** */

void cc_recmutex_init(void);

void cc_recmutex_struct_init(cc_recmutex * rwmutex);
void cc_recmutex_struct_clean(cc_recmutex * rwmutex);

/* some internal locks are defined here */
int cc_recmutex_internal_field_lock(void);
int cc_recmutex_internal_field_unlock(void);
int cc_recmutex_internal_notify_lock(void);
int cc_recmutex_internal_notify_unlock(void);
/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_RECMUTEXP_H */
