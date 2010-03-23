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

/* OBSOLETE: the C data abstraction for a hash has been moved into
   dict.c and renamed to cc_dict. This code is present here just to be
   backwards API and ABI compatible on the Coin 2.x releases. */

/* ********************************************************************** */

#define COIN_ALLOW_CC_HASH /* Hack to get around include protection
                              for obsoleted ADT. */

/*! \file hash.h */
#include <Inventor/C/base/hash.h>

#include <cassert>
#include <cstdio>
#include <cstring>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#include "base/hashp.h"
#include "tidbitsp.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::memset;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

#undef COIN_ALLOW_CC_HASH

/* ********************************************************************** */
/* private functions */

extern "C" {

static cc_hash_key
hash_default_hashfunc(const cc_hash_key key)
{
  return key;
}

} // extern "C"

static unsigned int
hash_get_index(cc_hash * ht, cc_hash_key key)
{
  assert(ht != NULL);
  key = ht->hashfunc(key);
  return key % ht->size;
}

static void
hash_resize(cc_hash * ht, unsigned int newsize)
{
  cc_hash_entry ** oldbuckets = ht->buckets;
  unsigned int oldsize = ht->size, i;
  cc_hash_entry * prev;

  /* Never shrink the table */
  if (ht->size >= newsize)
    return;

  ht->size = newsize;
  ht->elements = 0;
  ht->threshold = (unsigned int) (newsize * ht->loadfactor);
  ht->buckets = (cc_hash_entry **) calloc(newsize, sizeof(cc_hash_entry*));

  /* Transfer all mappings */
  for (i = 0; i < oldsize; i++) {
    cc_hash_entry * he = oldbuckets[i];
    while (he) {
      cc_hash_put(ht, he->key, he->val);
      prev = he;
      he = he->next;
      cc_memalloc_deallocate(ht->memalloc, (void*) prev);
    }
  }
  free(oldbuckets);
}

/* ********************************************************************** */
/* public api */

/*!
  Construct a hash table.

  \a size is the initial bucket size. The caller need not attempt to
  find a good (prime number) value for this argument to ensure good
  hashing. That will be taken care of internally.

  \a loadfactor is the percentage the table should be filled before
  resizing, and should be a number from 0 to 1. It is of course
  possible to specify a number bigger than 1, but then there will be
  greater chance of having many elements on the same bucket (linear
  search for an element). If you supply a number <= 0 for loadfactor,
  the default value 0.75 will be used.
*/
cc_hash *
cc_hash_construct(unsigned int size, float loadfactor)
{
  unsigned int s;
  cc_hash * ht = (cc_hash *) malloc(sizeof(cc_hash));

  /* size should be a prime number */
  s = (unsigned int) coin_geq_prime_number(size);
  if (loadfactor <= 0.0f) loadfactor = 0.75f;
  
  ht->size = s;
  ht->elements = 0;
  ht->threshold = (unsigned int) (s * loadfactor);
  ht->loadfactor = loadfactor;
  ht->buckets = (cc_hash_entry **) calloc(s, sizeof(cc_hash_entry*));
  ht->hashfunc = hash_default_hashfunc;
  /* we use a memory allocator to avoid an operating system malloc
     every time a new entry is needed */
  ht->memalloc = cc_memalloc_construct(sizeof(cc_hash_entry));
  return ht;
}

/*!
  Destruct the hash table \a ht.
*/
void
cc_hash_destruct(cc_hash * ht)
{
  cc_hash_clear(ht);
  cc_memalloc_destruct(ht->memalloc);
  free(ht->buckets);
  free(ht);
}

/*!
  Clear/remove all elements in the hash table \a ht.
*/
void
cc_hash_clear(cc_hash * ht)
{
  // cc_memalloc_clear() will free memory used by internal
  // structures. To avoid continuous memory allocation/deallocation
  // that could be bad for performance (cc_hash is used in
  // SoSensorManager) we manually free all entries from cc_memalloc
  // instead.
#if 0 // disabled
  cc_memalloc_clear(ht->memalloc); /* free all memory used by all entries */
#else // new version that will not trigger any malloc()/free() calls
  unsigned int i;
  cc_hash_entry * entry;
  cc_hash_entry * next;
  for (i = 0; i < ht->size; i++) {
    entry = ht->buckets[i];
    while (entry) {
      next = entry->next;
      cc_memalloc_deallocate(ht->memalloc, (void*) entry);
      entry = next;
    }
  }
#endif // new version

  // all memory has been freed. Just clear buckets
  memset(ht->buckets, 0, ht->size * sizeof(cc_hash_entry*));
  ht->elements = 0;
}

