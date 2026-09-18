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

#include <Inventor/C/base/heap.h>

#include <cassert>
#include <cstdlib>
#include <cstdio>

#include "base/dict.h"
#include "base/heapp.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::realloc;
using std::malloc;
using std::free;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

/* ********************************************************************** */

/*!
  \typedef int cc_heap_compare_cb(void * o1, void * o2)

  A type definition for heap compare callback function.
  The heap compare function yields true if the first argument of the call
  appears before the second in the strict weak ordering relation 
  induced by this type, and false otherwise, i.e. cc_heap_compare_cb(x, x) == false
*/

/*! 
  \typedef struct cc_heap cc_heap

  A type definition for the cc_heap structure
*/

/* ********************************************************************** */
/* private functions */

#define HEAP_PARENT(i) (((i) - 1) / 2)
#define HEAP_LEFT(i) ((i) * 2 + 1)
#define HEAP_RIGHT(i) ((i) * 2 + 2)

static void
heap_resize(cc_heap * h, unsigned int newsize)
{
  /* Never shrink the heap */
  if (h->size >= newsize)
    return;

  h->array = static_cast<void **>(realloc(h->array, newsize * sizeof(void *)));
  assert(h->array);
  h->size = newsize;
}

static void
heap_swap(void ** left, void ** right) {
  void * tmp = *left;
  *left = *right;
  *right = tmp;
}

static void
heap_heapify_down(cc_heap * h, uintptr_t i)
{
  uintptr_t largest = i;

  while (1) {
    i = largest;
    uintptr_t left = HEAP_LEFT(i);
    uintptr_t right = HEAP_RIGHT(i);

    /* Check which node is larger of i and its two children; if any
     * of them is larger swap it with i and proceed down on the child
     */
    if (left < h->elements && h->compare(h->array[left], h->array[largest]) > 0)
      largest = left;

    if (right < h->elements && h->compare(h->array[right], h->array[largest]) > 0)
      largest = right;

    if (largest == i)
      break;

    heap_swap(&h->array[i], &h->array[largest]);

    if (h->support_remove) {
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]), reinterpret_cast<void *>(i));
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[largest]), reinterpret_cast<void *>(largest));
    }
  }
}

static void
heap_heapify_up(cc_heap * h, uintptr_t i)
{
  if (i == 0)
    return;

  /* If o is greater than its parent, swap them and proceed up on the parent */
  while (i > 0 && h->compare(h->array[i], h->array[HEAP_PARENT(i)]) > 0) {
    uintptr_t parent = HEAP_PARENT(i);
    heap_swap(&h->array[i], &h->array[parent]);

    if (h->support_remove) {
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]), reinterpret_cast<void*>(i));
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[parent]), reinterpret_cast<void*>(parent));
    }

    i = parent;
  }
}

static void
heap_heapify(cc_heap * h, uintptr_t i)
{
  if (i > 0 && h->compare(h->array[i], h->array[HEAP_PARENT(i)]) > 0) {
    heap_heapify_up(h, i);
  }
  else {
    heap_heapify_down(h, i);
  }
}

static void
heap_rebuild(cc_heap * h)
{
  if (h->support_remove) {
    cc_dict_clear(h->hash);
    h->duplicates = 0;
    for (uintptr_t i = 0; i < h->elements; ++i) {
      if (!cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]),
                       reinterpret_cast<void *>(i))) {
        ++h->duplicates;
      }
    }
  }

  for (uintptr_t i = h->elements / 2; i > 0; --i) {
    heap_heapify_down(h, i - 1);
  }
}

static void
heap_restore_duplicate_index(cc_heap * h, void * o)
{
  if (h->duplicates == 0) return;

  for (uintptr_t i = 0; i < h->elements; ++i) {
    if (h->array[i] == o) {
      --h->duplicates;
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(o),
                  reinterpret_cast<void *>(i));
      return;
    }
  }
}

/* ********************************************************************** */
/* public api */

/*!

  Construct a heap. \a size is the initial array size. A zero size is
  normalized to one element of initial capacity.

  For a minimum heap \a comparecb should return 1 if the first element
  is less than the second, zero if they are equal or the first element
  is greater than the second.
  For a maximum heap \a comparecb should return 1 if the first element
  is greater than the second, zero if they are equal or the first element
  is less than the second.

  \a support_remove specifies if the heap should support removal of
  elements (other than the top element) after they are added; this
  requires use of a hash table to be efficient, but as a slight runtime
  overhead will be incurred for the add and extract_top functions the
  support can be disabled if you don't need it.

*/

