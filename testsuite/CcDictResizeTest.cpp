// Exercise the production implementation with deterministic bucket-allocation
// failures. The macro affects only calls in dict.cpp, not the linked library
// or the allocator that owns dictionary entries.
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>
#include <limits>
#include <stdexcept>
#include "base/dict.h"
#include "base/dictp.h"
#include "tidbitsp.h"
#include "coindefs.h"

static bool failnext = false;
static bool failmalloc = false;
static bool failconstruct = false;
static bool failallocate = false;
static unsigned int allocations = 0;
static unsigned int hashes = 0;

static void *
test_calloc(size_t count, size_t size)
{
  ++allocations;
  if (failnext) {
    failnext = false;
    return NULL;
  }
  return std::calloc(count, size);
}

static void *
test_malloc(size_t size)
{
  if (failmalloc) {
    failmalloc = false;
    return NULL;
  }
  return std::malloc(size);
}

static cc_memalloc *
test_memalloc_construct(unsigned int size, unsigned int alignment)
{
  if (failconstruct) {
    failconstruct = false;
    return NULL;
  }
  return cc_memalloc_construct_aligned(size, alignment);
}

static void *
test_memalloc_allocate(cc_memalloc * allocator)
{
  if (failallocate) {
    failallocate = false;
    return NULL;
  }
  return cc_memalloc_allocate(allocator);
}

#define calloc(count, size) test_calloc(count, size)
#define malloc(size) test_malloc(size)
#define cc_memalloc_construct_aligned(size, alignment) test_memalloc_construct(size, alignment)
#define cc_memalloc_allocate(allocator) test_memalloc_allocate(allocator)
#include "../src/base/dict.cpp"
#undef calloc
#undef malloc
#undef cc_memalloc_construct_aligned
#undef cc_memalloc_allocate

#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); \
  return false; } } while (false)

static uintptr_t mixed_hash(uintptr_t key)
{
  ++hashes;
  return key * static_cast<uintptr_t>(2654435761u);
}

static uintptr_t collision_hash(uintptr_t)
{
  ++hashes;
  return 1;
}

static unsigned int hash_calls_before_throw = 0;

static uintptr_t throwing_hash(uintptr_t key)
{
  if (hash_calls_before_throw-- == 0)
    throw std::runtime_error("hash failed");
  return key;
}

static cc_dict_entry * find_entry(cc_dict * dict, uintptr_t key)
{
  for (cc_dict_entry * entry = dict->buckets[dict_get_index(dict, key)];
       entry != NULL; entry = entry->next) {
    if (entry->key == key) return entry;
  }
  return NULL;
}

static bool relink(cc_dict_hash_func * hash)
{
  const unsigned int count = 32;
  cc_dict * dict = cc_dict_construct(17, 2.0f);
  cc_dict_set_hash_func(dict, hash);
  int values[count];
  cc_dict_entry * entries[count];
  for (unsigned int i = 0; i < count; ++i) {
    values[i] = static_cast<int>(i);
    CHECK(cc_dict_put(dict, i, &values[i]));
    entries[i] = find_entry(dict, i);
  }
  // The requested growth is deliberately still below the occupancy target.
  // Moving existing entries must not initiate another resize.
  dict->loadfactor = 0.01f;
  const unsigned int before = allocations;
  hashes = 0;
  dict_resize(dict, 37);
  CHECK(allocations == before + 1);
  CHECK(hashes == count);
  CHECK(dict->size == 37);
  CHECK(dict->threshold == 0);
  CHECK(cc_dict_get_num_elements(dict) == count);
  for (unsigned int i = 0; i < count; ++i) {
    CHECK(find_entry(dict, i) == entries[i]);
    void * value = NULL;
    CHECK(cc_dict_get(dict, i, &value));
    CHECK(value == &values[i]);
    CHECK(cc_dict_remove(dict, i));
  }
  CHECK(cc_dict_get_num_elements(dict) == 0);
  cc_dict_clear(dict);
  CHECK(cc_dict_put(dict, 99, &values[0]));
  void * value = NULL;
  CHECK(cc_dict_get(dict, 99, &value));
  CHECK(value == &values[0]);
  cc_dict_destruct(dict);
  return true;
}

