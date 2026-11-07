/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/* OBSOLETE: the C data abstraction for a hash has been moved into
   dict.c and renamed to cc_dict. This code is present here just to be
   backwards API and ABI compatible with existing clients. */

/* ********************************************************************** */

#define COIN_ALLOW_CC_HASH /* Hack to get around include protection
                              for obsoleted ADT. */

#include <Inventor/C/base/hash.h>

#include <cassert>
#include <cmath>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#include "base/hashp.h"
#include "primep.h"
#include "base/oomp.h"
#include "tidbitsp.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::memset;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

#undef COIN_ALLOW_CC_HASH

/* ********************************************************************** */

/*!
  \typedef uintptr_t cc_hash_key;

  The type definition used locally for a hash key.
*/

/*!
  \struct cc_hash hash.h Inventor/C/base/hash.h

  Note that the cc_hash structure has been obsoleted and should no longer be
  used. It is maintained purely for backwards compatibility.
  
  cc_dict is now the preferred structure.
*/

/*!
  \typedef struct cc_hash cc_hash;

  The type definition for the cc_hash structure.
*/

/*!
  \typedef cc_hash_key cc_hash_func(const cc_hash_key key)

  A type definition for cc_hash_func function pointers.
*/

/*!
  \typedef void cc_hash_apply_func(cc_hash_key key, void * val, void * closure)

  A type definition for cc_hash_apply_func function pointers.
*/

/* ********************************************************************** */
/* private functions */

extern "C" {

/*!
  Private default function - actually does nothing.
*/

static cc_hash_key
hash_default_hashfunc(const cc_hash_key key)
{
  return key;
}

} // extern "C"

/*!
  Private function that returns the index for a given key.
*/

static unsigned int
hash_get_index(cc_hash * ht, cc_hash_key key)
{
  assert(ht != NULL);
  key = ht->hashfunc(key);
  return key % ht->size;
}

/*!
  Private function to resize the hash table.
*/

static unsigned int
hash_threshold(unsigned int size, float loadfactor)
{
  const double scaled = static_cast<double>(size) * loadfactor;
  return scaled >= static_cast<double>(UINT_MAX) ? UINT_MAX :
    static_cast<unsigned int>(scaled);
}

static void
hash_rebuild(cc_hash * ht, unsigned int newsize, cc_hash_func * func)
{
  if (static_cast<size_t>(newsize) > SIZE_MAX / sizeof(cc_hash_entry *) ||
      static_cast<size_t>(ht->elements) > SIZE_MAX / sizeof(unsigned int))
    return;

  cc_hash_entry ** buckets = (cc_hash_entry **)
    calloc(newsize, sizeof(cc_hash_entry *));
  if (buckets == NULL) return;

  // The default hash cannot throw. Custom hashes need a plan so a callback
  // exception cannot leave the old bucket chains partly rewritten.
  unsigned int * indices = NULL;
  if (func != hash_default_hashfunc && ht->elements != 0) {
    indices = (unsigned int *)
      malloc(static_cast<size_t>(ht->elements) * sizeof(unsigned int));
    if (indices == NULL) {
      free(buckets);
      return;
    }
    size_t count = 0;
    try {
      for (unsigned int i = 0; i < ht->size; ++i) {
        for (cc_hash_entry * entry = ht->buckets[i]; entry != NULL;
             entry = entry->next) {
          indices[count++] = func(entry->key) % newsize;
        }
      }
    }
    catch (...) {
      free(indices);
      free(buckets);
      throw;
    }
  }

  size_t count = 0;
  for (unsigned int i = 0; i < ht->size; ++i) {
    cc_hash_entry * entry = ht->buckets[i];
    while (entry != NULL) {
      cc_hash_entry * next = entry->next;
      const unsigned int index = indices != NULL ?
        indices[count++] : entry->key % newsize;
      entry->next = buckets[index];
      buckets[index] = entry;
      entry = next;
    }
  }
  free(indices);

  cc_hash_entry ** oldbuckets = ht->buckets;
  ht->buckets = buckets;
  ht->size = newsize;
  ht->threshold = hash_threshold(newsize, ht->loadfactor);
  ht->hashfunc = func;
  free(oldbuckets);
}