cc_heap *
cc_heap_construct(unsigned int size,
                  cc_heap_compare_cb * comparecb,
                  SbBool support_remove)
{
  if (size == 0) size = 1;

  cc_heap * h = static_cast<cc_heap *>(malloc(sizeof(cc_heap)));
  assert(h);

  h->size = size;
  h->elements = 0;
  h->array = static_cast<void **>(malloc(size * sizeof(void *)));
  assert(h->array);
  h->compare = comparecb;
  h->support_remove = support_remove;
  h->duplicates = 0;
  h->hash = NULL;
  if (support_remove) {
    h->hash = cc_dict_construct(size, 0.0f);
  }
  return h;
}

/*!
  Destruct the heap \a h.
*/
void
cc_heap_destruct(cc_heap * h)
{
  cc_heap_clear(h);
  free(h->array);
  if (h->hash) cc_dict_destruct(h->hash);
  free(h);
}

/*!
  Clear/remove all elements in the heap \a h.
*/
void cc_heap_clear(cc_heap * h)
{
  h->elements = 0;
  h->duplicates = 0;
  if (h->hash) cc_dict_clear(h->hash);
}

/*!
  Add the element \a o to the heap \a h. The same pointer may be added
  more than once; each addition creates a separate heap entry.
*/
void
cc_heap_add(cc_heap * h, void * o)
{
  /* Resize the heap if it is full or the threshold is exceeded */
  if (h->elements == h->size) {
    heap_resize(h, h->size * 2);
  }

  uintptr_t i = h->elements++;
  h->array[i] = o;
  if (h->support_remove) {
    if (!cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]),
                     reinterpret_cast<void*>(i))) {
      ++h->duplicates;
    }
  }

  heap_heapify_up(h, i);
}

/*!
  Returns the top element from the heap \a h. If the heap is empty,
  NULL is returned.
*/
void *
cc_heap_get_top(cc_heap * h)
{
  if (h->elements == 0) return NULL;
  return h->array[0];
}

/*!
  Returns and removes the top element from the heap \a h. If the
  heap is empty, NULL is returned.
*/
void *
cc_heap_extract_top(cc_heap * h)
{
  if (h->elements == 0) return NULL;

  void * top = h->array[0];
  const uintptr_t last = --h->elements;

  if (h->support_remove) {
    cc_dict_remove(h->hash, reinterpret_cast<uintptr_t>(top));
  }

  if (last > 0) {
    h->array[0] = h->array[last];
    if (h->support_remove) {
      cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[0]),
                  reinterpret_cast<void *>(0));
    }
    heap_heapify_down(h, 0);
  }

  if (h->support_remove) heap_restore_duplicate_index(h, top);

  return top;
}

/*!
  Remove one occurrence of \a o from the heap \a h; if present TRUE is
  returned, otherwise FALSE. Please note that the heap must have been
  created with support_remove.
*/
int
cc_heap_remove(cc_heap * h, void * o)
{
  if (!h->support_remove) {
    assert(h->support_remove);
    return FALSE;
  }

  void * tmp;
  if (!cc_dict_get(h->hash, reinterpret_cast<uintptr_t>(o), &tmp))
    return FALSE;

  uintptr_t i = reinterpret_cast<uintptr_t>(tmp);
  assert(i < h->elements);
  assert(h->array[i] == o);

  const uintptr_t last = --h->elements;
  cc_dict_remove(h->hash, reinterpret_cast<uintptr_t>(o));

  if (i != last) {
    h->array[i] = h->array[last];
    cc_dict_put(h->hash, reinterpret_cast<uintptr_t>(h->array[i]),
                reinterpret_cast<void *>(i));
    heap_heapify(h, i);
  }

  heap_restore_duplicate_index(h, o);

  return TRUE;
}

/*!
  Updates the heap \a h for the new value of existing object \a o; if the
  object is present TRUE is returned, otherwise FALSE. The heap must have
  been created with support_remove. If the same pointer occurs more than
  once, all occurrences are reorganized.
*/
int
cc_heap_update(cc_heap * h, void * o)
{
  if (!h->support_remove) {
    assert(h->support_remove);
    return FALSE;
  }

  void * tmp;
  if (!cc_dict_get(h->hash, reinterpret_cast<uintptr_t>(o), &tmp))
    return FALSE;

  uintptr_t i = reinterpret_cast<uintptr_t>(tmp);
  assert(i < h->elements);
  assert(h->array[i] == o);

  if (h->duplicates > 0) {
    unsigned int occurrences = 0;
    for (uintptr_t j = 0; j < h->elements; ++j) {
      if (h->array[j] == o) ++occurrences;
    }
    if (occurrences > 1) {
      heap_rebuild(h);
      return TRUE;
    }
  }

  heap_heapify(h, i);

  return TRUE;
}

/*!
  Returns the number of elements in the heap \a h.
*/
unsigned int
cc_heap_elements(cc_heap * h)
{
  return h->elements;
}