static bool failed_growth()
{
  cc_dict * dict = cc_dict_construct(2, 0.75f);
  int values[3] = { 10, 20, 30 };
  CHECK(cc_dict_put(dict, 0, &values[0]));
  cc_dict_entry ** buckets = dict->buckets;
  cc_dict_entry * first = find_entry(dict, 0);
  const unsigned int size = dict->size;
  const unsigned int threshold = dict->threshold;
  const unsigned int before = allocations;
  failnext = true;
  // Failure is in optional bucket growth, after the entry was inserted.
  CHECK(cc_dict_put(dict, 1, &values[1]));
  CHECK(!failnext);
  CHECK(allocations == before + 1);
  CHECK(dict->buckets == buckets);
  CHECK(dict->size == size);
  CHECK(dict->threshold == threshold);
  CHECK(cc_dict_get_num_elements(dict) == 2);
  CHECK(find_entry(dict, 0) == first);
  cc_dict_entry * second = find_entry(dict, 1);
  CHECK(second != NULL);
  for (unsigned int i = 0; i < 2; ++i) {
    void * value = NULL;
    CHECK(cc_dict_get(dict, i, &value));
    CHECK(value == &values[i]);
  }
  // Overwrites must not try to grow, even above the old threshold.
  CHECK(!cc_dict_put(dict, 1, &values[2]));
  CHECK(allocations == before + 1);
  // A subsequent insertion can retry growth and preserve both entries.
  CHECK(cc_dict_put(dict, 2, &values[2]));
  CHECK(dict->size > size);
  CHECK(find_entry(dict, 0) == first);
  CHECK(find_entry(dict, 1) == second);
  CHECK(cc_dict_get_num_elements(dict) == 3);
  for (unsigned int i = 0; i < 3; ++i) {
    void * value = NULL;
    CHECK(cc_dict_get(dict, i, &value));
    CHECK(value == &values[i == 1 ? 2 : i]);
    CHECK(cc_dict_remove(dict, i));
  }
  cc_dict_clear(dict);
  cc_dict_destruct(dict);
  return true;
}

static bool numeric_loadfactor()
{
  cc_dict * dict = cc_dict_construct(2, 1e10f);
  CHECK(dict != NULL);
  CHECK(dict->threshold == UINT_MAX);
  dict_resize(dict, 37);
  CHECK(dict->threshold == UINT_MAX);
  cc_dict_destruct(dict);

  const float invalid[] = {
    std::numeric_limits<float>::quiet_NaN(),
    std::numeric_limits<float>::infinity(),
    -std::numeric_limits<float>::infinity()
  };
  for (unsigned int i = 0; i < 3; ++i) {
    dict = cc_dict_construct(2, invalid[i]);
    CHECK(dict != NULL);
    CHECK(dict->loadfactor == 0.75f);
    CHECK(dict->threshold == 1);
    cc_dict_destruct(dict);
  }
  return true;
}

static bool allocation_failures()
{
  failmalloc = true;
  CHECK(cc_dict_construct(2, 0.75f) == NULL);
  CHECK(!failmalloc);
  failnext = true;
  CHECK(cc_dict_construct(2, 0.75f) == NULL);
  CHECK(!failnext);
  failconstruct = true;
  CHECK(cc_dict_construct(2, 0.75f) == NULL);
  CHECK(!failconstruct);

  cc_dict * dict = cc_dict_construct(2, 0.75f);
  CHECK(dict != NULL);
  failallocate = true;
  CHECK(cc_dict_try_put(dict, 1, dict) == CC_DICT_PUT_FAILED);
  CHECK(!failallocate);
  CHECK(cc_dict_get_num_elements(dict) == 0);
  CHECK(find_entry(dict, 1) == NULL);
  CHECK(cc_dict_try_put(dict, 1, dict) == CC_DICT_PUT_INSERTED);
  CHECK(cc_dict_try_put(dict, 1, NULL) == CC_DICT_PUT_REPLACED);

  cc_dict_hash_func * original = dict->hashfunc;
  failnext = true;
  cc_dict_set_hash_func(dict, mixed_hash);
  CHECK(!failnext);
  CHECK(dict->hashfunc == original);
  CHECK(find_entry(dict, 1) != NULL);
  failmalloc = true;
  cc_dict_set_hash_func(dict, mixed_hash);
  CHECK(!failmalloc);
  CHECK(dict->hashfunc == original);
  CHECK(find_entry(dict, 1) != NULL);
  cc_dict_destruct(dict);
  return true;
}

struct ApplyRemoval {
  cc_dict * dict;
  unsigned int visits;
  unsigned int seen;
};

