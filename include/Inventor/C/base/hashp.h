#ifndef CC_HASHP_H
#define CC_HASHP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

#include <Inventor/C/base/memalloc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  struct cc_hash_entry {
    unsigned long key;
    void * val;
    struct cc_hash_entry * next;
  };
  typedef struct cc_hash_entry cc_hash_entry;

  struct cc_hash {
    unsigned int size;
    unsigned int elements;
    float loadfactor;
    unsigned int threshold;
    cc_hash_entry ** buckets;
    cc_hash_func * hashfunc;
    cc_memalloc * memalloc;
  };

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_HASHP_H */
