#ifndef CC_STORAGEP_H
#define CC_STORAGEP_H

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

#include <Inventor/C/threads/common.h>  /* cc_storage */
#include <Inventor/C/base/hash.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  struct cc_storage {
    unsigned int size;
    void (*constructor)(void *);
    void (*destructor)(void *);
    cc_hash * dict;
    cc_mutex * mutex;
  };
  
  void cc_storage_thread_cleanup(unsigned long threadid);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_STORAGEP_H */
