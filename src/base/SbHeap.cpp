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

/*!
  \class SbHeap SbHeap.h Inventor/SbHeap.h
  \brief The SbHeap class is a generic heap class.

  \ingroup coin_base

  FIXME: write doc

  Note: SbHeap is an extension versus the Open Inventor API.
*/

/*!
  \struct SbHeapFuncs SbHeap.h Inventor/SbHeap.h
  \brief The SbHeapFuncs struct is used to specify functions on heap elements.
*/

/*!
  \var SbHeapFuncs::eval_func

  The \e eval_func member is a pointer to a function that should
  return a weight-value for a heap element. Smaller elements are
  extracted first from the heap. The returned value must define a stable
  ordering and must not be NaN.  */
/*!
  \var SbHeapFuncs::get_index_func

  \e get_index_func is a pointer to a function which should return the
  element's heap index. Supplying it makes removing an arbitrary element
  or changing its weight faster than the default linear lookup. It may
  only be supplied together with set_index_func. Each element must then
  store its heap index in its own data structures. An element not in a
  heap must use an index smaller than 1.  */
/*!
  \var SbHeapFuncs::set_index_func

  \e set_index_func is used to set this index value, and will be
  called whenever the element is moved in the heap. It is called with -1
  when an element is removed, extracted, or discarded by emptyHeap(). It
  may be supplied without get_index_func when movement notifications are
  useful but indexed lookup is not.  */

#include <Inventor/SbHeap.h>
#include <cstring>
#include <cstdio>
#include <cassert>

/*!
  Constructor. \a hFuncs specifies the functions for modifying
  and returning information about the heap object, \a initsize
  specifies the initial number of allocated elements. This array
  will automatically grow when necessary, but if you know
  approximately how many elements the heap will contain, you
  should supply this to avoid some reallocs.

  \a hFuncs must provide an evaluation function. An index getter requires
  an index setter, while a setter may be supplied without a getter. A heap
  using an index getter must have exclusive use of each object's stored heap
  index.
*/
SbHeap::SbHeap(const SbHeapFuncs &hFuncs, const int initsize)
  : heap(initsize)
{
  this->funcs = hFuncs;
  assert(funcs.eval_func);
  assert(funcs.get_index_func == NULL || funcs.set_index_func != NULL);
  if (funcs.get_index_func != NULL && funcs.set_index_func == NULL) {
    this->funcs.get_index_func = NULL;
  }
  this->heap.append(NULL);
}

/*!
  Destructor.
*/
SbHeap::~SbHeap(void)
{
}

/*!
  Removes all the elements from the heap. If an index setter is supplied,
  every removed element is assigned index -1.
*/
void
SbHeap::emptyHeap(void)
{
  if (this->funcs.set_index_func) {
    for (int i = 1; i < this->heap.getLength(); ++i) {
      this->funcs.set_index_func(this->heap[i], -1);
    }
  }
  this->heap.truncate(0);
  this->heap.append(NULL);
}

/*!
  Traverses each heap elements, and calls \a func for each element.
  \a func must not be NULL and must not modify this heap while traversing it.
*/
SbBool
SbHeap::traverseHeap(SbBool (*func)(void *, void *), void *userdata) const
{
  if (func == NULL) {
    assert(func != NULL);
    return FALSE;
  }
  SbBool ok = TRUE;
  int hsize = this->heap.getLength() - 1;

  for(int i = 1; (i <= hsize) && ok; i++)
    if (this->heap[i]) ok = func(this->heap[i], userdata);

  return ok;
}

/*!
  Adds a non-NULL element to the heap. Returns the element's heap position.
  The same pointer may occur more than once only when index callbacks are
  not used. Indexed elements must not simultaneously belong to another heap.
*/
int
SbHeap::add(void *obj)
{
  if (obj == NULL) {
    assert(obj != NULL);
    return -1;
  }
#ifndef NDEBUG
  const float weight = this->funcs.eval_func(obj);
  assert(weight == weight); // NaN is not an ordered heap weight.
  // Indexed objects can only represent membership in one heap position.
  if (this->funcs.get_index_func) {
    assert(this->funcs.get_index_func(obj) < 1);
    assert(this->heap.find(obj) < 0);
  }
#endif // !NDEBUG
  return this->heapInsert(obj);
}

/*!
  Removes an element from the heap. \a idx must be between 1 and size(),
  inclusive. An invalid index is rejected without changing the heap in
  release builds and triggers an assertion in debug builds.
*/
void
SbHeap::remove(const int idx)
{
  int hsize = this->heap.getLength()-1;
  if (idx < 1 || idx > hsize) {
    assert(idx >= 1 && idx <= hsize);
    return;
  }

  void * removed = this->heap[idx];
  if (idx == hsize) {
    this->heap.truncate(hsize);
    if (this->funcs.set_index_func)
      this->funcs.set_index_func(removed, -1);
    return;
  }
  this->heap[idx] = this->heap[hsize];
  if (this->funcs.set_index_func)
    this->funcs.set_index_func(this->heap[idx], idx);
  this->heap.truncate(hsize);

  this->newWeightAt(this->heap[idx], idx);
  if (this->funcs.set_index_func)
    this->funcs.set_index_func(removed, -1);
}