static void
remove_current(uintptr_t key, void *, void * closure)
{
  ApplyRemoval * state = static_cast<ApplyRemoval *>(closure);
  state->visits++;
  state->seen |= 1U << key;
  cc_dict_remove(state->dict, key);
}

static bool apply_removes_current()
{
  cc_dict * dict = cc_dict_construct(17, 0.75f);
  CHECK(dict != NULL);
  cc_dict_set_hash_func(dict, collision_hash);
  for (uintptr_t key = 0; key < 4; ++key) {
    CHECK(cc_dict_put(dict, key, dict));
  }
  ApplyRemoval state = { dict, 0, 0 };
  cc_dict_apply(dict, remove_current, &state);
  CHECK(state.visits == 4);
  CHECK(state.seen == 15);
  CHECK(cc_dict_get_num_elements(dict) == 0);
  cc_dict_destruct(dict);
  return true;
}

static bool throwing_hash_preserves_state()
{
  cc_dict * dict = cc_dict_construct(17, 1.0f);
  CHECK(dict != NULL);
  CHECK(cc_dict_put(dict, 0, NULL));
  CHECK(cc_dict_put(dict, 17, NULL));
  cc_dict_entry ** originalbuckets = dict->buckets;
  cc_dict_entry * first = find_entry(dict, 0);
  cc_dict_entry * second = find_entry(dict, 17);
  cc_dict_entry * firstnext = first->next;
  cc_dict_entry * secondnext = second->next;
  hash_calls_before_throw = 1;
  try {
    cc_dict_set_hash_func(dict, throwing_hash);
    CHECK(false);
  }
  catch (const std::runtime_error &) { }
  CHECK(dict->hashfunc == dict_default_hashfunc);
  CHECK(dict->buckets == originalbuckets);
  CHECK(find_entry(dict, 0) == first && first->next == firstnext);
  CHECK(find_entry(dict, 17) == second && second->next == secondnext);
  void * found = NULL;
  CHECK(cc_dict_get(dict, 0, &found));
  CHECK(cc_dict_get(dict, 17, &found));
  cc_dict_set_hash_func(dict, collision_hash);
  cc_dict_set_hash_func(dict, NULL);
  CHECK(cc_dict_get(dict, 0, &found));
  CHECK(cc_dict_get(dict, 17, &found));
  cc_dict_destruct(dict);

  dict = cc_dict_construct(2, 1.0f);
  CHECK(dict != NULL);
  cc_dict_set_hash_func(dict, throwing_hash);
  hash_calls_before_throw = 10;
  CHECK(cc_dict_put(dict, 0, NULL));
  CHECK(cc_dict_put(dict, 2, NULL));
  originalbuckets = dict->buckets;
  hash_calls_before_throw = 2;
  try {
    cc_dict_put(dict, 4, NULL);
    CHECK(false);
  }
  catch (const std::runtime_error &) { }
  CHECK(cc_dict_get_num_elements(dict) == 3);
  CHECK(dict->buckets == originalbuckets);
  CHECK(dict->size == 2);
  hash_calls_before_throw = 10;
  CHECK(cc_dict_get(dict, 0, &found));
  CHECK(cc_dict_get(dict, 2, &found));
  CHECK(cc_dict_get(dict, 4, &found));
  cc_dict_destruct(dict);
  return true;
}


