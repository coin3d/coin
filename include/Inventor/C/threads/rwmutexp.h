#ifndef CC_RWMUTEXP_H
#define CC_RWMUTEXP_H

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

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/threads/common.h>
#include <Inventor/C/threads/mutexp.h>
#include <Inventor/C/threads/condvarp.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct cc_rwmutex {
  cc_precedence policy;
  int readers;
  int writers;
  int readwaiters;
  int writewaiters;
  cc_mutex mutex;
  cc_condvar read;
  cc_condvar write;
};

/* ********************************************************************** */

void cc_rwmutex_struct_init(cc_rwmutex * rwmutex);
void cc_rwmutex_struct_clean(cc_rwmutex * rwmutex);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_RWMUTEXP_H */
