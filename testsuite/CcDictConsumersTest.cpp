// Compile the dictionary and its three callers in isolation. Faults affect
// these sources only; worker-pool and mutex implementations remain real.
#include <config.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>
#include <Inventor/C/base/memalloc.h>
#include <Inventor/C/base/heap.h>
#include <Inventor/C/threads/storage.h>
#include <Inventor/C/threads/sched.h>
#include <Inventor/C/threads/mutex.h>
#include <Inventor/C/threads/thread.h>
#include <Inventor/C/threads/wpool.h>
#include "base/dict.h"
#include "tidbitsp.h"
#include "coindefs.h"

enum Fault { NONE, MALLOC, DICTIONARY, ENTRY, ALLOCATOR, MUTEX, POOL, HEAP };
static Fault fault = NONE;
static unsigned int countdown = 0;
static int owned = 0;

static void arm(Fault next, unsigned int call = 1)
{
  fault = next;
  countdown = call;
}

static bool fail(Fault point)
{
  if (fault != point || --countdown != 0) return false;
  fault = NONE;
  return true;
}

static cc_memalloc * test_allocator(unsigned int size, unsigned int alignment)
{
  if (fail(ALLOCATOR)) return NULL;
  cc_memalloc * result = cc_memalloc_construct_aligned(size, alignment);
  if (result != NULL) ++owned;
  return result;
}
static void test_allocator_destruct(cc_memalloc * allocator)
{
  cc_memalloc_destruct(allocator);
  --owned;
}
static void * test_entry(cc_memalloc * allocator)
{
  return fail(ENTRY) ? NULL : cc_memalloc_allocate(allocator);
}

#define cc_memalloc_construct_aligned test_allocator
#define cc_memalloc_destruct test_allocator_destruct
#define cc_memalloc_allocate test_entry
#include "../src/base/dict.cpp"
#undef cc_memalloc_construct_aligned
#undef cc_memalloc_destruct
#undef cc_memalloc_allocate

static cc_dict * test_dictionary(unsigned int size, float factor)
{
  if (fail(DICTIONARY)) return NULL;
  cc_dict * result = cc_dict_construct(size, factor);
  if (result != NULL) ++owned;
  return result;
}
static void test_dictionary_destruct(cc_dict * dict)
{
  cc_dict_destruct(dict);
  --owned;
}
static void * test_malloc(size_t size)
{
  if (fail(MALLOC)) return NULL;
  void * result = std::malloc(size);
  if (result != NULL) ++owned;
  return result;
}
static void test_free(void * ptr)
{
  if (ptr != NULL) --owned;
  std::free(ptr);
}

#define malloc(size) test_malloc(size)
#define free(ptr) test_free(ptr)
#define cc_dict_construct test_dictionary
#define cc_dict_destruct test_dictionary_destruct
#include "../src/base/heap.cpp"
#undef malloc
#undef free
#undef cc_dict_construct
#undef cc_dict_destruct

#ifdef HAVE_THREADS
static cc_mutex * test_mutex()
{
  if (fail(MUTEX)) return NULL;
  cc_mutex * result = cc_mutex_construct();
  if (result != NULL) ++owned;
  return result;
}
static void test_mutex_destruct(cc_mutex * mutex)
{
  cc_mutex_destruct(mutex);
  --owned;
}
#endif

#define malloc(size) test_malloc(size)
#define free(ptr) test_free(ptr)
#define cc_dict_construct test_dictionary
#define cc_dict_destruct test_dictionary_destruct
#define cc_mutex_construct test_mutex
#define cc_mutex_destruct test_mutex_destruct
#include "../src/threads/storage.cpp"
#undef malloc
#undef free
#undef cc_dict_construct
#undef cc_dict_destruct
#undef cc_mutex_construct
#undef cc_mutex_destruct

#ifdef HAVE_THREADS
static cc_wpool * test_pool(int count)
{
  if (fail(POOL)) return NULL;
  cc_wpool * result = cc_wpool_construct(count);
  if (result != NULL) ++owned;
  return result;
}
static void test_pool_destruct(cc_wpool * pool)
{
  cc_wpool_destruct(pool);
  --owned;
}
static cc_heap * test_heap(unsigned int size, cc_heap_compare_cb * compare,
                           SbBool support_remove)
{
  if (fail(HEAP)) return NULL;
  cc_heap * result = cc_heap_construct(size, compare, support_remove);
  if (result != NULL) ++owned;
  return result;
}
static void test_heap_destruct(cc_heap * heap)
{
  cc_heap_destruct(heap);
  --owned;
}

