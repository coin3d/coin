// Exercise the production implementation with deterministic bucket-allocation
// failures. The macro affects only calls in dict.cpp, not the linked library
// or the allocator that owns dictionary entries.
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "base/dict.h"
#include "base/dictp.h"
#include "tidbitsp.h"
#include "coindefs.h"

static bool failnext = false;
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

#define calloc(count, size) test_calloc(count, size)
#include "../src/base/dict.cpp"
#undef calloc

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

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  if (std::strcmp(argv[1], "relink") == 0)
    return relink(mixed_hash) && relink(collision_hash) ? 0 : 1;
  if (std::strcmp(argv[1], "failure") == 0)
    return failed_growth() ? 0 : 1;
  return 2;
}