/*!
  \overload

  \a obj must be a non-NULL element currently stored in this heap. A missing
  object or a stale callback index is rejected without changing the heap in
  release builds and triggers an assertion in debug builds.
*/
void
SbHeap::remove(void *obj)
{
  if (obj == NULL) {
    assert(obj != NULL);
    return;
  }
  int idx;
  if (this->funcs.get_index_func) {
    idx = this->funcs.get_index_func(obj);
  }
  else { // slow!!!
    idx = this->heap.find(obj);
  }
  if (idx < 1 || idx >= this->heap.getLength() || this->heap[idx] != obj) {
    assert(idx >= 1 && idx < this->heap.getLength() &&
           this->heap[idx] == obj);
    return;
  }
  this->remove(idx);
}

/*!
  Returns and removes the first element in the heap, or \a NULL
  if heap is empty.
*/
void *
SbHeap::extractMin(void)
{
  return this->heapExtractMin();
}

/*!
  Returns the first element in the heap, or \e NULL if heap is empty.
*/
void *
SbHeap::getMin(void)
{
  if (this->heap.getLength() > 1)
    return heap[1];
  else
    return NULL;
}

/*!
  Returns the heap element at index \a idx in the heap. The index must be
  between 1 and size(), inclusive. An invalid index returns NULL in release
  builds and triggers an assertion in debug builds.
*/
void *
SbHeap::operator[](const int idx)
{
  if (idx < 1 || idx >= this->heap.getLength()) {
    assert(idx >= 1 && idx < this->heap.getLength());
    return NULL;
  }
  return heap[idx];
}

/*!
  Fixes heap if necessary when the element at \a hpos has changed weight.
  If you know the element's heap position you can supply it in \a hpos.
  The object must currently occupy that position. A missing object, invalid
  position or stale callback index is rejected without changing the heap in
  release builds and triggers an assertion in debug builds.
*/
void
SbHeap::newWeight(void *obj, int hpos)
{
  if (obj == NULL) {
    assert(obj != NULL);
    return;
  }
#ifndef NDEBUG
  const float weight = this->funcs.eval_func(obj);
  assert(weight == weight); // NaN is not an ordered heap weight.
#endif // !NDEBUG
  int hsize = this->heap.getLength()-1;
  if (hpos < 0) {
    if (this->funcs.get_index_func)
      hpos = this->funcs.get_index_func(obj);
    else
      hpos = this->heap.find(obj);
  }
  if (hpos < 1 || hpos > hsize || this->heap[hpos] != obj) {
    assert(hpos >= 1 && hpos <= hsize && this->heap[hpos] == obj);
    return;
  }

  if (this->funcs.get_index_func == NULL) {
    int occurrences = 0;
    for (int i = 1; i <= hsize; ++i) {
      if (this->heap[i] == obj && ++occurrences > 1) {
        this->buildHeap();
        return;
      }
    }
  }

  this->newWeightAt(obj, hpos);
}

void
SbHeap::newWeightAt(void *obj, const int hpos)
{
  int hsize = this->heap.getLength()-1;
  int i = hpos;

  float (*eval)(void*) = this->funcs.eval_func;
  void (*setindex)(void*,int) = this->funcs.set_index_func;

  if (i > 1 && eval(obj) < eval(heap[i/2])) {
    while (i > 1 && eval(this->heap[i/2]) > eval(obj)) {
      this->heap[i] = this->heap[i/2];
      if (setindex) setindex(this->heap[i], i);
      i >>= 1;
    }
    this->heap[i]=obj;
    if (setindex) setindex(obj, i);
  }
  if ((i<<1) > hsize) return;
  if (eval(obj) > eval(this->heap[i*2])) this->heapify(i);
  else {
    if ((i<<1)+1 > hsize) return;
    if (eval(obj) > eval(this->heap[i*2+1])) this->heapify(i);
  }
}

/*!
  Builds heap out of randomly ordered data-structure.
*/
SbBool
SbHeap::buildHeap(SbBool (*progresscb)(float percentage, void *data),
                  void *data)
{
  SbBool ok = TRUE;
  int hsize = this->heap.getLength()-1;

#ifndef NDEBUG
  for (int i = 1; i <= hsize; ++i) {
    const float weight = this->funcs.eval_func(this->heap[i]);
    assert(weight == weight); // NaN is not an ordered heap weight.
  }
#endif // !NDEBUG

  int nrelems = hsize >> 1;

  for (int i = nrelems; (i >= 1) && ok; i--) {
    this->heapify(i);
    if(progresscb && ((i & 31) == 0))
      ok = progresscb(static_cast<float>(nrelems - i)/static_cast<float>(nrelems), data);
  }
  return ok;
}

/*!
  Returns the number of elements in the heap.
*/
int
SbHeap::size(void) const
{
  return this->heap.getLength() - 1;
}

//
// PRIVATE
//

//
// Maintain the heap-structure. Both children must be heaps
//
void
SbHeap::heapify(const int idx)
{
  int l, r, smallest;
  void *tmp;
  int hsize = this->heap.getLength()-1;

  float (*eval)(void*) = this->funcs.eval_func;
  void (*setindex)(void*,int) = this->funcs.set_index_func;

  l = 2*idx;
  r = l+1;
  if (l <= hsize && eval(heap[l]) < eval(heap[idx])) smallest = l;
  else smallest = idx;
  if (r <= hsize && eval(heap[r]) < eval(heap[smallest])) smallest = r;
  if (smallest != idx) {
    tmp = this->heap[idx];
    this->heap[idx] = this->heap[smallest];
    this->heap[smallest] = tmp;
    if (setindex) {
      setindex(this->heap[idx], idx);
      setindex(this->heap[smallest], smallest);
    }
    this->heapify(smallest);
  }
}