/*!
  Returns TRUE of the heap \a h is empty, otherwise FALSE.
*/
SbBool
cc_heap_empty(cc_heap * h)
{
  return h->elements == 0 ? TRUE : FALSE;
}

/*!
  Print heap \a h using a specified callback \a printcb.
*/
void
cc_heap_print(cc_heap * h, cc_heap_print_cb * printcb, SbString& str, SbBool printLeveled/* = FALSE*/)
{
  if (!printLeveled) {
    for (unsigned int i = 0; i < h->elements; ++i) {
      printcb(h->array[i], str);
      str += ' ';
    }
  }
  else {
    unsigned int level = 0;
    unsigned int level_items = 1;
    unsigned int printed_items = 0;
    for (unsigned int i = 0; i < h->elements; ++i)
    {
      if (printed_items == 0 ) {
        SbString level_str;
        level_str.sprintf("\nlevel #%d : ", level);
        str += level_str;
      }

      printcb(h->array[i], str);
      str += ' ';
      ++printed_items;

      if (printed_items == level_items)
      {
        ++level;
        level_items *= 2; // next level has at most twice as many items
        printed_items = 0;
      }
    }
    str += '\n';
  }
}

#undef HEAP_LEFT
#undef HEAP_PARENT
#undef HEAP_RIGHT

#ifdef COIN_TEST_SUITE
#include <algorithm>
#include <limits>
#include <sstream>
#include <vector>

class mock_up {
public:
  typedef struct wrapped_value {
    double x;
  } wrapped_value;

  static void heap_print_cb(void * v, SbString& str) {
    wrapped_value* value = reinterpret_cast<wrapped_value*>(v);
    std::ostringstream oss;
    oss << value->x;
    str += oss.str().c_str();
  }

  static double heap_evaluate_cb(void * v)
  {
    wrapped_value* value = reinterpret_cast<wrapped_value*>(v);
    return value->x;
  }

  // strict weak ordering is needed for compare callbacks
  static int min_heap_compare_cb(void * lhs, void * rhs)
  {
    return heap_evaluate_cb(lhs) < heap_evaluate_cb(rhs) ? 1 : 0;
  }

  static int max_heap_compare_cb(void * lhs, void * rhs)
  {
    return heap_evaluate_cb(lhs) > heap_evaluate_cb(rhs) ? 1 : 0;
  }
};

namespace {

struct CcHeapTestItem {
  int priority;
  SbBool active;
};

static int
ccheap_test_max_compare(void * lhs, void * rhs)
{
  const int left = static_cast<CcHeapTestItem *>(lhs)->priority;
  const int right = static_cast<CcHeapTestItem *>(rhs)->priority;
  return left > right ? 1 : left < right ? -1 : 0;
}

static int
ccheap_test_min_compare(void * lhs, void * rhs)
{
  return -ccheap_test_max_compare(lhs, rhs);
}

static int
ccheap_test_active_count(const CcHeapTestItem * items, int count)
{
  int result = 0;
  for (int i = 0; i < count; ++i) {
    if (items[i].active) ++result;
  }
  return result;
}

static int
ccheap_test_best_priority(const CcHeapTestItem * items, int count,
                          SbBool maximum)
{
  int result = 0;
  SbBool found = FALSE;
  for (int i = 0; i < count; ++i) {
    if (items[i].active &&
        (!found || (maximum ? items[i].priority > result
                            : items[i].priority < result))) {
      result = items[i].priority;
      found = TRUE;
    }
  }
  BOOST_REQUIRE(found);
  return result;
}

static void
ccheap_test_check_top(cc_heap * heap, const CcHeapTestItem * items,
                      int count, SbBool maximum)
{
  const int active = ccheap_test_active_count(items, count);
  BOOST_CHECK_EQUAL(cc_heap_elements(heap), static_cast<unsigned int>(active));
  BOOST_CHECK_EQUAL(cc_heap_empty(heap), active == 0 ? TRUE : FALSE);
  if (active == 0) {
    BOOST_CHECK(cc_heap_get_top(heap) == NULL);
  }
  else {
    CcHeapTestItem * top = static_cast<CcHeapTestItem *>(cc_heap_get_top(heap));
    BOOST_REQUIRE(top != NULL);
    BOOST_CHECK(top->active);
    BOOST_CHECK_EQUAL(top->priority,
                      ccheap_test_best_priority(items, count, maximum));
  }
}

static unsigned int
ccheap_test_random(unsigned int & state)
{
  state = state * 1664525u + 1013904223u;
  return state;
}

static int
ccheap_test_find_item(CcHeapTestItem * items, int count, SbBool active,
                      unsigned int & state)
{
  const int start = static_cast<int>(ccheap_test_random(state) %
                                     static_cast<unsigned int>(count));
  for (int offset = 0; offset < count; ++offset) {
    const int index = (start + offset) % count;
    if (items[index].active == active) return index;
  }
  return -1;
}

static SbBool
ccheap_test_matches_model(cc_heap * heap, const CcHeapTestItem * items,
                          int count, SbBool maximum)
{
  const int active = ccheap_test_active_count(items, count);
  if (cc_heap_elements(heap) != static_cast<unsigned int>(active)) return FALSE;
  if (cc_heap_empty(heap) != (active == 0 ? TRUE : FALSE)) return FALSE;

  CcHeapTestItem * top =
    static_cast<CcHeapTestItem *>(cc_heap_get_top(heap));
  if (active == 0) return top == NULL ? TRUE : FALSE;
  if (top == NULL || !top->active) return FALSE;

  int best = top->priority;
  for (int i = 0; i < count; ++i) {
    if (items[i].active &&
        (maximum ? items[i].priority > best : items[i].priority < best)) {
      best = items[i].priority;
    }
  }
  return top->priority == best ? TRUE : FALSE;
}

static SbBool
ccheap_test_extracts_ordered(cc_heap * heap, SbBool maximum)
{
  int previous = maximum ? std::numeric_limits<int>::max()
                         : std::numeric_limits<int>::min();
  while (!cc_heap_empty(heap)) {
    CcHeapTestItem * item =
      static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
    if (item == NULL) return FALSE;
    if (maximum ? previous < item->priority : previous > item->priority) {
      return FALSE;
    }
    previous = item->priority;
  }
  return TRUE;
}

} // namespace

