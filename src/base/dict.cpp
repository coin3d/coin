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

#include "base/dict.h"

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#include "tidbitsp.h"
#include "base/dictp.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::free;
using std::malloc;
using std::calloc;
using std::memset;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

/* ********************************************************************** */
/* private functions */

extern "C" {

static uintptr_t
dict_default_hashfunc(const uintptr_t key)
{
  return key;
}

} // extern "C"

static unsigned int
dict_get_index(cc_dict * ht, uintptr_t key)
{
  assert(ht != NULL);
  key = ht->hashfunc(key);
  return (unsigned int) (key % ht->size);
}

static void
dict_resize(cc_dict * ht, unsigned int newsize)
{
  cc_dict_entry ** oldbuckets = ht->buckets;
  unsigned int oldsize = ht->size, i;

  /* Never shrink the table */
  if (ht->size >= newsize)
    return;

  ht->size = newsize;
  ht->elements = 0;
  ht->threshold = (unsigned int) (newsize * ht->loadfactor);
  ht->buckets = (cc_dict_entry **) calloc(newsize, sizeof(cc_dict_entry*));

  /* Transfer all mappings */
  for (i = 0; i < oldsize; i++) {
    cc_dict_entry * he = oldbuckets[i];
    while (he) {
      cc_dict_entry * oldentry = he;
      he = he->next;
      cc_dict_put(ht, oldentry->key, oldentry->val);
      cc_memalloc_deallocate(ht->memalloc, oldentry);
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
cc_dict *
cc_dict_construct(unsigned int size, float loadfactor)
{
  unsigned int s;
  cc_dict * ht = (cc_dict *) malloc(sizeof(cc_dict));
  
  s = (unsigned int) coin_geq_prime_number(size);
  if (loadfactor <= 0.0f) loadfactor = 0.75f;
  
  ht->size = s;
  ht->elements = 0;
  ht->threshold = (unsigned int) (s * loadfactor);
  ht->loadfactor = loadfactor;
  ht->buckets = (cc_dict_entry **) calloc(s, sizeof(cc_dict_entry*));
  ht->hashfunc = dict_default_hashfunc;
  /* we use a memory allocator to avoid an operating system malloc
     every time a new entry is needed */
  ht->memalloc = cc_memalloc_construct(sizeof(cc_dict_entry));
  return ht;
}

/*!
  Destruct the hash table \a ht.
*/
void
cc_dict_destruct(cc_dict * ht)
{
  cc_dict_clear(ht);
  cc_memalloc_destruct(ht->memalloc);
  free(ht->buckets);
  free(ht);
}

/*!
  Clear/remove all elements in the hash table \a ht.
*/
void
cc_dict_clear(cc_dict * ht)
{
  // cc_memalloc_clear() will free memory used by internal
  // structures. To avoid continuous memory allocation/deallocation
  // that could be bad for performance (cc_dict is used in
  // SoSensorManager) we manually free all entries from cc_memalloc
  // instead.
#if 0 // disabled
  cc_memalloc_clear(ht->memalloc); /* free all memory used by all entries */
#else // new version that will not trigger any malloc()/free() calls
  unsigned int i;
  cc_dict_entry * entry;
  cc_dict_entry * next;
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
  memset(ht->buckets, 0, ht->size * sizeof(cc_dict_entry*));
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
cc_dict_put(cc_dict * ht, uintptr_t key, void * val)
{
  unsigned int i = dict_get_index(ht, key);
  cc_dict_entry * he = ht->buckets[i];

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
  he = (cc_dict_entry *) cc_memalloc_allocate(ht->memalloc);
  he->key = key;
  he->val = val;
  he->next = ht->buckets[i];
  ht->buckets[i] = he;
  
  if (ht->elements++ >= ht->threshold) {
    dict_resize(ht, (unsigned int) coin_geq_prime_number(ht->size + 1));
  }
  return TRUE;
}

/*!

  Find the element with key value \a key. If found, the value is written to
  \a val, and TRUE is returned. Otherwise FALSE is returned and \a val
  is not changed.

*/
SbBool
cc_dict_get(cc_dict * ht, uintptr_t key, void ** val)
{
  cc_dict_entry * he;
  unsigned int i = dict_get_index(ht, key);
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
cc_dict_remove(cc_dict * ht, uintptr_t key)
{
  cc_dict_entry * he, *next, * prev;
  unsigned int i = dict_get_index(ht, key);

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
cc_dict_get_num_elements(cc_dict * ht)
{
  return ht->elements;
}

/*!
  Set the hash func that is used to map key values into
  a bucket index.

  Existing entries are reindexed using the new function. The entries
  themselves are preserved; only their bucket links are changed.
*/
void
cc_dict_set_hash_func(cc_dict * ht, cc_dict_hash_func * func)
{
  assert(ht != NULL);
  if (ht->hashfunc == func) return;
  if (ht->elements == 0) {
    ht->hashfunc = func;
    return;
  }

  cc_dict_entry ** buckets = (cc_dict_entry **)
    calloc(ht->size, sizeof(cc_dict_entry *));
  assert(buckets != NULL);
  if (buckets == NULL) return;

  for (unsigned int i = 0; i < ht->size; ++i) {
    cc_dict_entry * entry = ht->buckets[i];
    while (entry != NULL) {
      cc_dict_entry * next = entry->next;
      const unsigned int idx = (unsigned int) (func(entry->key) % ht->size);
      entry->next = buckets[idx];
      buckets[idx] = entry;
      entry = next;
    }
  }

  free(ht->buckets);
  ht->buckets = buckets;
  ht->hashfunc = func;
}

/*!
  Call \a func for for each element in the hash table.
*/
void
cc_dict_apply(cc_dict * ht, cc_dict_apply_func * func, void * closure)
{
  unsigned int i;
  cc_dict_entry * elem;
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
cc_dict_print_stat(cc_dict * ht)
{
  unsigned int i, used_buckets = 0, max_chain_l = 0;
  for (i = 0; i < ht->size; i++) {
    if (ht->buckets[i]) {
      unsigned int chain_l = 0;
      cc_dict_entry * he = ht->buckets[i];
      used_buckets++;
      while (he) {
        chain_l++;
        he = he->next;
      }
      if (chain_l > max_chain_l) max_chain_l = chain_l;
    }
  }
  cc_debugerror_postinfo("cc_dict_print_stat",
                         "Used buckets %u of %u (%u elements), "
                         "avg chain length: %.2f, max chain length: %u\n",
                         used_buckets, ht->size, ht->elements,
                         used_buckets > 0 ?
                           (float)ht->elements / used_buckets : 0.0f,
                         max_chain_l);
}

#ifdef COIN_TEST_SUITE
#include <base/dict.h>

#include <limits>
#include <string>

#include <Inventor/C/base/string.h>
#include <Inventor/C/errors/debugerror.h>

namespace {

static uintptr_t
ccdict_test_collision_hash(const uintptr_t)
{
  return 1;
}

static uintptr_t
ccdict_test_mixed_hash(const uintptr_t key)
{
  return (key ^ (key >> 17)) * static_cast<uintptr_t>(2654435761u);
}

struct CcDictApplyData {
  unsigned int count;
  uintptr_t keysum;
  uintptr_t valuesum;
};

static void
ccdict_test_apply_cb(uintptr_t key, void * value, void * closure)
{
  CcDictApplyData * data = static_cast<CcDictApplyData *>(closure);
  ++data->count;
  data->keysum += key;
  data->valuesum += reinterpret_cast<uintptr_t>(value);
}

static unsigned int
ccdict_test_random(unsigned int & state)
{
  state = state * 1664525u + 1013904223u;
  return state;
}

static void
ccdict_test_debugerror_cb(const cc_debugerror * error, void * closure)
{
  std::string * message = static_cast<std::string *>(closure);
  const cc_string * debugstring = cc_error_get_debug_string(&error->super);
  *message = cc_string_get_text(debugstring);
}

} // namespace

BOOST_AUTO_TEST_CASE(ccdict_empty_zero_key_and_null_value)
{
  cc_dict * dictionary = cc_dict_construct(0, 0.0f);
  BOOST_REQUIRE(dictionary != NULL);
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);

  void * output = reinterpret_cast<void *>(static_cast<uintptr_t>(0x1234u));
  BOOST_CHECK(!cc_dict_get(dictionary, 99u, &output));
  BOOST_CHECK(output == reinterpret_cast<void *>(static_cast<uintptr_t>(0x1234u)));

  BOOST_CHECK(cc_dict_put(dictionary, 0u, NULL));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 1u);
  output = reinterpret_cast<void *>(static_cast<uintptr_t>(0x1234u));
  BOOST_CHECK(cc_dict_get(dictionary, 0u, &output));
  BOOST_CHECK(output == NULL);

  BOOST_CHECK(cc_dict_remove(dictionary, 0u));
  BOOST_CHECK(!cc_dict_remove(dictionary, 0u));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_put_overwrites_without_growing)
{
  cc_dict * dictionary = cc_dict_construct(2, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);
  void * first = reinterpret_cast<void *>(static_cast<uintptr_t>(11u));
  void * second = reinterpret_cast<void *>(static_cast<uintptr_t>(22u));

  BOOST_CHECK(cc_dict_put(dictionary, 7u, first));
  BOOST_CHECK(!cc_dict_put(dictionary, 7u, second));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 1u);

  void * output = NULL;
  BOOST_CHECK(cc_dict_get(dictionary, 7u, &output));
  BOOST_CHECK(output == second);

  BOOST_CHECK(!cc_dict_put(dictionary, 7u, NULL));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 1u);
  output = second;
  BOOST_CHECK(cc_dict_get(dictionary, 7u, &output));
  BOOST_CHECK(output == NULL);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_collision_chain_removes_every_position)
{
  const unsigned int count = 257;
  cc_dict * dictionary = cc_dict_construct(2, 8.0f);
  BOOST_REQUIRE(dictionary != NULL);
  cc_dict_set_hash_func(dictionary, ccdict_test_collision_hash);

  for (unsigned int i = 0; i < count; ++i) {
    BOOST_CHECK(cc_dict_put(dictionary, i,
      reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u))));
  }
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), count);

  for (unsigned int i = 0; i < count; ++i) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, i, &output));
    BOOST_CHECK(output ==
      reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u)));
  }

  for (unsigned int step = 0; step < count; ++step) {
    const unsigned int key = (step * 131u) % count;
    BOOST_CHECK(cc_dict_remove(dictionary, key));
    BOOST_CHECK(!cc_dict_remove(dictionary, key));
  }
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_resize_preserves_all_mappings)
{
  const unsigned int count = 8192;
  cc_dict * dictionary = cc_dict_construct(1, 0.25f);
  BOOST_REQUIRE(dictionary != NULL);
  cc_dict_set_hash_func(dictionary, ccdict_test_mixed_hash);

  for (unsigned int i = 0; i < count; ++i) {
    BOOST_CHECK(cc_dict_put(dictionary, i * 17u,
      reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u))));
  }
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), count);
  for (unsigned int i = 0; i < count; ++i) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, i * 17u, &output));
    BOOST_CHECK(output ==
      reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u)));
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_clear_and_reuse)
{
  cc_dict * dictionary = cc_dict_construct(1, 0.1f);
  BOOST_REQUIRE(dictionary != NULL);
  for (unsigned int cycle = 0; cycle < 20; ++cycle) {
    for (unsigned int i = 0; i < 512; ++i) {
      BOOST_CHECK(cc_dict_put(dictionary, cycle * 1000u + i,
        reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u))));
    }
    BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 512u);
    cc_dict_clear(dictionary);
    BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);
    void * output = NULL;
    BOOST_CHECK(!cc_dict_get(dictionary, cycle * 1000u, &output));
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_apply_visits_each_mapping_once)
{
  const unsigned int count = 1000;
  cc_dict * dictionary = cc_dict_construct(3, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);
  uintptr_t expectedkeysum = 0;
  uintptr_t expectedvaluesum = 0;
  for (unsigned int i = 0; i < count; ++i) {
    const uintptr_t key = static_cast<uintptr_t>(i) * 29u;
    const uintptr_t value = static_cast<uintptr_t>(i) + 3u;
    expectedkeysum += key;
    expectedvaluesum += value;
    cc_dict_put(dictionary, key, reinterpret_cast<void *>(value));
  }

  CcDictApplyData result = { 0u, 0u, 0u };
  cc_dict_apply(dictionary, ccdict_test_apply_cb, &result);
  BOOST_CHECK_EQUAL(result.count, count);
  BOOST_CHECK_EQUAL(result.keysum, expectedkeysum);
  BOOST_CHECK_EQUAL(result.valuesum, expectedvaluesum);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_randomized_operations_match_model)
{
  const unsigned int keycount = 256;
  SbBool present[keycount];
  uintptr_t values[keycount];
  for (unsigned int i = 0; i < keycount; ++i) {
    present[i] = FALSE;
    values[i] = 0u;
  }

  cc_dict * dictionary = cc_dict_construct(1, 0.5f);
  BOOST_REQUIRE(dictionary != NULL);
  unsigned int state = 0xd1c710a5u;
  unsigned int expectedcount = 0u;

  for (unsigned int operation = 0; operation < 20000; ++operation) {
    const unsigned int key = ccdict_test_random(state) % keycount;
    const unsigned int choice = ccdict_test_random(state) % 100u;
    if (choice < 45u) {
      const uintptr_t value = static_cast<uintptr_t>(ccdict_test_random(state));
      const SbBool inserted = cc_dict_put(
        dictionary, key, reinterpret_cast<void *>(value));
      BOOST_CHECK_EQUAL(inserted, present[key] ? FALSE : TRUE);
      if (!present[key]) ++expectedcount;
      present[key] = TRUE;
      values[key] = value;
    }
    else if (choice < 70u) {
      const SbBool removed = cc_dict_remove(dictionary, key);
      BOOST_CHECK_EQUAL(removed, present[key]);
      if (present[key]) --expectedcount;
      present[key] = FALSE;
    }
    else if (choice < 98u) {
      void * output = reinterpret_cast<void *>(static_cast<uintptr_t>(1u));
      const SbBool found = cc_dict_get(dictionary, key, &output);
      BOOST_CHECK_EQUAL(found, present[key]);
      if (present[key]) {
        BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(output), values[key]);
      }
      else {
        BOOST_CHECK(output == reinterpret_cast<void *>(static_cast<uintptr_t>(1u)));
      }
    }
    else {
      cc_dict_clear(dictionary);
      expectedcount = 0u;
      for (unsigned int i = 0; i < keycount; ++i) present[i] = FALSE;
    }
    BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), expectedcount);
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_changing_hash_reindexes_existing_mappings)
{
  const uintptr_t count = 4096u;
  cc_dict * dictionary = cc_dict_construct(2, 0.25f);
  BOOST_REQUIRE(dictionary != NULL);

  for (uintptr_t key = 1u; key <= count; ++key) {
    BOOST_CHECK(cc_dict_put(dictionary, key,
      reinterpret_cast<void *>(key + 1000u)));
  }

  cc_dict_set_hash_func(dictionary, ccdict_test_collision_hash);
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary),
                    static_cast<unsigned int>(count));
  for (uintptr_t key = 1u; key <= count; ++key) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, key, &output));
    BOOST_CHECK(output == reinterpret_cast<void *>(key + 1000u));
  }

  BOOST_CHECK(!cc_dict_put(dictionary, 17u,
    reinterpret_cast<void *>(static_cast<uintptr_t>(17000u))));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary),
                    static_cast<unsigned int>(count));
  BOOST_CHECK(cc_dict_remove(dictionary, 31u));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary),
                    static_cast<unsigned int>(count - 1u));

  cc_dict_set_hash_func(dictionary, ccdict_test_mixed_hash);
  for (uintptr_t key = 1u; key <= count; ++key) {
    void * output = NULL;
    if (key == 31u) {
      BOOST_CHECK(!cc_dict_get(dictionary, key, &output));
    }
    else {
      BOOST_CHECK(cc_dict_get(dictionary, key, &output));
      const uintptr_t expected = key == 17u ? 17000u : key + 1000u;
      BOOST_CHECK(output == reinterpret_cast<void *>(expected));
    }
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_reapplying_same_hash_preserves_mappings)
{
  cc_dict * dictionary = cc_dict_construct(17, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);
  cc_dict_set_hash_func(dictionary, ccdict_test_mixed_hash);
  for (uintptr_t key = 1u; key <= 32u; ++key) {
    cc_dict_put(dictionary, key, reinterpret_cast<void *>(key));
  }

  cc_dict_set_hash_func(dictionary, ccdict_test_mixed_hash);
  for (uintptr_t key = 1u; key <= 32u; ++key) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, key, &output));
    BOOST_CHECK(output == reinterpret_cast<void *>(key));
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_resize_threshold_and_overwrite_boundary)
{
  cc_dict * dictionary = cc_dict_construct(11, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);

  for (uintptr_t key = 1u; key <= 8u; ++key) {
    BOOST_CHECK(cc_dict_put(dictionary, key, reinterpret_cast<void *>(key)));
  }
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 8u);

  BOOST_CHECK(!cc_dict_put(dictionary, 4u,
    reinterpret_cast<void *>(static_cast<uintptr_t>(400u))));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 8u);

  BOOST_CHECK(cc_dict_put(dictionary, 9u,
    reinterpret_cast<void *>(static_cast<uintptr_t>(9u))));
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 9u);

  void * output = NULL;
  BOOST_CHECK(cc_dict_get(dictionary, 4u, &output));
  BOOST_CHECK(output ==
    reinterpret_cast<void *>(static_cast<uintptr_t>(400u)));
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_hash_changes_are_safe_while_empty)
{
  cc_dict * dictionary = cc_dict_construct(17, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);

  cc_dict_set_hash_func(dictionary, ccdict_test_collision_hash);
  cc_dict_set_hash_func(dictionary, ccdict_test_mixed_hash);
  for (uintptr_t key = 0u; key < 128u; ++key) {
    cc_dict_put(dictionary, key, reinterpret_cast<void *>(key + 1u));
  }
  for (uintptr_t key = 0u; key < 128u; ++key) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, key, &output));
    BOOST_CHECK(output == reinterpret_cast<void *>(key + 1u));
  }

  cc_dict_clear(dictionary);
  cc_dict_set_hash_func(dictionary, ccdict_test_collision_hash);
  for (uintptr_t key = 0u; key < 128u; ++key) {
    cc_dict_put(dictionary, key, reinterpret_cast<void *>(key + 2u));
  }
  for (uintptr_t key = 0u; key < 128u; ++key) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, key, &output));
    BOOST_CHECK(output == reinterpret_cast<void *>(key + 2u));
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_extreme_and_pointer_shaped_keys)
{
  const uintptr_t maximum = std::numeric_limits<uintptr_t>::max();
  const uintptr_t keys[] = {
    0u, 1u, maximum, maximum - 1u, maximum / 2u,
    static_cast<uintptr_t>(0x1000u),
    static_cast<uintptr_t>(0x100000u),
    static_cast<uintptr_t>(0x10000000u)
  };
  const size_t count = sizeof(keys) / sizeof(keys[0]);
  cc_dict * dictionary = cc_dict_construct(3, 0.5f);
  BOOST_REQUIRE(dictionary != NULL);

  for (size_t i = 0; i < count; ++i) {
    BOOST_CHECK(cc_dict_put(dictionary, keys[i],
      reinterpret_cast<void *>(static_cast<uintptr_t>(i + 1u))));
  }
  for (size_t i = 0; i < count; ++i) {
    void * output = NULL;
    BOOST_CHECK(cc_dict_get(dictionary, keys[i], &output));
    BOOST_CHECK_EQUAL(reinterpret_cast<uintptr_t>(output),
                      static_cast<uintptr_t>(i + 1u));
  }
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_finite_loadfactor_boundaries)
{
  const float loadfactors[] = { -2.0f, 0.0f, 0.01f, 1.0f, 2.0f };
  const size_t factorcount = sizeof(loadfactors) / sizeof(loadfactors[0]);

  for (size_t factor = 0; factor < factorcount; ++factor) {
    cc_dict * dictionary = cc_dict_construct(2, loadfactors[factor]);
    BOOST_REQUIRE(dictionary != NULL);

    for (uintptr_t key = 0u; key < 64u; ++key) {
      cc_dict_put(dictionary, key, reinterpret_cast<void *>(key + 1u));
    }
    BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 64u);
    for (uintptr_t key = 0u; key < 64u; ++key) {
      void * output = NULL;
      BOOST_CHECK(cc_dict_get(dictionary, key, &output));
      BOOST_CHECK(output == reinterpret_cast<void *>(key + 1u));
    }
    cc_dict_destruct(dictionary);
  }
}