//
// Returns the smallest object, and removes it from the heap
//
void *
SbHeap::heapExtractMin(void)
{
  int hsize = this->heap.getLength()-1;
  void *min;
  if (hsize < 1) return NULL;
  min = this->heap[1];
  if (hsize > 1) {
    this->heap[1] = this->heap[hsize];
    if (this->funcs.set_index_func)
      this->funcs.set_index_func(this->heap[1], 1);
  }
  this->heap.truncate(hsize);
  if (hsize > 1) this->heapify(1);
  if (this->funcs.set_index_func)
    this->funcs.set_index_func(min, -1);

  return min;
}

int
SbHeap::heapInsert(void *obj)
{
  int i;
  int hsize = this->heap.getLength();
  i = hsize;
  void (*setindex)(void*,int) = this->funcs.set_index_func;
  float (*eval)(void*) = this->funcs.eval_func;

  this->heap.append(NULL); // will be overwritten later

  while (i > 1 && eval(this->heap[i>>1]) > eval(obj)) {
    this->heap[i] = this->heap[i>>1];
    if (setindex) setindex(this->heap[i], i);
    i >>= 1;
  }
  this->heap[i] = obj;
  if (setindex) setindex(obj, i);
  return i;
}

#ifdef COIN_TEST_SUITE

#include <cfloat>
#include <limits>
#include <type_traits>
#include <vector>

namespace {

struct SbHeapTestItem {
  float weight;
  int index;
  SbBool active;
};

static float
sbheap_test_evaluate(void * object)
{
  return static_cast<SbHeapTestItem *>(object)->weight;
}

static int
sbheap_test_get_index(void * object)
{
  return static_cast<SbHeapTestItem *>(object)->index;
}

static void
sbheap_test_set_index(void * object, int index)
{
  static_cast<SbHeapTestItem *>(object)->index = index;
}

static SbHeapFuncs
sbheap_test_functions(SbBool indexed = TRUE)
{
  SbHeapFuncs functions;
  functions.eval_func = sbheap_test_evaluate;
  functions.get_index_func = indexed ? sbheap_test_get_index : NULL;
  functions.set_index_func = indexed ? sbheap_test_set_index : NULL;
  return functions;
}

static void
sbheap_test_check_invariants(SbHeap & heap,
                             const std::vector<SbHeapTestItem *> & items)
{
  int activecount = 0;
  for (size_t i = 0; i < items.size(); ++i) {
    if (items[i]->active) ++activecount;
  }
  BOOST_REQUIRE_EQUAL(heap.size(), activecount);

  for (int i = 1; i <= heap.size(); ++i) {
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap[i]);
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK(item->active);
    BOOST_CHECK_EQUAL(item->index, i);

    const int left = i * 2;
    const int right = left + 1;
    if (left <= heap.size()) {
      SbHeapTestItem * child = static_cast<SbHeapTestItem *>(heap[left]);
      BOOST_CHECK_MESSAGE(item->weight <= child->weight,
                          "left child violates the min-heap property");
    }
    if (right <= heap.size()) {
      SbHeapTestItem * child = static_cast<SbHeapTestItem *>(heap[right]);
      BOOST_CHECK_MESSAGE(item->weight <= child->weight,
                          "right child violates the min-heap property");
    }
  }
}

static float
sbheap_test_minimum(const std::vector<SbHeapTestItem *> & items)
{
  float minimum = 0.0f;
  SbBool found = FALSE;
  for (size_t i = 0; i < items.size(); ++i) {
    if (items[i]->active && (!found || items[i]->weight < minimum)) {
      minimum = items[i]->weight;
      found = TRUE;
    }
  }
  BOOST_REQUIRE(found);
  return minimum;
}

static SbBool
sbheap_test_visit(void * object, void * closure)
{
  SbHeapTestItem * item = static_cast<SbHeapTestItem *>(object);
  int * visits = static_cast<int *>(closure);
  BOOST_CHECK(item->active);
  ++*visits;
  return TRUE;
}

struct SbHeapTraversalState {
  int visits;
  int limit;
};

static SbBool
sbheap_test_visit_until_limit(void *, void * closure)
{
  SbHeapTraversalState * state = static_cast<SbHeapTraversalState *>(closure);
  ++state->visits;
  return state->visits < state->limit;
}

struct SbHeapProgressState {
  std::vector<float> values;
  int stopafter;
};

static SbBool
sbheap_test_progress(float percentage, void * closure)
{
  SbHeapProgressState * state = static_cast<SbHeapProgressState *>(closure);
  state->values.push_back(percentage);
  return state->stopafter < 0 ||
    static_cast<int>(state->values.size()) < state->stopafter;
}

struct SbHeapLazyItem {
  float sourceweight;
  float cachedweight;
  int index;
  int evaluations;
  SbBool dirty;
};

static float
sbheap_test_lazy_evaluate(void * object)
{
  SbHeapLazyItem * item = static_cast<SbHeapLazyItem *>(object);
  ++item->evaluations;
  if (item->dirty) {
    item->cachedweight = item->sourceweight;
    item->dirty = FALSE;
  }
  return item->cachedweight;
}