/*!

  Insert a new element in the hash table \a ht. \a key is the key used
  to identify the element, while \a val is the element value. If \a
  key is already used by another element, the element value will be
  overwritten, and \e FALSE is returned. Otherwise a new element is
  created and \e TRUE is returned.

 */
SbBool
cc_hash_put(cc_hash * ht, cc_hash_key key, void * val)
{
  unsigned int i = hash_get_index(ht, key);
  cc_hash_entry * he = ht->buckets[i];

  while (he) {
    if (he->key == key) {
      /* Replace the old value */
      he->val = val;
      return FALSE;
    }
    he = he->next;
  }

  /* Key not already in the hash table; insert a new
   * entry as the first element in the bucket
   */
  he = (cc_hash_entry *) cc_memalloc_allocate(ht->memalloc);
  he->key = key;
  he->val = val;
  he->next = ht->buckets[i];
  ht->buckets[i] = he;

  if (ht->elements++ >= ht->threshold) {
    hash_resize(ht, (unsigned int) coin_geq_prime_number(ht->size + 1));
  }
  return TRUE;
}

/*!

  Find the element with key value \a key. If found, the value is written to
  \a val, and TRUE is returned. Otherwise FALSE is returned and \a val
  is not changed.

*/
SbBool
cc_hash_get(cc_hash * ht, cc_hash_key key, void ** val)
{
  cc_hash_entry * he;
  unsigned int i = hash_get_index(ht, key);
  he = ht->buckets[i];
  while (he) {
    if (he->key == key) {
      *val = he->val;
      return TRUE;
    }
    he = he->next;
  }
  return FALSE;
}

/*!
  Attempt to remove the element with key value \a key. Returns
  TRUE if found, FALSE otherwise.
*/
SbBool
cc_hash_remove(cc_hash * ht, cc_hash_key key)
{
  cc_hash_entry * he, *next, * prev;
  unsigned int i = hash_get_index(ht, key);

  he = ht->buckets[i];
  prev = NULL;
  while (he) {
    next = he->next;
    if (he->key == key) {
      ht->elements--;
      if (prev == NULL) {
        ht->buckets[i] = next;
      }
      else {
        prev->next = next;
      }
      cc_memalloc_deallocate(ht->memalloc, (void*) he);
      return TRUE;
    }
    prev = he;
    he = next;
  }
  return FALSE;
}

/*!
  Return the number of elements in the hash table.
*/
unsigned int
cc_hash_get_num_elements(cc_hash * ht)
{
  return ht->elements;
}

/*!
  Set the hash func that is used to map key values into
  a bucket index.
*/
void
cc_hash_set_hash_func(cc_hash * ht, cc_hash_func * func)
{
  ht->hashfunc = func;
}

/*!
  Call \a func for for each element in the hash table.
*/
void
cc_hash_apply(cc_hash * ht, cc_hash_apply_func * func, void * closure)
{
  unsigned int i;
  cc_hash_entry * elem;
  for (i = 0; i < ht->size; i++) {
    elem = ht->buckets[i];
    while (elem) {
      func(elem->key, elem->val, closure);
      elem = elem->next;
    }
  }
}

/*!
  For debugging only. Prints information about hash with
  cc_debugerror.
*/
void
cc_hash_print_stat(cc_hash * ht)
{
  unsigned int i, used_buckets = 0, max_chain_l = 0;
  for (i = 0; i < ht->size; i++) {
    if (ht->buckets[i]) {
      unsigned int chain_l = 0;
      cc_hash_entry * he = ht->buckets[i];
      used_buckets++;
      while (he) {
        chain_l++;
        he = he->next;
      }
      if (chain_l > max_chain_l) max_chain_l = chain_l;
    }
  }
  cc_debugerror_postinfo("cc_hash_print_stat",
                         "Used buckets %u of %u (%u elements), "
                         "avg chain length: %.2f, max chain length: %u\n",
                         used_buckets, ht->size, ht->elements,
                         (float)ht->elements / used_buckets, max_chain_l);
}