static bool capacity_boundaries()
{
  const unsigned long largest = 4294967291UL;
  CHECK(coin_exact_prime_at_least(0) == 2);
  CHECK(coin_exact_prime_at_least(3) == 3);
  CHECK(coin_exact_prime_at_least(18) == 19);
  CHECK(coin_exact_prime_at_least(largest - 1) == largest);
  CHECK(coin_exact_prime_at_least(largest) == largest);
  CHECK(coin_exact_prime_at_least(largest + 1) == 0);
  CHECK(coin_exact_prime_at_least(ULONG_MAX) == 0);
  CHECK(coin_growth_prime_at_least(18) == 37);
  CHECK(coin_growth_prime_at_least(largest + 1) == 0);
  const unsigned int before = allocations;
  CHECK(cc_dict_construct(UINT_MAX, 0.75f) == NULL);
  CHECK(allocations == before);

  cc_dict * dict = cc_dict_construct(18, 0.75f);
  CHECK(dict != NULL);
  CHECK(dict->size == 19);
  for (uintptr_t key = 0; key < 15; ++key)
    CHECK(cc_dict_try_put(dict, key, dict) == CC_DICT_PUT_INSERTED);
  CHECK(dict->size == 37);
  // Exhaust the element counter synthetically without allocating UINT_MAX nodes.
  dict->elements = UINT_MAX;
  CHECK(cc_dict_try_put(dict, 99, dict) == CC_DICT_PUT_FAILED);
  CHECK(find_entry(dict, 99) == NULL);
  CHECK(cc_dict_try_put(dict, 0, NULL) == CC_DICT_PUT_REPLACED);
  CHECK(dict->elements == UINT_MAX);
  dict->elements = 15;
  cc_dict_destruct(dict);

  // A positive subnormal factor must remain valid with a zero threshold.
  dict = cc_dict_construct(2, std::numeric_limits<float>::denorm_min());
  CHECK(dict != NULL);
  CHECK(dict->threshold == 0);
  CHECK(cc_dict_try_put(dict, 0, NULL) == CC_DICT_PUT_INSERTED);
  CHECK(dict->size == 5);
  CHECK(cc_dict_try_put(dict, 1, NULL) == CC_DICT_PUT_INSERTED);
  CHECK(dict->size == 11);
  cc_dict_destruct(dict);
  return true;
}

static bool rebuild_allocation_failures()
{
  cc_dict * dict = cc_dict_construct(17, 2.0f);
  CHECK(dict != NULL);
  for (uintptr_t key = 0; key < 8; ++key)
    CHECK(cc_dict_try_put(dict, key, dict) == CC_DICT_PUT_INSERTED);
  cc_dict_entry * entries[8];
  for (uintptr_t key = 0; key < 8; ++key) entries[key] = find_entry(dict, key);
  cc_dict_entry ** buckets = dict->buckets;
  // Replacement buckets succeed; the custom-hash destination plan fails.
  failmalloc = true;
  cc_dict_set_hash_func(dict, collision_hash);
  CHECK(!failmalloc);
  CHECK(dict->hashfunc == dict_default_hashfunc);
  CHECK(dict->buckets == buckets);
  for (uintptr_t key = 0; key < 8; ++key) CHECK(find_entry(dict, key) == entries[key]);

  cc_dict_set_hash_func(dict, collision_hash);
  CHECK(dict->hashfunc == collision_hash);
  buckets = dict->buckets;
  failmalloc = true;
  dict_resize(dict, 37);
  CHECK(!failmalloc);
  CHECK(dict->buckets == buckets && dict->size == 17);
  for (uintptr_t key = 0; key < 8; ++key) CHECK(find_entry(dict, key) == entries[key]);
  dict_resize(dict, 37);
  CHECK(dict->size == 37);
  for (uintptr_t key = 0; key < 8; ++key) CHECK(find_entry(dict, key) == entries[key]);
  // Reset to the default hash does not need a custom-hash plan allocation.
  cc_dict_set_hash_func(dict, NULL);
  CHECK(dict->hashfunc == dict_default_hashfunc);
  for (uintptr_t key = 0; key < 8; ++key) CHECK(find_entry(dict, key) == entries[key]);
  cc_dict_clear(dict);
  cc_dict_set_hash_func(dict, collision_hash);
  cc_dict_set_hash_func(dict, NULL);
  CHECK(dict->hashfunc == dict_default_hashfunc);
  cc_dict_destruct(dict);
  return true;
}

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  if (std::strcmp(argv[1], "relink") == 0)
    return relink(mixed_hash) && relink(collision_hash) ? 0 : 1;
  if (std::strcmp(argv[1], "failure") == 0)
    return failed_growth() ? 0 : 1;
  if (std::strcmp(argv[1], "numeric") == 0)
    return numeric_loadfactor() ? 0 : 1;
  if (std::strcmp(argv[1], "oom") == 0)
    return allocation_failures() ? 0 : 1;
  if (std::strcmp(argv[1], "apply") == 0)
    return apply_removes_current() ? 0 : 1;
  if (std::strcmp(argv[1], "exception") == 0)
    return throwing_hash_preserves_state() ? 0 : 1;
  if (std::strcmp(argv[1], "capacity") == 0)
    return capacity_boundaries() ? 0 : 1;
  if (std::strcmp(argv[1], "rebuild-oom") == 0)
    return rebuild_allocation_failures() ? 0 : 1;
  return 2;
}