static int
sbheap_test_lazy_get_index(void * object)
{
  return static_cast<SbHeapLazyItem *>(object)->index;
}

static void
sbheap_test_lazy_set_index(void * object, int index)
{
  static_cast<SbHeapLazyItem *>(object)->index = index;
}

static SbHeapFuncs
sbheap_test_lazy_functions(void)
{
  SbHeapFuncs functions;
  functions.eval_func = sbheap_test_lazy_evaluate;
  functions.get_index_func = sbheap_test_lazy_get_index;
  functions.set_index_func = sbheap_test_lazy_set_index;
  return functions;
}

static unsigned int
sbheap_test_random(unsigned int & state)
{
  state = state * 1664525u + 1013904223u;
  return state;
}

static int
sbheap_test_find_item(SbHeapTestItem * items, int count, SbBool active,
                      unsigned int & state)
{
  const int start = static_cast<int>(sbheap_test_random(state) %
                                     static_cast<unsigned int>(count));
  for (int offset = 0; offset < count; ++offset) {
    const int index = (start + offset) % count;
    if (items[index].active == active) return index;
  }
  return -1;
}

} // namespace

BOOST_AUTO_TEST_CASE(sbheap_empty_and_single_element)
{
  SbHeap heap(sbheap_test_functions(), 1);
  BOOST_CHECK_EQUAL(heap.size(), 0);
  BOOST_CHECK(heap.getMin() == NULL);
  BOOST_CHECK(heap.extractMin() == NULL);

  SbHeapTestItem item = { 4.0f, -1, TRUE };
  BOOST_CHECK_EQUAL(heap.add(&item), 1);
  BOOST_CHECK_EQUAL(item.index, 1);
  BOOST_CHECK(heap.getMin() == &item);
  BOOST_CHECK(heap.extractMin() == &item);
  BOOST_CHECK_EQUAL(heap.size(), 0);
  BOOST_CHECK(heap.getMin() == NULL);
}

BOOST_AUTO_TEST_CASE(sbheap_extracts_in_nondecreasing_order)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[] = {
    { 8.0f, -1, TRUE }, { -3.0f, -1, TRUE },
    { 5.0f, -1, TRUE }, { 5.0f, -1, TRUE },
    { 1.0f, -1, TRUE }, { 13.0f, -1, TRUE },
    { 0.0f, -1, TRUE }, { -1.0f, -1, TRUE }
  };
  std::vector<SbHeapTestItem *> items;

  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
    sbheap_test_check_invariants(heap, items);
  }

  while (heap.size() > 0) {
    const float expected = sbheap_test_minimum(items);
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_EQUAL(item->weight, expected);
    item->active = FALSE;
    sbheap_test_check_invariants(heap, items);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_repairs_changed_weights)
{
  SbHeap heap(sbheap_test_functions(), 4);
  SbHeapTestItem storage[32];
  std::vector<SbHeapTestItem *> items;

  for (int i = 0; i < 32; ++i) {
    storage[i].weight = static_cast<float>((i * 17) % 41 - 20);
    storage[i].index = -1;
    storage[i].active = TRUE;
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
  }
  sbheap_test_check_invariants(heap, items);

  unsigned int state = 0x13579bdu;
  for (int operation = 0; operation < 256; ++operation) {
    state = state * 1664525u + 1013904223u;
    const int itemindex = static_cast<int>(state % 32u);
    state = state * 1664525u + 1013904223u;
    storage[itemindex].weight = static_cast<float>(static_cast<int>(state % 401u) - 200);

    if ((operation & 1) == 0) {
      heap.newWeight(&storage[itemindex]);
    }
    else {
      heap.newWeight(&storage[itemindex], storage[itemindex].index);
    }
    sbheap_test_check_invariants(heap, items);
    BOOST_REQUIRE(heap.getMin() != NULL);
    BOOST_CHECK_EQUAL(static_cast<SbHeapTestItem *>(heap.getMin())->weight,
                      sbheap_test_minimum(items));
  }
}

