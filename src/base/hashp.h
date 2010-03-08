#ifndef CC_HASHP_H
#define CC_HASHP_H

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

#include <Inventor/C/base/memalloc.h>
#define COIN_ALLOW_CC_HASH
#include <Inventor/C/base/hash.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  struct cc_hash_entry {
    cc_hash_key key;
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