#define malloc(size) test_malloc(size)
#define free(ptr) test_free(ptr)
#define cc_dict_construct test_dictionary
#define cc_dict_destruct test_dictionary_destruct
#define cc_mutex_construct test_mutex
#define cc_mutex_destruct test_mutex_destruct
#define cc_wpool_construct test_pool
#define cc_wpool_destruct test_pool_destruct
#define cc_heap_construct test_heap
#define cc_heap_destruct test_heap_destruct
#define cc_memalloc_construct_aligned test_allocator
#define cc_memalloc_destruct test_allocator_destruct
#define cc_memalloc_allocate test_entry
#include "../src/threads/sched.cpp"
#undef malloc
#undef free
#undef cc_dict_construct
#undef cc_dict_destruct
#undef cc_mutex_construct
#undef cc_mutex_destruct
#undef cc_wpool_construct
#undef cc_wpool_destruct
#undef cc_heap_construct
#undef cc_heap_destruct
#undef cc_memalloc_construct_aligned
#undef cc_memalloc_destruct
#undef cc_memalloc_allocate
#endif

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return false; } } while (false)

static int compare_ints(void * a, void * b)
{
  const int x = *static_cast<int *>(a), y = *static_cast<int *>(b);
  return (x > y) - (x < y);
}

static bool constructors()
{
  const Fault faults[] = { MALLOC, DICTIONARY, ALLOCATOR };
  for (unsigned int i = 0; i < sizeof(faults) / sizeof(faults[0]); ++i) {
    arm(faults[i]);
    CHECK(cc_heap_construct(4, compare_ints, TRUE) == NULL);
    CHECK(fault == NONE && owned == 0);
    arm(faults[i]);
    CHECK(cc_storage_construct(sizeof(int)) == NULL);
    CHECK(fault == NONE && owned == 0);
  }
  arm(MALLOC, 2); // Heap array, after the heap object was allocated.
  CHECK(cc_heap_construct(4, compare_ints, TRUE) == NULL);
  CHECK(fault == NONE && owned == 0);
#ifdef HAVE_THREADS
  arm(MUTEX);
  CHECK(cc_storage_construct(sizeof(int)) == NULL);
  CHECK(fault == NONE && owned == 0);
  const Fault schedfaults[] = { MALLOC, POOL, MUTEX, HEAP, ALLOCATOR, DICTIONARY };
  for (unsigned int i = 0; i < sizeof(schedfaults) / sizeof(schedfaults[0]); ++i) {
    arm(schedfaults[i]);
    CHECK(cc_sched_construct(0) == NULL);
    CHECK(fault == NONE && owned == 0);
  }
  // The first allocator/dictionary belongs to the heap; the second is the
  // scheduler's item pool / ID map. Both unwind all earlier resources.
  arm(ALLOCATOR, 2);
  CHECK(cc_sched_construct(0) == NULL);
  CHECK(fault == NONE && owned == 0);
  arm(DICTIONARY, 2);
  CHECK(cc_sched_construct(0) == NULL);
  CHECK(fault == NONE && owned == 0);
#endif
  return true;
}

static bool heap_failure()
{
  cc_heap * heap = cc_heap_construct(4, compare_ints, TRUE);
  CHECK(heap != NULL);
  int values[] = { 10, 20 };
  cc_heap_add(heap, &values[0]);
  arm(ENTRY);
  cc_heap_add(heap, &values[1]);
  CHECK(fault == NONE && cc_heap_elements(heap) == 1);
  CHECK(cc_heap_get_top(heap) == &values[0]);
  CHECK(!cc_heap_remove(heap, &values[1]));
  cc_heap_add(heap, &values[1]);
  CHECK(cc_heap_elements(heap) == 2);
  CHECK(cc_heap_extract_top(heap) == &values[1]);
  CHECK(cc_heap_extract_top(heap) == &values[0]);
  cc_heap_destruct(heap);
  return owned == 0;
}