static void
hash_resize(cc_hash * ht, unsigned int newsize)
{
  /* Never shrink the table */
  if (ht->size >= newsize)
    return;
  hash_rebuild(ht, newsize, ht->hashfunc);
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
  or a non-finite value, the default value 0.75 will be used. A finite
  factor greater than one is accepted and its threshold saturates at
  UINT_MAX. Allocation failure terminates with an OOM diagnostic because
  legacy SbDict callers require a valid hash table. A bucket request above
  the largest representable prime is handled by the same diagnostic policy.
*/
cc_hash *
cc_hash_construct(unsigned int size, float loadfactor)
{
  const unsigned int s =
    static_cast<unsigned int>(coin_exact_prime_at_least(size));
  if (s == 0) coin_oom_abort("cc_hash_construct capacity");
  if (static_cast<size_t>(s) > SIZE_MAX / sizeof(cc_hash_entry *))
    coin_oom_abort("cc_hash_construct buckets");

  cc_hash * ht = (cc_hash *) malloc(sizeof(cc_hash));
  if (ht == NULL) coin_oom_abort("cc_hash_construct");

  if (!std::isfinite(loadfactor) || loadfactor <= 0.0f)
    loadfactor = 0.75f;
  
  ht->size = s;
  ht->elements = 0;
  ht->threshold = hash_threshold(s, loadfactor);
  ht->loadfactor = loadfactor;
  ht->buckets = (cc_hash_entry **) calloc(s, sizeof(cc_hash_entry*));
  if (ht->buckets == NULL) coin_oom_abort("cc_hash_construct buckets");
  ht->hashfunc = hash_default_hashfunc;
  /* we use a memory allocator to avoid an operating system malloc
     every time a new entry is needed */
  ht->memalloc = cc_memalloc_construct_aligned(
    sizeof(cc_hash_entry), alignof(cc_hash_entry));
  if (ht->memalloc == NULL) coin_oom_abort("cc_hash_construct allocator");
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
      cc_memalloc_deallocate(ht->memalloc, entry);
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

  Entry allocation failure or element-count exhaustion terminates with a
  diagnostic. Optional growth failure preserves the newly inserted key.
  A custom hash exception propagates: during the initial lookup it leaves
  the table unchanged; during optional growth the new key remains inserted
  in the original, valid table.

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
  if (ht->elements == UINT_MAX) coin_oom_abort("cc_hash_put capacity");
  he = (cc_hash_entry *) cc_memalloc_allocate(ht->memalloc);
  if (he == NULL) coin_oom_abort("cc_hash_put");
  he->key = key;
  he->val = val;
  he->next = ht->buckets[i];
  ht->buckets[i] = he;

  if (ht->elements++ >= ht->threshold && ht->size < UINT_MAX) {
    hash_resize(ht, (unsigned int) coin_growth_prime_at_least(ht->size + 1));
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
      cc_memalloc_deallocate(ht->memalloc, he);
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
  a bucket index. Passing NULL restores the default hash function.

  Existing entries are reindexed when the function changes. If allocating
  replacement storage fails, the current function and entries are unchanged.
  A hash exception propagates without changing the previous function or links.
  Hash callbacks must be stable and must not modify this table.
*/
void
cc_hash_set_hash_func(cc_hash * ht, cc_hash_func * func)
{
  assert(ht != NULL);
  if (func == NULL) func = hash_default_hashfunc;
  if (ht->hashfunc == func) return;
  if (ht->elements == 0) {
    ht->hashfunc = func;
    return;
  }

  hash_rebuild(ht, ht->size, func);
}

/*!
  Call \a func for each element in the hash table. The callback may remove
  its current entry, but must not otherwise mutate or destroy the table.
*/
void
cc_hash_apply(cc_hash * ht, cc_hash_apply_func * func, void * closure)
{
  unsigned int i;
  cc_hash_entry * elem;
  for (i = 0; i < ht->size; i++) {
    elem = ht->buckets[i];
    while (elem) {
      cc_hash_entry * next = elem->next;
      func(elem->key, elem->val, closure);
      elem = next;
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
                         used_buckets > 0 ?
                           (float)ht->elements / used_buckets : 0.0f,
                         max_chain_l);
}

#ifdef COIN_TEST_SUITE
#include <string>
#include <Inventor/C/base/string.h>
#include <Inventor/C/errors/debugerror.h>

static void
cchash_test_debugerror_cb(const cc_debugerror * error, void * closure)
{
  std::string * message = static_cast<std::string *>(closure);
  const cc_string * debugstring = cc_error_get_debug_string(&error->super);
  *message = cc_string_get_text(debugstring);
}

BOOST_AUTO_TEST_CASE(cchash_print_stat_handles_empty_hash)
{
  cc_hash * hash = cc_hash_construct(2, 0.75f);
  BOOST_REQUIRE(hash != NULL);
  std::string message;
  cc_debugerror_cb * previouscallback = cc_debugerror_get_handler_callback();
  void * previousdata = cc_debugerror_get_handler_data();
  cc_debugerror_set_handler_callback(cchash_test_debugerror_cb, &message);
  cc_hash_print_stat(hash);
  cc_debugerror_set_handler_callback(previouscallback, previousdata);

  BOOST_CHECK_MESSAGE(message.find("avg chain length: 0.00") !=
                        std::string::npos,
    "statistics for an empty hash did not report a zero average");
  cc_hash_destruct(hash);
}

struct CcHashApplyMutationData {
  cc_hash * hash;
  unsigned int visits;
};

static cc_hash_key
cchash_test_collision_hash(cc_hash_key)
{
  return 1;
}

static void
cchash_test_remove_current_during_apply(cc_hash_key key, void *, void * closure)
{
  CcHashApplyMutationData * data =
    static_cast<CcHashApplyMutationData *>(closure);
  ++data->visits;
  cc_hash_remove(data->hash, key);
}

BOOST_AUTO_TEST_CASE(cchash_apply_can_remove_current_entry)
{
  cc_hash * hash = cc_hash_construct(2, 1.0f);
  BOOST_REQUIRE(hash != NULL);
  cc_hash_set_hash_func(hash, cchash_test_collision_hash);
  BOOST_CHECK(cc_hash_put(hash, 0, NULL));
  BOOST_CHECK(cc_hash_put(hash, 2, NULL));
  CcHashApplyMutationData data = { hash, 0 };
  cc_hash_apply(hash, cchash_test_remove_current_during_apply, &data);
  BOOST_CHECK_EQUAL(data.visits, 2u);
  BOOST_CHECK_EQUAL(cc_hash_get_num_elements(hash), 0u);
  cc_hash_destruct(hash);
}
#endif // COIN_TEST_SUITE