BOOST_AUTO_TEST_CASE(min_heap) {
  mock_up::wrapped_value val[] = {{3}, {2}, {1}, {15}, {5}, {4}, {45}};
  cc_heap* heap = cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb*>(mock_up::min_heap_compare_cb), TRUE);
  for (int i = 0, n = sizeof(val) / sizeof(val[0]); i < n; ++i)
    cc_heap_add(heap, &val[i]);
  SbString result;
  cc_heap_print(heap, reinterpret_cast<cc_heap_print_cb*>(mock_up::heap_print_cb), result, FALSE);
  cc_heap_destruct(heap);
  heap = NULL;
  SbString str("1 3 2 15 5 4 45 ");
  BOOST_CHECK_MESSAGE(str == result,
    std::string("Mismatch between ") + result.getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(max_heap) {
  mock_up::wrapped_value val[] = {{3}, {2}, {1}, {15}, {5}, {4}, {45}};
  cc_heap* heap = cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb*>(mock_up::max_heap_compare_cb), TRUE);
  for (int i = 0, n = sizeof(val) / sizeof(val[0]); i < n; ++i)
    cc_heap_add(heap, &val[i]);
  SbString result;
  cc_heap_print(heap, reinterpret_cast<cc_heap_print_cb*>(mock_up::heap_print_cb), result, FALSE);
  cc_heap_destruct(heap);
  heap = NULL;
  SbString str("45 5 15 2 3 1 4 ");
  BOOST_CHECK_MESSAGE(str == result,
    std::string("Mismatch between ") + result.getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(heap_add) {
  mock_up::wrapped_value val[] = {{3}, {2}, {1}, {15}, {5}, {4}, {45}};
  cc_heap* heap = cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb*>(mock_up::min_heap_compare_cb), TRUE);
  for (int i = 0, n = sizeof(val) / sizeof(val[0]); i < n; ++i)
    cc_heap_add(heap, &val[i]);
  double added_value = 12;
  cc_heap_add(heap, &added_value);
  SbString result;
  cc_heap_print(heap, reinterpret_cast<cc_heap_print_cb*>(mock_up::heap_print_cb), result, FALSE);
  cc_heap_destruct(heap);
  heap = NULL;
  SbString str("1 3 2 12 5 4 45 15 ");
  BOOST_CHECK_MESSAGE(str == result,
    std::string("Mismatch between ") + result.getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(heap_remove) {
  mock_up::wrapped_value val[] = {{3}, {2}, {1}, {15}, {5}, {4}, {45}};
  cc_heap* heap = cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb*>(mock_up::min_heap_compare_cb), TRUE);
  for (int i = 0, n = sizeof(val) / sizeof(val[0]); i < n; ++i)
    cc_heap_add(heap, &val[i]);
  cc_heap_remove(heap, &val[3]);
  SbString result;
  cc_heap_print(heap, reinterpret_cast<cc_heap_print_cb*>(mock_up::heap_print_cb), result, FALSE);
  cc_heap_destruct(heap);
  heap = NULL;
  SbString str("1 3 2 45 5 4 ");
  BOOST_CHECK_MESSAGE(str == result,
    std::string("Mismatch between ") + result.getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(heap_update) {
  mock_up::wrapped_value val[] = {{3}, {2}, {1}, {15}, {5}, {4}, {45}};
  cc_heap* heap = cc_heap_construct(256, reinterpret_cast<cc_heap_compare_cb*>(mock_up::min_heap_compare_cb), TRUE);
  for (int i = 0, n = sizeof(val) / sizeof(val[0]); i < n; ++i)
    cc_heap_add(heap, &val[i]);
  val[3].x = 1;
  cc_heap_update(heap, &val[3]);
  SbString result;
  cc_heap_print(heap, reinterpret_cast<cc_heap_print_cb*>(mock_up::heap_print_cb), result, FALSE);
  cc_heap_destruct(heap);
  heap = NULL;
  SbString str("1 1 2 3 5 4 45 ");
  BOOST_CHECK_MESSAGE(str == result,
    std::string("Mismatch between ") + result.getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(ccheap_empty_single_and_clear_reuse)
{
  cc_heap * heap = cc_heap_construct(1, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  BOOST_CHECK(cc_heap_empty(heap));
  BOOST_CHECK_EQUAL(cc_heap_elements(heap), 0u);
  BOOST_CHECK(cc_heap_get_top(heap) == NULL);
  BOOST_CHECK(cc_heap_extract_top(heap) == NULL);

  CcHeapTestItem first = { 4, TRUE };
  cc_heap_add(heap, &first);
  BOOST_CHECK(!cc_heap_empty(heap));
  BOOST_CHECK(cc_heap_get_top(heap) == &first);
  BOOST_CHECK(cc_heap_extract_top(heap) == &first);
  BOOST_CHECK(cc_heap_empty(heap));

  CcHeapTestItem storage[] = {
    { 1, TRUE }, { 7, TRUE }, { 3, TRUE }
  };
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    cc_heap_add(heap, &storage[i]);
  }
  BOOST_CHECK(cc_heap_get_top(heap) == &storage[1]);
  cc_heap_clear(heap);
  BOOST_CHECK(cc_heap_empty(heap));
  BOOST_CHECK(cc_heap_get_top(heap) == NULL);

  CcHeapTestItem reused = { 9, TRUE };
  cc_heap_add(heap, &reused);
  BOOST_CHECK(cc_heap_extract_top(heap) == &reused);
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_extracts_in_comparator_order)
{
  const int priorities[] = { 8, -3, 5, 5, 1, 13, 0, -1 };
  for (int maximum = 0; maximum <= 1; ++maximum) {
    for (int supportremove = 0; supportremove <= 1; ++supportremove) {
      CcHeapTestItem storage[8];
      cc_heap * heap = cc_heap_construct(
        2, maximum ? ccheap_test_max_compare : ccheap_test_min_compare,
        supportremove ? TRUE : FALSE);
      BOOST_REQUIRE(heap != NULL);

      for (int i = 0; i < 8; ++i) {
        storage[i].priority = priorities[i];
        storage[i].active = TRUE;
        cc_heap_add(heap, &storage[i]);
      }

      int previous = maximum ? std::numeric_limits<int>::max()
                             : std::numeric_limits<int>::min();
      while (!cc_heap_empty(heap)) {
        CcHeapTestItem * item =
          static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
        BOOST_REQUIRE(item != NULL);
        if (maximum) BOOST_CHECK(previous >= item->priority);
        else BOOST_CHECK(previous <= item->priority);
        previous = item->priority;
      }
      BOOST_CHECK(cc_heap_extract_top(heap) == NULL);
      cc_heap_destruct(heap);
    }
  }
}

BOOST_AUTO_TEST_CASE(ccheap_remove_reports_missing_objects)
{
  CcHeapTestItem stored = { 2, TRUE };
  CcHeapTestItem absent = { 7, FALSE };

  cc_heap * indexed = cc_heap_construct(2, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(indexed != NULL);
  cc_heap_add(indexed, &stored);
  BOOST_CHECK(!cc_heap_remove(indexed, &absent));
  BOOST_CHECK_EQUAL(cc_heap_elements(indexed), 1u);
  BOOST_CHECK(cc_heap_get_top(indexed) == &stored);
  cc_heap_destruct(indexed);
}

BOOST_AUTO_TEST_CASE(ccheap_removes_root_and_last_element)
{
  CcHeapTestItem storage[] = {
    { 100, TRUE }, { 50, TRUE }, { 90, TRUE },
    { 40, TRUE }, { 45, TRUE }, { 80, TRUE }, { 85, TRUE }
  };
  cc_heap * heap = cc_heap_construct(2, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    cc_heap_add(heap, &storage[i]);
  }

  BOOST_CHECK(cc_heap_remove(heap, &storage[0]));
  storage[0].active = FALSE;
  ccheap_test_check_top(heap, storage, 7, TRUE);

  BOOST_CHECK(cc_heap_remove(heap, &storage[5]));
  storage[5].active = FALSE;
  ccheap_test_check_top(heap, storage, 7, TRUE);
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_remove_rebalances_toward_root)
{
  CcHeapTestItem storage[] = {
    { 100, TRUE }, { 50, TRUE }, { 90, TRUE },
    { 40, TRUE }, { 45, TRUE }, { 80, TRUE }, { 85, TRUE }
  };
  cc_heap * heap = cc_heap_construct(7, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    cc_heap_add(heap, &storage[i]);
  }

  BOOST_REQUIRE(cc_heap_remove(heap, &storage[3]));
  storage[3].active = FALSE;
  int previous = std::numeric_limits<int>::max();
  while (!cc_heap_empty(heap)) {
    CcHeapTestItem * item =
      static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_MESSAGE(previous >= item->priority,
                        "remove left an element below a lower-priority parent");
    previous = item->priority;
  }
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_update_moves_in_both_directions)
{
  CcHeapTestItem storage[] = {
    { 10, TRUE }, { 20, TRUE }, { 30, TRUE },
    { 40, TRUE }, { 50, TRUE }, { 60, TRUE }
  };
  cc_heap * heap = cc_heap_construct(2, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    cc_heap_add(heap, &storage[i]);
  }

  storage[0].priority = 100;
  BOOST_CHECK(cc_heap_update(heap, &storage[0]));
  BOOST_CHECK(cc_heap_get_top(heap) == &storage[0]);

  storage[0].priority = -100;
  BOOST_CHECK(cc_heap_update(heap, &storage[0]));
  BOOST_REQUIRE(cc_heap_get_top(heap) != NULL);
  BOOST_CHECK_EQUAL(static_cast<CcHeapTestItem *>(cc_heap_get_top(heap))->priority,
                    60);

  CcHeapTestItem absent = { 1000, FALSE };
  BOOST_CHECK(!cc_heap_update(heap, &absent));
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_randomized_operations_match_model)
{
  const int itemcount = 128;
  CcHeapTestItem storage[itemcount];
  cc_heap * heap = cc_heap_construct(1, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (int i = 0; i < itemcount; ++i) {
    storage[i].priority = 0;
    storage[i].active = FALSE;
  }

  unsigned int state = 0xcc4ea921u;
  for (int operation = 0; operation < 5000; ++operation) {
    const unsigned int choice = ccheap_test_random(state) % 100u;
    int index;

    if (choice < 40u) {
      index = ccheap_test_find_item(storage, itemcount, FALSE, state);
      if (index >= 0) {
        storage[index].priority = static_cast<int>(
          ccheap_test_random(state) % 20001u) - 10000;
        storage[index].active = TRUE;
        cc_heap_add(heap, &storage[index]);
      }
    }
    else if (choice < 75u) {
      index = ccheap_test_find_item(storage, itemcount, TRUE, state);
      if (index >= 0) {
        storage[index].priority = static_cast<int>(
          ccheap_test_random(state) % 20001u) - 10000;
        BOOST_CHECK(cc_heap_update(heap, &storage[index]));
      }
    }
    else if (choice < 95u) {
      if (!cc_heap_empty(heap)) {
        const int expected =
          ccheap_test_best_priority(storage, itemcount, TRUE);
        CcHeapTestItem * item =
          static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
        BOOST_REQUIRE(item != NULL);
        BOOST_CHECK_EQUAL(item->priority, expected);
        item->active = FALSE;
      }
    }
    else {
      cc_heap_clear(heap);
      for (int i = 0; i < itemcount; ++i) storage[i].active = FALSE;
    }
    ccheap_test_check_top(heap, storage, itemcount, TRUE);
  }
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_grows_from_minimum_capacity)
{
  const int itemcount = 4096;
  std::vector<CcHeapTestItem> storage(static_cast<size_t>(itemcount));
  cc_heap * heap = cc_heap_construct(1, ccheap_test_min_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);

  for (int i = 0; i < itemcount; ++i) {
    storage[static_cast<size_t>(i)].priority = (i * 8191) % 4099 - 2049;
    storage[static_cast<size_t>(i)].active = TRUE;
    cc_heap_add(heap, &storage[static_cast<size_t>(i)]);
  }
  BOOST_CHECK_EQUAL(cc_heap_elements(heap),
                    static_cast<unsigned int>(itemcount));

  int previous = std::numeric_limits<int>::min();
  while (!cc_heap_empty(heap)) {
    CcHeapTestItem * item =
      static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK(previous <= item->priority);
    previous = item->priority;
  }
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_equal_priorities_keep_all_distinct_objects)
{
  CcHeapTestItem storage[64];
  cc_heap * heap = cc_heap_construct(2, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (int i = 0; i < 64; ++i) {
    storage[i].priority = 7;
    storage[i].active = TRUE;
    cc_heap_add(heap, &storage[i]);
  }
  BOOST_CHECK_EQUAL(cc_heap_elements(heap), 64u);
  for (int i = 0; i < 64; ++i) {
    CcHeapTestItem * item =
      static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_EQUAL(item->priority, 7);
    BOOST_CHECK(item->active);
    item->active = FALSE;
  }
  BOOST_CHECK(cc_heap_empty(heap));
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_accepts_zero_initial_capacity)
{
  CcHeapTestItem item = { 7, TRUE };
  cc_heap * heap = cc_heap_construct(0, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  cc_heap_add(heap, &item);
  BOOST_CHECK(cc_heap_extract_top(heap) == &item);
  BOOST_CHECK(cc_heap_empty(heap));
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_exhaustive_small_heap_removals)
{
  unsigned int cases = 0;
  unsigned int violations = 0;

  for (int count = 1; count <= 7; ++count) {
    std::vector<int> insertion(static_cast<size_t>(count));
    for (int i = 0; i < count; ++i) insertion[static_cast<size_t>(i)] = i;

    do {
      for (int maximum = 0; maximum <= 1; ++maximum) {
        for (int removed = 0; removed < count; ++removed) {
          std::vector<CcHeapTestItem> storage(static_cast<size_t>(count));
          cc_heap * heap = cc_heap_construct(
            1, maximum ? ccheap_test_max_compare : ccheap_test_min_compare,
            TRUE);
          BOOST_REQUIRE(heap != NULL);

          for (int i = 0; i < count; ++i) {
            storage[static_cast<size_t>(i)].priority = i;
            storage[static_cast<size_t>(i)].active = TRUE;
          }
          for (int i = 0; i < count; ++i) {
            cc_heap_add(heap,
              &storage[static_cast<size_t>(insertion[static_cast<size_t>(i)])]);
          }

          SbBool valid = cc_heap_remove(
            heap, &storage[static_cast<size_t>(removed)]) ? TRUE : FALSE;
          storage[static_cast<size_t>(removed)].active = FALSE;
          valid = valid && ccheap_test_matches_model(
            heap, &storage[0], count, maximum ? TRUE : FALSE);
          valid = valid && ccheap_test_extracts_ordered(
            heap, maximum ? TRUE : FALSE);
          if (!valid) ++violations;
          ++cases;
          cc_heap_destruct(heap);
        }
      }
    } while (std::next_permutation(insertion.begin(), insertion.end()));
  }

  std::ostringstream details;
  details << violations << " of " << cases
          << " exhaustive removal cases broke heap ordering";
  BOOST_CHECK_MESSAGE(violations == 0, details.str());
}

BOOST_AUTO_TEST_CASE(ccheap_randomized_operations_with_arbitrary_remove)
{
  const int itemcount = 96;
  SbBool consistent = TRUE;

  for (int maximum = 0; maximum <= 1; ++maximum) {
    CcHeapTestItem storage[itemcount];
    cc_heap * heap = cc_heap_construct(
      1, maximum ? ccheap_test_max_compare : ccheap_test_min_compare, TRUE);
    BOOST_REQUIRE(heap != NULL);
    for (int i = 0; i < itemcount; ++i) {
      storage[i].priority = 0;
      storage[i].active = FALSE;
    }

    unsigned int state = maximum ? 0x74d2ac19u : 0x1cb57a83u;
    for (int operation = 0; operation < 5000; ++operation) {
      const unsigned int choice = ccheap_test_random(state) % 100u;
      int index;

      if (choice < 32u) {
        index = ccheap_test_find_item(storage, itemcount, FALSE, state);
        if (index >= 0) {
          storage[index].priority = static_cast<int>(
            ccheap_test_random(state) % 20001u) - 10000;
          storage[index].active = TRUE;
          cc_heap_add(heap, &storage[index]);
        }
      }
      else if (choice < 55u) {
        index = ccheap_test_find_item(storage, itemcount, TRUE, state);
        if (index >= 0) {
          storage[index].priority = static_cast<int>(
            ccheap_test_random(state) % 20001u) - 10000;
          if (!cc_heap_update(heap, &storage[index])) consistent = FALSE;
        }
      }
      else if (choice < 78u) {
        index = ccheap_test_find_item(storage, itemcount, TRUE, state);
        if (index >= 0) {
          if (!cc_heap_remove(heap, &storage[index])) consistent = FALSE;
          storage[index].active = FALSE;
          if (cc_heap_remove(heap, &storage[index])) consistent = FALSE;
        }
      }
      else if (choice < 95u) {
        if (!cc_heap_empty(heap)) {
          CcHeapTestItem * item =
            static_cast<CcHeapTestItem *>(cc_heap_extract_top(heap));
          if (item == NULL || !item->active) {
            consistent = FALSE;
          }
          else {
            const int expected = ccheap_test_best_priority(
              storage, itemcount, maximum ? TRUE : FALSE);
            if (item->priority != expected) consistent = FALSE;
            item->active = FALSE;
          }
        }
      }
      else {
        cc_heap_clear(heap);
        for (int i = 0; i < itemcount; ++i) storage[i].active = FALSE;
      }

      if (!ccheap_test_matches_model(
            heap, storage, itemcount, maximum ? TRUE : FALSE)) {
        consistent = FALSE;
      }
    }
    cc_heap_destruct(heap);
  }

  BOOST_CHECK_MESSAGE(consistent,
    "random add/update/remove/extract/clear sequence diverged from its model");
}

BOOST_AUTO_TEST_CASE(ccheap_remove_index_stays_coherent_across_lifecycles)
{
  const int itemcount = 128;
  CcHeapTestItem storage[itemcount];
  cc_heap * heap = cc_heap_construct(1, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);
  for (int i = 0; i < itemcount; ++i) {
    storage[i].priority = (i * 37) % itemcount;
    storage[i].active = TRUE;
    cc_heap_add(heap, &storage[i]);
  }

  SbBool coherent = TRUE;
  unsigned int state = 0xa8374c51u;
  for (int remaining = itemcount; remaining > 0; --remaining) {
    const int index = ccheap_test_find_item(
      storage, itemcount, TRUE, state);
    if (index < 0 || !cc_heap_remove(heap, &storage[index])) {
      coherent = FALSE;
      break;
    }
    storage[index].active = FALSE;
    if (cc_heap_remove(heap, &storage[index])) coherent = FALSE;
    if (cc_heap_update(heap, &storage[index])) coherent = FALSE;
    if (cc_heap_elements(heap) != static_cast<unsigned int>(remaining - 1)) {
      coherent = FALSE;
    }
  }

  cc_heap_clear(heap);
  for (int i = 0; i < itemcount; ++i) {
    storage[i].priority = itemcount - i;
    storage[i].active = TRUE;
    cc_heap_add(heap, &storage[i]);
  }
  for (int i = 0; i < itemcount; ++i) {
    if (!cc_heap_remove(heap, &storage[i])) coherent = FALSE;
    storage[i].active = FALSE;
  }
  if (!cc_heap_empty(heap)) coherent = FALSE;

  BOOST_CHECK_MESSAGE(coherent,
    "remove index lost or retained an entry across remove/clear/reuse");
  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_indexed_duplicate_pointer_lifecycle)
{
  CcHeapTestItem duplicate = { 17, TRUE };
  CcHeapTestItem other = { 5, TRUE };
  cc_heap * heap = cc_heap_construct(1, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);

  cc_heap_add(heap, &duplicate);
  cc_heap_add(heap, &other);
  cc_heap_add(heap, &duplicate);
  BOOST_CHECK_EQUAL(cc_heap_elements(heap), 3u);

  BOOST_CHECK(cc_heap_remove(heap, &duplicate));
  BOOST_CHECK_EQUAL(cc_heap_elements(heap), 2u);
  const SbBool removedsecond = cc_heap_remove(heap, &duplicate) ? TRUE : FALSE;
  BOOST_CHECK_MESSAGE(removedsecond && cc_heap_elements(heap) == 1u &&
                      cc_heap_get_top(heap) == &other,
    "a second stored occurrence of the same pointer became unreachable");

  cc_heap_destruct(heap);
}

BOOST_AUTO_TEST_CASE(ccheap_duplicate_pointer_update_and_extract)
{
  CcHeapTestItem duplicate = { 50, TRUE };
  CcHeapTestItem other = { 40, TRUE };
  cc_heap * heap = cc_heap_construct(1, ccheap_test_max_compare, TRUE);
  BOOST_REQUIRE(heap != NULL);

  cc_heap_add(heap, &duplicate);
  cc_heap_add(heap, &other);
  cc_heap_add(heap, &duplicate);
  cc_heap_add(heap, &duplicate);

  duplicate.priority = 10;
  BOOST_CHECK(cc_heap_update(heap, &duplicate));
  BOOST_CHECK(cc_heap_get_top(heap) == &other);
  BOOST_CHECK(cc_heap_extract_top(heap) == &other);
  BOOST_CHECK(cc_heap_extract_top(heap) == &duplicate);
  BOOST_CHECK(cc_heap_extract_top(heap) == &duplicate);
  BOOST_CHECK(cc_heap_extract_top(heap) == &duplicate);
  BOOST_CHECK(cc_heap_empty(heap));

  cc_heap_destruct(heap);
}
#endif //COIN_TEST_SUITE