static bool heap_rebuild_failure()
{
  cc_heap * heap = cc_heap_construct(4, compare_ints, TRUE);
  CHECK(heap != NULL);
  int a = 10, b = 20;
  cc_heap_add(heap, &a);
  cc_heap_add(heap, &b);
  cc_heap_add(heap, &a);
  a = 30;
  arm(ENTRY);
  CHECK(cc_heap_update(heap, &a));
  CHECK(fault == ENTRY); // Reindexing only replaces existing entries.
  fault = NONE;
  CHECK(heap->duplicates == 1);
  CHECK(cc_heap_get_top(heap) == &a);
  CHECK(cc_heap_remove(heap, &a));
  CHECK(cc_heap_remove(heap, &a));
  CHECK(!cc_heap_remove(heap, &a));
  CHECK(cc_heap_extract_top(heap) == &b);
  cc_heap_destruct(heap);
  return owned == 0;
}

static unsigned int constructed = 0, destroyed = 0;
static void storage_construct(void * ptr)
{
  ++constructed;
  if (ptr != NULL) *static_cast<int *>(ptr) = 42;
}
static void storage_destruct(void *) { ++destroyed; }

static bool storage_failure()
{
  cc_storage * storage = cc_storage_construct_etc(sizeof(int), storage_construct,
                                                 storage_destruct);
  CHECK(storage != NULL);
  arm(MALLOC);
  CHECK(cc_storage_get(storage) == NULL);
  CHECK(fault == NONE && constructed == 0 && destroyed == 0);
  arm(ENTRY);
  CHECK(cc_storage_get(storage) == NULL);
  CHECK(fault == NONE && constructed == 1 && destroyed == 1);
  CHECK(cc_dict_get_num_elements(storage->dict) == 0);
  void * value = cc_storage_get(storage);
  CHECK(value != NULL && *static_cast<int *>(value) == 42);
  CHECK(cc_storage_get(storage) == value);
  CHECK(constructed == 2 && destroyed == 1);
  cc_storage_destruct(storage);
  CHECK(destroyed == 2);
  return owned == 0;
}

#ifdef HAVE_THREADS
static void job(void *) { }

static bool scheduler_failure()
{
  // No workers: jobs remain queued, so rollback tests require no timing or
  // sleeps and cannot race with a running callback.
  cc_sched * sched = cc_sched_construct(0);
  CHECK(sched != NULL);
  for (unsigned int step = 1; step <= 3; ++step) {
    // Item pool, heap dictionary, then scheduler ID dictionary.
    arm(ENTRY, step);
    CHECK(cc_sched_schedule(sched, job, NULL, 1.0f) == 0);
    CHECK(fault == NONE);
    CHECK(cc_heap_elements(sched->itemheap) == 0);
    CHECK(cc_dict_get_num_elements(sched->schedid_dict) == 0);
    CHECK(sched->itemalloc != NULL);
    const uint32_t id = cc_sched_schedule(sched, job, NULL, 1.0f);
    CHECK(id != 0 && cc_sched_get_num_remaining(sched) == 1);
    CHECK(cc_sched_unschedule(sched, id));
    CHECK(cc_sched_get_num_remaining(sched) == 0);
  }
  cc_sched_destruct(sched);
  return owned == 0;
}

static bool scheduler_ids()
{
  cc_sched * sched = cc_sched_construct(0);
  CHECK(sched != NULL);
  sched->schedid_counter = UINT32_MAX;
  const uint32_t first = cc_sched_schedule(sched, job, NULL, 1.0f);
  const uint32_t second = cc_sched_schedule(sched, job, NULL, 1.0f);
  CHECK(first == UINT32_MAX && second == 1);
  sched->schedid_counter = UINT32_MAX;
  const uint32_t third = cc_sched_schedule(sched, job, NULL, 1.0f);
  CHECK(third == 2 && cc_sched_get_num_remaining(sched) == 3);
  CHECK(cc_sched_unschedule(sched, first));
  CHECK(cc_sched_unschedule(sched, second));
  CHECK(cc_sched_unschedule(sched, third));
  CHECK(cc_sched_get_num_remaining(sched) == 0);
  cc_sched_destruct(sched);
  return owned == 0;
}
#endif

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  if (std::strcmp(argv[1], "constructors") == 0) return constructors() ? 0 : 1;
  if (std::strcmp(argv[1], "heap") == 0) return heap_failure() ? 0 : 1;
  if (std::strcmp(argv[1], "rebuild") == 0) return heap_rebuild_failure() ? 0 : 1;
  if (std::strcmp(argv[1], "storage") == 0) return storage_failure() ? 0 : 1;
#ifdef HAVE_THREADS
  if (std::strcmp(argv[1], "scheduler") == 0) return scheduler_failure() ? 0 : 1;
  if (std::strcmp(argv[1], "ids") == 0) return scheduler_ids() ? 0 : 1;
#endif
  return 2;
}