BOOST_AUTO_TEST_CASE(ccdict_repeated_clear_on_empty_dictionary)
{
  cc_dict * dictionary = cc_dict_construct(1, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);
  for (unsigned int i = 0; i < 1000u; ++i) {
    cc_dict_clear(dictionary);
    BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);
  }
  cc_dict_put(dictionary, 1u,
    reinterpret_cast<void *>(static_cast<uintptr_t>(2u)));
  cc_dict_clear(dictionary);
  cc_dict_clear(dictionary);
  BOOST_CHECK_EQUAL(cc_dict_get_num_elements(dictionary), 0u);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_apply_with_total_collision_visits_every_entry)
{
  const unsigned int count = 1024u;
  cc_dict * dictionary = cc_dict_construct(2, 2.0f);
  BOOST_REQUIRE(dictionary != NULL);
  cc_dict_set_hash_func(dictionary, ccdict_test_collision_hash);
  uintptr_t expectedkeysum = 0u;
  uintptr_t expectedvaluesum = 0u;
  for (uintptr_t key = 0u; key < count; ++key) {
    expectedkeysum += key;
    expectedvaluesum += key + 1u;
    cc_dict_put(dictionary, key, reinterpret_cast<void *>(key + 1u));
  }

  CcDictApplyData result = { 0u, 0u, 0u };
  cc_dict_apply(dictionary, ccdict_test_apply_cb, &result);
  BOOST_CHECK_EQUAL(result.count, count);
  BOOST_CHECK_EQUAL(result.keysum, expectedkeysum);
  BOOST_CHECK_EQUAL(result.valuesum, expectedvaluesum);
  cc_dict_destruct(dictionary);
}

BOOST_AUTO_TEST_CASE(ccdict_print_stat_handles_empty_dictionary)
{
  cc_dict * dictionary = cc_dict_construct(2, 0.75f);
  BOOST_REQUIRE(dictionary != NULL);
  std::string message;
  cc_debugerror_cb * previouscallback = cc_debugerror_get_handler_callback();
  void * previousdata = cc_debugerror_get_handler_data();
  cc_debugerror_set_handler_callback(ccdict_test_debugerror_cb, &message);
  cc_dict_print_stat(dictionary);
  cc_debugerror_set_handler_callback(previouscallback, previousdata);

  BOOST_CHECK(!message.empty());
  BOOST_CHECK_MESSAGE(message.find("avg chain length: 0.00") !=
                        std::string::npos,
    "statistics for an empty dictionary did not report a zero average");
  cc_dict_destruct(dictionary);
}
#endif // COIN_TEST_SUITE
