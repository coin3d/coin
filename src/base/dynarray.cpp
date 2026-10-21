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

// FIXME: this implements a C ADT for a dynamic array of void
// pointers. It's basically just a quick hack of wrapping the C ADT
// around our SbPList C++ class. We should really correct this to be
// the other way around: we should make a generic C ADT for a dynamic
// array, which is then wrapped by SbList.
//
// Don't make this class public until the C rewrite happens, and the
// API has been audited. This ADT must have been reimplemented a
// zillion times around in various projects, so check out how our API
// design fares versus others.
//
// 20030604 mortene.
//
// UPDATE 20030915 mortene: this had actually been implemented in C
// already as cc_list...

#include "base/dynarray.h"

#include <Inventor/lists/SbPList.h>

struct cc_dynarray {
  SbPList plist;
};

static SbBool
cc_dynarray_valid_index(const cc_dynarray * arr, unsigned int idx)
{
  return idx < static_cast<unsigned int>(arr->plist.getLength());
}

cc_dynarray *
cc_dynarray_new(void)
{
  return new struct cc_dynarray;
}

cc_dynarray *
cc_dynarray_duplicate(const cc_dynarray * src)
{
  cc_dynarray * p = cc_dynarray_new();
  p->plist = src->plist;
  return p;
}

void
cc_dynarray_destruct(cc_dynarray * arr)
{
  delete arr;
}

void
cc_dynarray_fit(cc_dynarray * arr)
{
  arr->plist.fit();
}

void
cc_dynarray_append(cc_dynarray * arr, void * item)
{
  arr->plist.append(item);
}

int
cc_dynarray_find(const cc_dynarray * arr, void * item)
{
  return arr->plist.find(item);
}

void
cc_dynarray_insert(cc_dynarray * arr, void * item, unsigned int idx)
{
  if (idx <= static_cast<unsigned int>(arr->plist.getLength())) {
    arr->plist.insert(item, static_cast<int>(idx));
  }
}

void
cc_dynarray_remove(cc_dynarray * arr, void * item)
{
  const int idx = arr->plist.find(item);
  if (idx >= 0) arr->plist.remove(idx);
}

void
cc_dynarray_remove_idx(cc_dynarray * arr, unsigned int idx)
{
  if (cc_dynarray_valid_index(arr, idx)) {
    arr->plist.remove(static_cast<int>(idx));
  }
}

void
cc_dynarray_removefast(cc_dynarray * arr, unsigned int idx)
{
  if (cc_dynarray_valid_index(arr, idx)) {
    arr->plist.removeFast(static_cast<int>(idx));
  }
}

unsigned int
cc_dynarray_length(const cc_dynarray * arr)
{
  return arr->plist.getLength();
}

void
cc_dynarray_truncate(cc_dynarray * arr, unsigned int len)
{
  if (len <= static_cast<unsigned int>(arr->plist.getLength())) {
    arr->plist.truncate(static_cast<int>(len));
  }
}

void **
cc_dynarray_get_arrayptr(const cc_dynarray * arr)
{
  return arr->plist.getArrayPtr();
}

void *
cc_dynarray_get(const cc_dynarray * arr, unsigned int idx)
{
  if (!cc_dynarray_valid_index(arr, idx)) return NULL;
  return arr->plist.get(static_cast<int>(idx));
}

SbBool
cc_dynarray_eq(const cc_dynarray * arr1, const cc_dynarray * arr2)
{
  return (arr1->plist == arr2->plist);
}

void
cc_dynarray_set(cc_dynarray * arr, unsigned int idx, void * item)
{
  if (cc_dynarray_valid_index(arr, idx)) {
    arr->plist.set(static_cast<int>(idx), item);
  }
}

#ifdef COIN_TEST_SUITE

#include <climits>

extern "C" {
  typedef struct cc_dynarray cc_dynarray;
  cc_dynarray * cc_dynarray_new(void);
  void cc_dynarray_destruct(cc_dynarray * arr);
  unsigned int cc_dynarray_length(const cc_dynarray * arr);
  void cc_dynarray_append(cc_dynarray * arr, void * item);
  void cc_dynarray_insert(cc_dynarray * arr, void * item, unsigned int idx);
  void * cc_dynarray_get(const cc_dynarray * arr, unsigned int idx);
  void cc_dynarray_set(cc_dynarray * arr, unsigned int idx, void * item);
  void cc_dynarray_remove(cc_dynarray * arr, void * item);
  void cc_dynarray_remove_idx(cc_dynarray * arr, unsigned int idx);
  void cc_dynarray_removefast(cc_dynarray * arr, unsigned int idx);
  void cc_dynarray_truncate(cc_dynarray * arr, unsigned int len);
}

BOOST_AUTO_TEST_CASE(cc_dynarray_reads_do_not_expand)
{
  int value = 1;
  cc_dynarray * array = cc_dynarray_new();
  cc_dynarray_append(array, &value);

  BOOST_CHECK(cc_dynarray_get(array, 1) == NULL);
  BOOST_CHECK(cc_dynarray_get(array, UINT_MAX) == NULL);
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 1U);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 0), &value);

  cc_dynarray_destruct(array);
}

BOOST_AUTO_TEST_CASE(cc_dynarray_rejects_invalid_mutations)
{
  int first = 1;
  int second = 2;
  int missing = 3;
  cc_dynarray * array = cc_dynarray_new();
  cc_dynarray_append(array, &first);
  cc_dynarray_append(array, &second);

  cc_dynarray_insert(array, &missing, 3);
  cc_dynarray_remove(array, &missing);
  cc_dynarray_remove_idx(array, UINT_MAX);
  cc_dynarray_removefast(array, 2);
  cc_dynarray_set(array, UINT_MAX, &missing);
  cc_dynarray_truncate(array, UINT_MAX);

  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 2U);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 0), &first);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 1), &second);

  cc_dynarray_destruct(array);
}

BOOST_AUTO_TEST_CASE(cc_dynarray_empty_and_valid_boundaries)
{
  int values[4] = { 0, 1, 2, 3 };
  cc_dynarray * array = cc_dynarray_new();
  BOOST_CHECK(cc_dynarray_get(array, 0) == NULL);
  cc_dynarray_remove_idx(array, 0);
  cc_dynarray_removefast(array, 0);
  cc_dynarray_set(array, 0, &values[0]);
  cc_dynarray_remove(array, &values[0]);
  cc_dynarray_truncate(array, 1);
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 0U);

  cc_dynarray_insert(array, &values[0], 0);
  cc_dynarray_insert(array, &values[2], 1); // insertion at length
  cc_dynarray_insert(array, &values[1], 1); // insertion in the middle
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 3U);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 0), &values[0]);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 1), &values[1]);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 2), &values[2]);
  cc_dynarray_set(array, 2, &values[3]);
  cc_dynarray_remove_idx(array, 1); // ordered removal
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 2U);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 1), &values[3]);
  cc_dynarray_append(array, &values[2]);
  cc_dynarray_removefast(array, 0); // last item replaces the removed item
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 0), &values[2]);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 1), &values[3]);
  cc_dynarray_truncate(array, 2); // equal length is valid
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 2U);
  cc_dynarray_remove(array, &values[2]);
  BOOST_CHECK_EQUAL(cc_dynarray_get(array, 0), &values[3]);
  cc_dynarray_removefast(array, 0); // removal of the last item
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 0U);
  cc_dynarray_insert(array, &values[0], 0);
  cc_dynarray_truncate(array, 0);
  BOOST_CHECK_EQUAL(cc_dynarray_length(array), 0U);
  cc_dynarray_destruct(array);
}

#endif // COIN_TEST_SUITE