BOOST_AUTO_TEST_CASE(sbheap_removes_arbitrary_elements)
{
  SbHeap heap(sbheap_test_functions(), 4);
  SbHeapTestItem storage[31];
  std::vector<SbHeapTestItem *> items;

  for (int i = 0; i < 31; ++i) {
    storage[i].weight = static_cast<float>((i * 23) % 47);
    storage[i].index = -1;
    storage[i].active = TRUE;
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
  }

  for (int i = 0; i < 31; i += 2) {
    if ((i & 2) == 0) heap.remove(&storage[i]);
    else heap.remove(storage[i].index);
    storage[i].active = FALSE;
    sbheap_test_check_invariants(heap, items);
  }

  while (heap.size() > 0) {
    const float expected = sbheap_test_minimum(items);
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_EQUAL(item->weight, expected);
    item->active = FALSE;
    sbheap_test_check_invariants(heap, items);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_builds_after_bulk_weight_changes)
{
  SbHeap heap(sbheap_test_functions(), 4);
  SbHeapTestItem storage[65];
  std::vector<SbHeapTestItem *> items;

  for (int i = 0; i < 65; ++i) {
    storage[i].weight = static_cast<float>(i);
    storage[i].index = -1;
    storage[i].active = TRUE;
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
  }

  for (int i = 0; i < 65; ++i) {
    storage[i].weight = static_cast<float>((i * 37) % 101 - 50);
  }
  BOOST_CHECK(heap.buildHeap());
  sbheap_test_check_invariants(heap, items);

  int visits = 0;
  BOOST_CHECK(heap.traverseHeap(sbheap_test_visit, &visits));
  BOOST_CHECK_EQUAL(visits, heap.size());
}

BOOST_AUTO_TEST_CASE(sbheap_removes_without_index_callbacks)
{
  SbHeap heap(sbheap_test_functions(FALSE), 2);
  SbHeapTestItem storage[] = {
    { 3.0f, -1, TRUE },
    { 1.0f, -1, TRUE },
    { 2.0f, -1, TRUE }
  };

  heap.add(&storage[0]);
  heap.add(&storage[1]);
  heap.add(&storage[2]);
  heap.remove(&storage[1]);

  BOOST_CHECK_EQUAL(heap.size(), 2);
  BOOST_REQUIRE(heap.getMin() != NULL);
  BOOST_CHECK_EQUAL(static_cast<SbHeapTestItem *>(heap.getMin())->weight, 2.0f);
  BOOST_CHECK(heap.extractMin() == &storage[2]);
  BOOST_CHECK(heap.extractMin() == &storage[0]);
  BOOST_CHECK(heap.extractMin() == NULL);
}

BOOST_AUTO_TEST_CASE(sbheap_accepts_zero_and_negative_size_hints)
{
  const int hints[] = { 0, -1, -1024 };
  for (size_t i = 0; i < sizeof(hints) / sizeof(hints[0]); ++i) {
    SbHeap heap(sbheap_test_functions(), hints[i]);
    SbHeapTestItem first = { 2.0f, -1, TRUE };
    SbHeapTestItem second = { 1.0f, -1, TRUE };
    heap.add(&first);
    heap.add(&second);
    BOOST_CHECK_EQUAL(heap.size(), 2);
    BOOST_CHECK(heap.extractMin() == &second);
    BOOST_CHECK(heap.extractMin() == &first);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_can_be_emptied_and_reused)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[16];

  for (int cycle = 0; cycle < 32; ++cycle) {
    for (int i = 0; i < 16; ++i) {
      storage[i].weight = static_cast<float>((i * 11 + cycle * 7) % 29);
      storage[i].index = -1;
      storage[i].active = TRUE;
      heap.add(&storage[i]);
    }
    BOOST_CHECK_EQUAL(heap.size(), 16);
    heap.emptyHeap();
    BOOST_CHECK_EQUAL(heap.size(), 0);
    BOOST_CHECK(heap.getMin() == NULL);
  }

  SbHeapTestItem finalitem = { -1.0f, -1, TRUE };
  BOOST_CHECK_EQUAL(heap.add(&finalitem), 1);
  BOOST_CHECK_EQUAL(finalitem.index, 1);
  BOOST_CHECK(heap.extractMin() == &finalitem);
}

BOOST_AUTO_TEST_CASE(sbheap_empty_heap_invalidates_all_indices)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[] = {
    { 3.0f, -1, TRUE }, { 1.0f, -1, TRUE }, { 2.0f, -1, TRUE }
  };
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    heap.add(&storage[i]);
  }

  heap.emptyHeap();
  BOOST_CHECK_EQUAL(heap.size(), 0);
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    BOOST_CHECK_EQUAL(storage[i].index, -1);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_handles_extreme_ordered_weights)
{
  SbHeap heap(sbheap_test_functions(), 2);
  const float infinity = std::numeric_limits<float>::infinity();
  SbHeapTestItem storage[] = {
    { FLT_MAX, -1, TRUE },
    { -FLT_MAX, -1, TRUE },
    { infinity, -1, TRUE },
    { -infinity, -1, TRUE },
    { 0.0f, -1, TRUE },
    { -0.0f, -1, TRUE }
  };
  std::vector<SbHeapTestItem *> items;

  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
  }

  while (heap.size() > 0) {
    const float expected = sbheap_test_minimum(items);
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_EQUAL(item->weight, expected);
    item->active = FALSE;
  }
}

BOOST_AUTO_TEST_CASE(sbheap_traversal_stops_when_callback_returns_false)
{
  SbHeap heap(sbheap_test_functions(FALSE), 2);
  SbHeapTestItem storage[10];
  for (int i = 0; i < 10; ++i) {
    storage[i].weight = static_cast<float>(i);
    storage[i].index = -1;
    storage[i].active = TRUE;
    heap.add(&storage[i]);
  }

  SbHeapTraversalState state = { 0, 4 };
  BOOST_CHECK(!heap.traverseHeap(sbheap_test_visit_until_limit, &state));
  BOOST_CHECK_EQUAL(state.visits, state.limit);

  heap.emptyHeap();
  state.visits = 0;
  BOOST_CHECK(heap.traverseHeap(sbheap_test_visit_until_limit, &state));
  BOOST_CHECK_EQUAL(state.visits, 0);
}

BOOST_AUTO_TEST_CASE(sbheap_build_progress_is_bounded_and_cancellable)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[257];
  for (int i = 0; i < 257; ++i) {
    storage[i].weight = static_cast<float>(i);
    storage[i].index = -1;
    storage[i].active = TRUE;
    heap.add(&storage[i]);
  }
  for (int i = 0; i < 257; ++i) {
    storage[i].weight = static_cast<float>(256 - i);
  }

  SbHeapProgressState cancelled = { std::vector<float>(), 3 };
  BOOST_CHECK(!heap.buildHeap(sbheap_test_progress, &cancelled));
  BOOST_CHECK_EQUAL(cancelled.values.size(), static_cast<size_t>(3));
  for (size_t i = 0; i < cancelled.values.size(); ++i) {
    BOOST_CHECK(cancelled.values[i] >= 0.0f);
    BOOST_CHECK(cancelled.values[i] <= 1.0f);
    if (i > 0) BOOST_CHECK(cancelled.values[i - 1] <= cancelled.values[i]);
  }

  SbHeapProgressState completed = { std::vector<float>(), -1 };
  BOOST_CHECK(heap.buildHeap(sbheap_test_progress, &completed));
  BOOST_REQUIRE(!completed.values.empty());
  for (size_t i = 0; i < completed.values.size(); ++i) {
    BOOST_CHECK(completed.values[i] >= 0.0f);
    BOOST_CHECK(completed.values[i] <= 1.0f);
    if (i > 0) BOOST_CHECK(completed.values[i - 1] <= completed.values[i]);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_does_not_own_stored_objects)
{
  SbHeapTestItem item = { 7.0f, -1, TRUE };
  {
    SbHeap heap(sbheap_test_functions(), 1);
    heap.add(&item);
  }
  item.weight = 3.0f;
  BOOST_CHECK_EQUAL(item.weight, 3.0f);
}

BOOST_AUTO_TEST_CASE(sbheap_is_not_copyable)
{
  BOOST_STATIC_ASSERT(!std::is_copy_constructible<SbHeap>::value);
  BOOST_STATIC_ASSERT(!std::is_copy_assignable<SbHeap>::value);
}

BOOST_AUTO_TEST_CASE(sbheap_removed_objects_have_invalid_indices)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[] = {
    { 3.0f, -1, TRUE },
    { 1.0f, -1, TRUE },
    { 2.0f, -1, TRUE }
  };

  heap.add(&storage[0]);
  heap.add(&storage[1]);
  heap.add(&storage[2]);
  heap.remove(&storage[1]);

  BOOST_CHECK_EQUAL(heap.size(), 2);
  BOOST_CHECK_EQUAL(storage[1].index, -1);
  SbHeapTestItem * extracted = static_cast<SbHeapTestItem *>(heap.extractMin());
  BOOST_REQUIRE(extracted != NULL);
  BOOST_CHECK_EQUAL(extracted->index, -1);
}

BOOST_AUTO_TEST_CASE(sbheap_supports_lazy_weight_evaluation)
{
  SbHeap heap(sbheap_test_lazy_functions(), 2);
  SbHeapLazyItem storage[32];

  for (int i = 0; i < 32; ++i) {
    storage[i].sourceweight = static_cast<float>((i * 19) % 43 - 21);
    storage[i].cachedweight = 10000.0f;
    storage[i].index = -1;
    storage[i].evaluations = 0;
    storage[i].dirty = TRUE;
    heap.add(&storage[i]);
  }

  storage[17].sourceweight = -1000.0f;
  storage[17].dirty = TRUE;
  heap.newWeight(&storage[17]);
  BOOST_CHECK(heap.getMin() == &storage[17]);
  BOOST_CHECK(!storage[17].dirty);

  storage[17].sourceweight = 1000.0f;
  storage[17].dirty = TRUE;
  heap.newWeight(&storage[17], storage[17].index);
  BOOST_CHECK(heap.getMin() != &storage[17]);
  BOOST_CHECK(!storage[17].dirty);

  for (int i = 0; i < 32; ++i) {
    storage[i].sourceweight = static_cast<float>((i * 37) % 59 - 29);
    storage[i].dirty = TRUE;
  }
  BOOST_CHECK(heap.buildHeap());

  float previous = -std::numeric_limits<float>::infinity();
  while (heap.size() > 0) {
    SbHeapLazyItem * item = static_cast<SbHeapLazyItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK(previous <= item->sourceweight);
    BOOST_CHECK(!item->dirty);
    BOOST_CHECK(item->evaluations > 0);
    previous = item->sourceweight;
  }
}

BOOST_AUTO_TEST_CASE(sbheap_randomized_operations_match_independent_model)
{
  const int itemcount = 128;
  SbHeapTestItem storage[itemcount];
  std::vector<SbHeapTestItem *> items;
  SbHeap heap(sbheap_test_functions(), 1);

  for (int i = 0; i < itemcount; ++i) {
    storage[i].weight = 0.0f;
    storage[i].index = -1;
    storage[i].active = FALSE;
    items.push_back(&storage[i]);
  }

  unsigned int state = 0x51b4e2adu;
  for (int operation = 0; operation < 5000; ++operation) {
    const unsigned int choice = sbheap_test_random(state) % 100u;
    int index;

    if (choice < 30u) {
      index = sbheap_test_find_item(storage, itemcount, FALSE, state);
      if (index >= 0) {
        storage[index].weight = static_cast<float>(
          static_cast<int>(sbheap_test_random(state) % 20001u) - 10000);
        storage[index].active = TRUE;
        heap.add(&storage[index]);
      }
    }
    else if (choice < 55u) {
      index = sbheap_test_find_item(storage, itemcount, TRUE, state);
      if (index >= 0) {
        storage[index].weight = static_cast<float>(
          static_cast<int>(sbheap_test_random(state) % 20001u) - 10000);
        if ((choice & 1u) == 0u) heap.newWeight(&storage[index]);
        else heap.newWeight(&storage[index], storage[index].index);
      }
    }
    else if (choice < 75u) {
      index = sbheap_test_find_item(storage, itemcount, TRUE, state);
      if (index >= 0) {
        if ((choice & 1u) == 0u) heap.remove(&storage[index]);
        else heap.remove(storage[index].index);
        storage[index].active = FALSE;
      }
    }
    else if (choice < 90u) {
      if (heap.size() > 0) {
        const float expected = sbheap_test_minimum(items);
        SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
        BOOST_REQUIRE(item != NULL);
        BOOST_CHECK_EQUAL(item->weight, expected);
        item->active = FALSE;
      }
      else {
        BOOST_CHECK(heap.extractMin() == NULL);
      }
    }
    else if (choice < 98u) {
      for (int i = 0; i < itemcount; ++i) {
        if (storage[i].active) {
          storage[i].weight = static_cast<float>(
            static_cast<int>(sbheap_test_random(state) % 20001u) - 10000);
        }
      }
      BOOST_CHECK(heap.buildHeap());
    }
    else {
      heap.emptyHeap();
      for (int i = 0; i < itemcount; ++i) {
        storage[i].active = FALSE;
        storage[i].index = -1;
      }
    }

    sbheap_test_check_invariants(heap, items);
    if (heap.size() > 0) {
      BOOST_REQUIRE(heap.getMin() != NULL);
      BOOST_CHECK_EQUAL(static_cast<SbHeapTestItem *>(heap.getMin())->weight,
                        sbheap_test_minimum(items));
    }
    else BOOST_CHECK(heap.getMin() == NULL);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_build_progress_handles_boundary_sizes)
{
  const int sizes[] = { 0, 1, 63, 64, 65, 127, 128, 129 };
  for (size_t sizeindex = 0;
       sizeindex < sizeof(sizes) / sizeof(sizes[0]); ++sizeindex) {
    const int count = sizes[sizeindex];
    std::vector<SbHeapTestItem> storage(static_cast<size_t>(count));
    SbHeap heap(sbheap_test_functions(), count);

    for (int i = 0; i < count; ++i) {
      storage[static_cast<size_t>(i)].weight = static_cast<float>(i);
      storage[static_cast<size_t>(i)].index = -1;
      storage[static_cast<size_t>(i)].active = TRUE;
      heap.add(&storage[static_cast<size_t>(i)]);
    }
    for (int i = 0; i < count; ++i) {
      storage[static_cast<size_t>(i)].weight = static_cast<float>(count - i);
    }

    SbHeapProgressState progress = { std::vector<float>(), -1 };
    BOOST_CHECK(heap.buildHeap(sbheap_test_progress, &progress));
    if (count < 64) BOOST_CHECK(progress.values.empty());
    else BOOST_CHECK(!progress.values.empty());
    for (size_t i = 0; i < progress.values.size(); ++i) {
      BOOST_CHECK(progress.values[i] >= 0.0f);
      BOOST_CHECK(progress.values[i] < 1.0f);
      if (i > 0) BOOST_CHECK(progress.values[i - 1] <= progress.values[i]);
    }

    float previous = -std::numeric_limits<float>::infinity();
    while (heap.size() > 0) {
      SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
      BOOST_REQUIRE(item != NULL);
      BOOST_CHECK(previous <= item->weight);
      previous = item->weight;
    }
  }
}

BOOST_AUTO_TEST_CASE(sbheap_recovers_after_cancelled_build)
{
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem storage[257];
  std::vector<SbHeapTestItem *> items;
  for (int i = 0; i < 257; ++i) {
    storage[i].weight = static_cast<float>(i);
    storage[i].index = -1;
    storage[i].active = TRUE;
    items.push_back(&storage[i]);
    heap.add(&storage[i]);
  }
  for (int i = 0; i < 257; ++i) {
    storage[i].weight = static_cast<float>((i * 101) % 263 - 131);
  }

  SbHeapProgressState cancelled = { std::vector<float>(), 1 };
  BOOST_CHECK(!heap.buildHeap(sbheap_test_progress, &cancelled));
  BOOST_CHECK_EQUAL(cancelled.values.size(), static_cast<size_t>(1));

  BOOST_CHECK(heap.buildHeap());
  sbheap_test_check_invariants(heap, items);
  BOOST_REQUIRE(heap.getMin() != NULL);
  BOOST_CHECK_EQUAL(static_cast<SbHeapTestItem *>(heap.getMin())->weight,
                    sbheap_test_minimum(items));
}

BOOST_AUTO_TEST_CASE(sbheap_supports_duplicate_pointers_without_indices)
{
  SbHeap heap(sbheap_test_functions(FALSE), 1);
  SbHeapTestItem repeated = { 1.0f, -1, TRUE };
  SbHeapTestItem other = { 2.0f, -1, TRUE };

  heap.add(&repeated);
  heap.add(&other);
  heap.add(&repeated);
  BOOST_CHECK_EQUAL(heap.size(), 3);

  heap.remove(&repeated);
  BOOST_CHECK_EQUAL(heap.size(), 2);
  BOOST_CHECK(heap.extractMin() == &repeated);
  BOOST_CHECK(heap.extractMin() == &other);
  BOOST_CHECK(heap.extractMin() == NULL);
}

BOOST_AUTO_TEST_CASE(sbheap_new_weight_repairs_all_duplicate_occurrences)
{
  SbHeap heap(sbheap_test_functions(FALSE), 2);
  SbHeapTestItem repeated = { 0.0f, -1, TRUE };
  SbHeapTestItem other[] = {
    { 1.0f, -1, TRUE }, { 2.0f, -1, TRUE }, { 3.0f, -1, TRUE }
  };

  heap.add(&repeated);
  heap.add(&repeated);
  heap.add(&repeated);
  for (size_t i = 0; i < sizeof(other) / sizeof(other[0]); ++i) {
    heap.add(&other[i]);
  }

  repeated.weight = 10.0f;
  heap.newWeight(&repeated);
  for (size_t i = 0; i < sizeof(other) / sizeof(other[0]); ++i) {
    BOOST_CHECK(heap.extractMin() == &other[i]);
  }
  BOOST_CHECK(heap.extractMin() == &repeated);
  BOOST_CHECK(heap.extractMin() == &repeated);
  BOOST_CHECK(heap.extractMin() == &repeated);
  BOOST_CHECK(heap.extractMin() == NULL);
}

BOOST_AUTO_TEST_CASE(sbheap_allows_index_setter_without_getter)
{
  SbHeapFuncs functions = sbheap_test_functions(FALSE);
  functions.set_index_func = sbheap_test_set_index;
  SbHeap heap(functions, 2);
  SbHeapTestItem storage[] = {
    { 3.0f, -1, TRUE }, { 1.0f, -1, TRUE }, { 2.0f, -1, TRUE }
  };
  for (size_t i = 0; i < sizeof(storage) / sizeof(storage[0]); ++i) {
    heap.add(&storage[i]);
    BOOST_CHECK(storage[i].index >= 1);
  }

  heap.remove(&storage[0]);
  BOOST_CHECK_EQUAL(storage[0].index, -1);
  BOOST_CHECK_EQUAL(heap.size(), 2);
  while (heap.size() > 0) {
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK_EQUAL(item->index, -1);
  }
}

BOOST_AUTO_TEST_CASE(sbheap_getter_without_setter_falls_back_in_release)
{
#ifdef NDEBUG
  SbHeapFuncs functions = sbheap_test_functions(FALSE);
  functions.get_index_func = sbheap_test_get_index;
  SbHeap heap(functions, 1);
  SbHeapTestItem item = { 1.0f, -1, TRUE };
  heap.add(&item);
  heap.remove(&item);
  BOOST_CHECK_EQUAL(heap.size(), 0);
#else
  BOOST_CHECK(TRUE);
#endif // NDEBUG
}

BOOST_AUTO_TEST_CASE(sbheap_grows_through_many_reallocations)
{
  const int itemcount = 4096;
  std::vector<SbHeapTestItem> storage(static_cast<size_t>(itemcount));
  SbHeap heap(sbheap_test_functions(), 1);

  for (int i = 0; i < itemcount; ++i) {
    SbHeapTestItem & item = storage[static_cast<size_t>(i)];
    item.weight = static_cast<float>((i * 8191) % 4099 - 2049);
    item.index = -1;
    item.active = TRUE;
    heap.add(&item);
  }
  BOOST_CHECK_EQUAL(heap.size(), itemcount);
  for (int i = 1; i <= heap.size(); ++i) {
    BOOST_CHECK_EQUAL(static_cast<SbHeapTestItem *>(heap[i])->index, i);
  }

  float previous = -std::numeric_limits<float>::infinity();
  while (heap.size() > 0) {
    SbHeapTestItem * item = static_cast<SbHeapTestItem *>(heap.extractMin());
    BOOST_REQUIRE(item != NULL);
    BOOST_CHECK(previous <= item->weight);
    previous = item->weight;
  }
}

BOOST_AUTO_TEST_CASE(sbheap_invalid_operations_are_noops_in_release)
{
#ifdef NDEBUG
  SbHeap heap(sbheap_test_functions(), 2);
  SbHeapTestItem stored = { 1.0f, -1, TRUE };
  SbHeapTestItem absent = { 0.0f, -1, TRUE };
  heap.add(&stored);

  BOOST_CHECK_EQUAL(heap.add(NULL), -1);
  heap.remove(0);
  heap.remove(2);
  heap.remove(static_cast<void *>(NULL));
  heap.remove(&absent);
  heap.newWeight(NULL);
  heap.newWeight(&absent);
  heap.newWeight(&stored, 2);
  BOOST_CHECK(!heap.traverseHeap(NULL, NULL));
  BOOST_CHECK(heap[0] == NULL);
  BOOST_CHECK(heap[2] == NULL);
  BOOST_CHECK_EQUAL(heap.size(), 1);
  BOOST_CHECK(heap.getMin() == &stored);
#else
  BOOST_CHECK(TRUE);
#endif // NDEBUG
}

#endif // COIN_TEST_SUITE
